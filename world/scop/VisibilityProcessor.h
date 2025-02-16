#pragma once
#include <vector>
#include "Frustum.h"
#include "RenderGroup.h"
#include "Camera.h"
#include <functional>
#include "AssetManager.h"

class VisibilityProcessor {
public:
    //std::shared_future<void> processAsync(RenderGroup const& render_group, RenderGroup const& occluders);
    void initialize(ComPtr<ID3D11Device> const& device, AssetManager& asset_manager, int width, int height);
    void process(ComPtr<ID3D11DeviceContext> const& context, RenderGroup const& render_group, Camera const& camera, RenderGroup const& occluders, ComPtr<ID3D11Buffer> const& camera_info);
    void getResult(RenderGroup& out);
private:
    RenderGroup render_group;
    ComPtr<ID3D11Texture2D> depth_texture;
    ComPtr<ID3D11DepthStencilView> depth_dsv;
    int width;
    int height;

    ComPtr<ID3D11VertexShader> subchunk_vs;
    ComPtr<ID3D11PixelShader> subchunk_ps;
    ComPtr<ID3D11InputLayout> subchunk_il;
    ComPtr<ID3D11Buffer> subchunk_cb;
    ComPtr<ID3D11RasterizerState> subchunk_rs;


    ComPtr<ID3D11Texture2D> hi_z_texture;
    ComPtr<ID3D11RenderTargetView> hi_z_rtv;
    std::vector<ComPtr<ID3D11ShaderResourceView>> hi_z_srv;
    std::vector<ComPtr<ID3D11UnorderedAccessView>> hi_z_uav;
    ComPtr<ID3D11Buffer> hi_z_cb;
    ComPtr<ID3D11ComputeShader> hi_z_cs;
    ComPtr<ID3D11CommandList> hi_z_commands;

    void intializeTextures(ComPtr<ID3D11Device> const& device, int width, int height);
    void initializeSubchunkData(ComPtr<ID3D11Device> const& device);
    void initializeHiZData(ComPtr<ID3D11Device> const& device, int width, int height);
    void applyFrustumCulling(Frustum const& frustum);
    void applyOcclusionCulling(ComPtr<ID3D11DeviceContext> const& context, RenderGroup const& occluders, ComPtr<ID3D11Buffer> const& camera_info_cb);
};
