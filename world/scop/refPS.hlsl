#define	AA 2
#define HASHSCALE 0.1031

/*
// shader input
uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;                 // shader playback time (in seconds)
uniform float     iTimeDelta;            // render time (in seconds)
uniform float     iFrameRate;            // shader frame rate
uniform int       iFrame;                // shader playback frame
uniform float     iChannelTime[4];       // channel playback time (in seconds)
uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube
uniform vec4      iDate;                 // (year, month, day, time in seconds)
*/

float4 main() : SV_Target
{
    return float4(0, 0, 0, 1);
}

/*
cbuffer Time : register(b0)
{
    float iTime;
}

float hash(float3 p3)
{
    p3 = frac(p3 * HASHSCALE);
    p3 += dot(p3, p3.yzx + 19.19);
    return frac((p3.x + p3.y) * p3.z);
}

float3 fade(float3 t)
{
    return t * t * t * (t * (6. * t - 15.) + 10.);
}

float grad(float hash, float3 p)
{
    int h = int(1e4 * hash) & 15;
    float u = h < 8 ? p.x : p.y,
 		  v = h < 4 ? p.y : h == 12 || h == 14 ? p.x : p.z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

//3d perlin noise, based on https://mrl.nyu.edu/~perlin/paper445.pdf and https://mrl.nyu.edu/~perlin/noise/.
float perlinNoise3D(float3 p)
{
    float3 pi = floor(p), pf = p - pi, w = fade(pf);
    return lerp(lerp(lerp(grad(hash(pi + float3(0, 0, 0)), pf - float3(0, 0, 0)),
                           grad(hash(pi + float3(1, 0, 0)), pf - float3(1, 0, 0)), w.x),
            	      lerp(grad(hash(pi + float3(0, 1, 0)), pf - float3(0, 1, 0)),
                	 	   grad(hash(pi + float3(1, 1, 0)), pf - float3(1, 1, 0)), w.x), w.y),
        		 lerp(lerp(grad(hash(pi + float3(0, 0, 1)), pf - float3(0, 0, 1)),
                		   grad(hash(pi + float3(1, 0, 1)), pf - float3(1, 0, 1)), w.x),
            		  lerp(grad(hash(pi + float3(0, 1, 1)), pf - float3(0, 1, 1)),
                		   grad(hash(pi + float3(1, 1, 1)), pf - float3(1, 1, 1)), w.x), w.y), w.z);
}

float fbm(float3 pos, int octaves, float persistence)
{
    float total = 0.0, frequency = 1.0, amplitude = 1.0, maxValue = 0.0;
    for (int i = 0; i < octaves; ++i)
    {
        total += perlinNoise3D(pos * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.0;
    }
    return total / maxValue;
}

float getNoise(float3 p)
{
    return 0.15 * fbm(p + 0.3 * iTime, 4, 0.3);
}

//build a right handed look at view matrix.
float3x3 lookAt(in float3 eye, in float3 center, in float3 up)
{
    float3 f = normalize(center - eye);
    float3 s = normalize(cross(f, up));
    float3 u = cross(s, f);
    return float3x3(s, u, -f);
}

//distance functions
float sdSphere(float3 pos, float radius)
{
    return length(pos) - radius;
}

float sdBox(float3 pos, float3 size)
{
    float3 d = abs(pos) - size;
    return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

//distance operations
float2 opU(float2 a, float2 b)
{
    return a.x < b.x ? a : b;
} //union

float2 mapScene(float3 pos)
{
    float shapeIndex = 0.0;
    float2 res = float2(sdSphere(pos - float3(-3, 0, 0), 2.0), shapeIndex++);
    res = opU(res, float2(sdBox(pos - float3(3, 0, 0), float3(1.5)), shapeIndex++));
    res.x += getNoise(pos);
    return res;
}

float sdScene(float3 pos)
{
    return mapScene(pos).x;
}

float3 calculateNormal(in float3 p)
{
    const float3 t = float3(0.01, 0.0, 0.0);
    float gradX = sdScene(p + t.xyy) - sdScene(p - t.xyy);
    float gradY = sdScene(p + t.yxy) - sdScene(p - t.yxy);
    float gradZ = sdScene(p + t.yyx) - sdScene(p - t.yyx);
    
    float3 normal = float3(gradX, gradY, gradZ);
    return normalize(normal);
}

float2 rayMarch(float3 rayOri, float3 rayDir)
{
    const float MAX_TRACE_DISTANCE = 20.0;
    const int MAX_STEPS = 64;
    
    float totalDistance = 0.0, shapeIndex = -1.0;
    for (int i = 0; i < MAX_STEPS; ++i)
    {
        float2 res = mapScene(rayOri + totalDistance * rayDir);
        float minHitDistance = 0.0005 * totalDistance;
        if (res.x < minHitDistance)
        {
            shapeIndex = res.y;
            break;
        }
        if (totalDistance > MAX_TRACE_DISTANCE)
            break;
        totalDistance += res.x;
    }
    
    return float2(totalDistance, shapeIndex);
}

float3 render(float3 rayOri, float3 rayDir)
{
    float3 color = pow(texture(iChannel1, rayDir).rgb, float3(2.2));
    float2 res = rayMarch(rayOri, rayDir);
    int shapeIndex = int(res.y);
    if (shapeIndex >= 0)
    {
        float3 pos = rayOri + rayDir * res.x;
        float3 normal = calculateNormal(pos);
        float3 refracDir = refract(rayDir, normal, 0.8);
        
        color = pow(texture(iChannel1, refracDir).rgb, float3(2.2)) * 0.9;
        if (shapeIndex == 0)
            color *= float3(0.3, 0.7, 1.0);
    }
    
    return color;
}

void mainImage(out float4 fragColor, in float2 fragCoord)
{
    float3 totalColor = float3(0.0);
    
    float2 angle = (iMouse.z > 0.5) ? 4.0 * (2.0 * iMouse.xy / iResolution.xy - 1.0) : float2(0.0);
    float3 rayOri = float3(12.0 * sin(angle.x), angle.y + 5.0, 12.0 * cos(angle.x));
    float3 rayTgt = float3(0.0);
    
    for (int i = 0; i < AA; ++i)
        for (int k = 0; k < AA; ++k)
        {
            float2 offset = float2(float(i) + .5, float(k) + .5) / float(AA) - .5;
            float2 uv = (fragCoord + offset - iResolution.xy * 0.5) / iResolution.x;
        
            float3x3 viewMat = lookAt(rayOri, rayTgt, float3(0.0, 1.0, 0.0));
            float3 rayDir = normalize(mul(float3(uv, -1.0), viewMat));

            float3 color = render(rayOri, rayDir);
            totalColor += color;
        }
    
    totalColor /= float(AA * AA);
    totalColor = pow(totalColor, float3(0.45));
    fragColor = float4(totalColor, 1.0);
}
*/