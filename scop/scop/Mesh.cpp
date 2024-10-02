#include "pch.h"
#include "Mesh.h"
#include "Geometry.h"
#include "Buffer.h"

Mesh::Mesh(ComPtr<ID3D11Device> device)
	:Super(ResourceType::Mesh), device(device)
{
}

Mesh::~Mesh()
{
}
void Mesh::CreateColorCube(vec3 origin, vec3 size)
{
	this->geometry_color = make_shared<Geometry<Vertex>>();


	vector<Vertex> vtx;
	vtx.resize(24);
	// ����
	vtx[0].pos = (vec3(origin.x, origin.y + size.y, origin.z)); // �� ����
	vtx[1].pos = (vec3(origin.x, origin.y + size.y, origin.z + size.z)); // �� ����
	vtx[2].pos = (vec3(origin.x + size.x, origin.y + size.y, origin.z + size.z)); // �� ������
	vtx[3].pos = (vec3(origin.x + size.x, origin.y + size.y, origin.z)); // �� ������
	vtx[0].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[1].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[2].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[3].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	//�Ʒ���
	vtx[4].pos = (vec3(origin.x, origin.y, origin.z)); // �� ����
	vtx[5].pos = (vec3(origin.x + size.x, origin.y, origin.z)); // �� ������
	vtx[6].pos = (vec3(origin.x + size.x, origin.y, origin.z + size.z)); //�� ������
	vtx[7].pos = (vec3(origin.x, origin.y, origin.z + size.z)); // �� ����
	vtx[4].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[5].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[6].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[7].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));

	//�ո�
	vtx[8].pos = (vec3(origin.x, origin.y, origin.z)); // ���� �Ʒ�
	vtx[9].pos = (vec3(origin.x, origin.y + size.y, origin.z)); // ���� ��
	vtx[10].pos = (vec3(origin.x + size.x, origin.y + size.y, origin.z)); // ������ ��
	vtx[11].pos = (vec3(origin.x + size.x, origin.y, origin.z)); // ������ �Ʒ�
	vtx[8].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[9].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[10].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[11].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));

	// �޸�
	vtx[12].pos = (vec3(origin.x, origin.y, origin.z + size.z)); //���� �Ʒ�
	vtx[13].pos = (vec3(origin.x + size.x, origin.y, origin.z + size.z)); // ������ �Ʒ�
	vtx[14].pos = (vec3(origin.x + size.x, origin.y + size.y, origin.z + size.z)); // ������ ��
	vtx[15].pos = (vec3(origin.x, origin.y + size.y, origin.z + size.z)); // ���� ��
	vtx[12].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[13].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[14].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[15].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));

	vtx[16].pos = (vec3(origin.x, origin.y, origin.z + size.z)); // �� �Ʒ�
	vtx[17].pos = (vec3(origin.x, origin.y + size.y, origin.z + size.z)); // �� ��
	vtx[18].pos = (vec3(origin.x, origin.y + size.y, origin.z)); // �� ��
	vtx[19].pos = (vec3(origin.x, origin.y, origin.z)); // �� �Ʒ�
	vtx[16].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[17].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[18].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[19].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	//�����ʸ�
	vtx[20].pos = (vec3(origin.x + size.x, origin.y, origin.z + size.z)); // �� �Ʒ�
	vtx[21].pos = (vec3(origin.x + size.x, origin.y, origin.z)); // �� �Ʒ�
	vtx[22].pos = (vec3(origin.x + size.x, origin.y + size.y, origin.z)); // �� ��
	vtx[23].pos = (vec3(origin.x + size.x, origin.y + size.y, origin.z + size.z)); // �� ��
	vtx[20].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[21].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[22].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vtx[23].color = (vec4(0.0f, 1.0f, 0.0f, 1.0f));
	this->geometry_color->SetVertices(vtx);
	vector<uint32> idx(36);

	//����
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;

	//�Ʒ���
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;

	//�ո�
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;

	//�޸�
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;

	//���ʸ�
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;

	//�����ʸ�
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	this->geometry_color->SetIndices(idx);

	this->vertex_color_buffer = make_shared<Buffer<Vertex>>(
		this->device,
		vtx.data(),
		static_cast<uint32>(vtx.size()),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->index_buffer = make_shared<Buffer<uint32>>(this->device, idx.data(), idx.size(), D3D11_BIND_INDEX_BUFFER);

}
//void Mesh::CreateCube(vec3 origin, vec3 size, vec2 uv, vec2 texture_size)
//{
//	this->geometry = make_shared<Geometry<VertexUV>>();
//	vec3 origin_div = origin / 16;
//	vec3 size_div = size / 16;
//
//	vector<VertexUV> vtx;
//	vtx.resize(24);
//	// ����
//	vtx[0].pos = (vec3(origin_div.x, origin_div.y + size_div.y, origin_div.z )); // �� ����
//	vtx[1].pos = (vec3(origin_div.x, origin_div.y + size_div.y, origin_div.z + size_div.z)); // �� ����
//	vtx[2].pos = (vec3(origin_div.x + size_div.x, origin_div.y + size_div.y, origin_div.z + size_div.z)); // �� ������
//	vtx[3].pos = (vec3(origin_div.x + size_div.x, origin_div.y + size_div.y, origin_div.z)); // �� ������
//	vtx[0].uv = (vec2((uv.x + size.z) / texture_size.x, (uv.y + size.z) / texture_size.x));
//	vtx[1].uv = (vec2((uv.x + size.z) / texture_size.x, uv.y / texture_size.x));
//	vtx[2].uv = (vec2((uv.x + size.z) + size.x / texture_size.x, (uv.y) / texture_size.x));
//	vtx[3].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, (uv.y + size.z) / texture_size.x));
//	//�Ʒ���
//	vtx[4].pos = (vec3(origin_div.x, origin_div.y, origin_div.z)); // �� ����
//	vtx[5].pos = (vec3(origin_div.x + size_div.x, origin_div.y, origin_div.z)); // �� ������
//	vtx[6].pos = (vec3(origin_div.x + size_div.x, origin_div.y, origin_div.z + size_div.z)); //�� ������
//	vtx[7].pos = (vec3(origin_div.x, origin_div.y, origin_div.z + size_div.z)); // �� ����
//	vtx[4].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, (uv.y + size.z) / texture_size.x));
//	vtx[5].uv = (vec2((uv.x + size.z + size.x + size.z) / texture_size.x, (uv.y + size.z) / texture_size.x));
//	vtx[6].uv = (vec2((uv.x + size.z + size.x + size.z) / texture_size.x, uv.y / texture_size.x));
//	vtx[7].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, uv.y / texture_size.x));
//
//	//�ո�
//	vtx[8].pos = (vec3(origin_div.x, origin_div.y, origin_div.z)); // ���� �Ʒ�
//	vtx[9].pos = (vec3(origin_div.x, origin_div.y + size_div.y, origin_div.z)); // ���� ��
//	vtx[10].pos = (vec3(origin_div.x + size_div.x, origin_div.y + size_div.y, origin_div.z)); // ������ ��
//	vtx[11].pos = (vec3(origin_div.x + size_div.x, origin_div.y, origin_div.z)); // ������ �Ʒ�
//	vtx[8].uv = (vec2((uv.x + size.z) / texture_size.x, (uv.y + size.z + size.y) / texture_size.x));
//	vtx[9].uv = (vec2((uv.x + size.z) / texture_size.x, (uv.y + size.z) / texture_size.x));
//	vtx[10].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, (uv.y + size.z) / texture_size.x));
//	vtx[11].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, (uv.y + size.z + size.y) / texture_size.x));
//	
//	// �޸�
//	vtx[12].pos = (vec3(origin_div.x, origin_div.y, origin_div.z + size_div.z)); //���� �Ʒ�
//	vtx[13].pos = (vec3(origin_div.x + size_div.x, origin_div.y, origin_div.z + size_div.z)); // ������ �Ʒ�
//	vtx[14].pos = (vec3(origin_div.x + size_div.x, origin_div.y + size_div.y, origin_div.z + size_div.z)); // ������ ��
//	vtx[15].pos = (vec3(origin_div.x, origin_div.y + size_div.y, origin_div.z + size_div.z)); // ���� ��
//	vtx[12].uv = (vec2((uv.x + size.z + size.x + size.z + size.x) / texture_size.x, (uv.y + size.z + size.y) / texture_size.x));
//	vtx[13].uv = (vec2((uv.x + size.z + size.x + size.z) / texture_size.x, (uv.y + size.z + size.y) / texture_size.x));
//	vtx[14].uv = (vec2((uv.x + size.z + size.x + size.z) / texture_size.x, (uv.y + size.z) / texture_size.x));
//	vtx[15].uv = (vec2((uv.x + size.z + size.x + size.z + size.x) / texture_size.x, (uv.y + size.z) / texture_size.x));
//	// ���ʸ�
//	vtx[16].pos = (vec3(origin_div.x, origin_div.y, origin_div.z + size_div.z)); // �� �Ʒ�
//	vtx[17].pos = (vec3(origin_div.x, origin_div.y + size_div.y, origin_div.z + size_div.z)); // �� ��
//	vtx[18].pos = (vec3(origin_div.x, origin_div.y + size_div.y, origin_div.z)); // �� ��
//	vtx[19].pos = (vec3(origin_div.x, origin_div.y, origin_div.z)); // �� �Ʒ�
//	vtx[16].uv = (vec2(uv.x / texture_size.x, (uv.y + size.z + size.y) / texture_size.x));
//	vtx[17].uv = (vec2(uv.x / texture_size.x, (uv.y + size.z) / texture_size.x));
//	vtx[18].uv = (vec2((uv.x + size.z) / texture_size.x, (uv.y + size.z) / texture_size.x));
//	vtx[19].uv = (vec2((uv.x + size.z) / texture_size.x, (uv.y + size.z + size.y) / texture_size.x));
//	//�����ʸ�
//	vtx[20].pos = (vec3(origin_div.x + size_div.x, origin_div.y, origin_div.z + size_div.z)); // �� �Ʒ�
//	vtx[21].pos = (vec3(origin_div.x + size_div.x, origin_div.y, origin_div.z)); // �� �Ʒ�
//	vtx[22].pos = (vec3(origin_div.x + size_div.x, origin_div.y + size_div.y, origin_div.z)); // �� ��
//	vtx[23].pos = (vec3(origin_div.x + size_div.x, origin_div.y + size_div.y, origin_div.z + size_div.z)); // �� ��
//	vtx[20].uv = (vec2((uv.x + size.z + size.x + size.z) / texture_size.x, (uv.y + size.z + size.y) / texture_size.x));
//	vtx[21].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, (uv.y + size.z + size.y) / texture_size.x));
//	vtx[22].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, (uv.y + size.z) / texture_size.x));
//	vtx[23].uv = (vec2((uv.x + size.z + size.x + size.z) / texture_size.x, (uv.y + size.z) / texture_size.x));
//	
//	this->geometry->SetVertices(vtx);
//	vector<uint32> idx(36);
//
//	//����
//	idx[0] = 0; idx[1] = 1; idx[2] = 2;
//	idx[3] = 0; idx[4] = 2; idx[5] = 3;
//
//	//�Ʒ���
//	idx[6] = 4; idx[7] = 5; idx[8] = 6;
//	idx[9] = 4; idx[10] = 6; idx[11] = 7;
//
//	//�ո�
//	idx[12] = 8; idx[13] = 9; idx[14] = 10;
//	idx[15] = 8; idx[16] = 10; idx[17] = 11;
//
//	//�޸�
//	idx[18] = 12; idx[19] = 13; idx[20] = 14;
//	idx[21] = 12; idx[22] = 14; idx[23] = 15;
//
//	//���ʸ�
//	idx[24] = 16; idx[25] = 17; idx[26] = 18;
//	idx[27] = 16; idx[28] = 18; idx[29] = 19;
//
//	//�����ʸ�
//	idx[30] = 20; idx[31] = 21; idx[32] = 22;
//	idx[33] = 20; idx[34] = 22; idx[35] = 23;
//
//	this->geometry->SetIndices(idx);
//
//	this->vertex_buffer = make_shared<Buffer<VertexUV>>(
//		this->device,
//		vtx.data(),
//		static_cast<uint32>(vtx.size()),
//		D3D11_BIND_VERTEX_BUFFER
//	);
//	this->index_buffer = make_shared<Buffer<uint32>>(this->device, idx.data(), idx.size(), D3D11_BIND_INDEX_BUFFER);
//
//}
void Mesh::CreateCube(vec3 origin, vec3 size, vec2 uv, vec2 texture_size)
{
	this->geometry = make_shared<Geometry<VertexUV>>();
	vec3 origin_div = origin / 16;
	vec3 size_div = size / 16;
	vector<VertexUV> vtx;
	vtx.resize(24);
	// ����
	vtx[0].pos = (vec3(0, size_div.y, 0)); // �� ����
	vtx[1].pos = (vec3(0, size_div.y, size_div.z)); // �� ����
	vtx[2].pos = (vec3(size_div.x, size_div.y, size_div.z)); // �� ������
	vtx[3].pos = (vec3(size_div.x, size_div.y, 0)); // �� ������
	vtx[0].uv = (vec2((uv.x + size.z) / texture_size.x, (uv.y + size.z) / texture_size.y)); 
	vtx[1].uv = (vec2((uv.x + size.z) / texture_size.x, uv.y / texture_size.y));
	vtx[2].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, (uv.y) / texture_size.y));
	vtx[3].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, (uv.y + size.z) / texture_size.y));
	//�Ʒ���
	vtx[4].pos = (vec3(0, 0, 0)); // �� ����
	vtx[5].pos = (vec3(size_div.x, 0, 0)); // �� ������
	vtx[6].pos = (vec3(size_div.x, 0, size_div.z)); //�� ������
	vtx[7].pos = (vec3(0, origin_div.y, size_div.z)); // �� ����
	vtx[4].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, (uv.y + size.z) / texture_size.y));
	vtx[5].uv = (vec2((uv.x + size.z + size.x + size.z) / texture_size.x, (uv.y + size.z) / texture_size.y));
	vtx[6].uv = (vec2((uv.x + size.z + size.x + size.z) / texture_size.x, uv.y / texture_size.y));
	vtx[7].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, uv.y / texture_size.y));

	//�ո�
	vtx[8].pos = (vec3(0,0 ,0 )); // ���� �Ʒ�
	vtx[9].pos = (vec3(0, size_div.y, 0)); // ���� ��
	vtx[10].pos = (vec3(size_div.x, size_div.y, 0)); // ������ ��
	vtx[11].pos = (vec3(size_div.x, 0, 0)); // ������ �Ʒ�
	vtx[8].uv = (vec2((uv.x + size.z) / texture_size.x, (uv.y + size.z + size.y) / texture_size.y));
	vtx[9].uv = (vec2((uv.x + size.z) / texture_size.x, (uv.y + size.z) / texture_size.y));
	vtx[10].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, (uv.y + size.z) / texture_size.y));
	vtx[11].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, (uv.y + size.z + size.y) / texture_size.y));

	// �޸�
	vtx[12].pos = (vec3(0, 0, size_div.z)); //���� �Ʒ�
	vtx[13].pos = (vec3(size_div.x, 0, size_div.z)); // ������ �Ʒ�
	vtx[14].pos = (vec3(size_div.x, size_div.y, size_div.z)); // ������ ��
	vtx[15].pos = (vec3(0, size_div.y, size_div.z)); // ���� ��
	vtx[12].uv = (vec2((uv.x + size.z + size.x + size.z + size.x) / texture_size.x, (uv.y + size.z + size.y) / texture_size.y));
	vtx[13].uv = (vec2((uv.x + size.z + size.x + size.z) / texture_size.x, (uv.y + size.z + size.y) / texture_size.y));
	vtx[14].uv = (vec2((uv.x + size.z + size.x + size.z) / texture_size.x, (uv.y + size.z) / texture_size.y));
	vtx[15].uv = (vec2((uv.x + size.z + size.x + size.z + size.x) / texture_size.x, (uv.y + size.z) / texture_size.y));
	// ���ʸ�
	vtx[16].pos = (vec3(0, 0, size_div.z)); // �� �Ʒ�
	vtx[17].pos = (vec3(0, size_div.y, size_div.z)); // �� ��
	vtx[18].pos = (vec3(0, size_div.y, 0)); // �� ��
	vtx[19].pos = (vec3(0, 0, 0)); // �� �Ʒ�
	vtx[16].uv = (vec2(uv.x / texture_size.x, (uv.y + size.z + size.y) / texture_size.y));
	vtx[17].uv = (vec2(uv.x / texture_size.x, (uv.y + size.z) / texture_size.y));
	vtx[18].uv = (vec2((uv.x + size.z) / texture_size.x, (uv.y + size.z) / texture_size.y));
	vtx[19].uv = (vec2((uv.x + size.z) / texture_size.x, (uv.y + size.z + size.y) / texture_size.y));
	//�����ʸ�
	vtx[20].pos = (vec3(size_div.x, 0, size_div.z)); // �� �Ʒ�
	vtx[21].pos = (vec3(size_div.x, 0, 0)); // �� �Ʒ�
	vtx[22].pos = (vec3(size_div.x, size_div.y, 0)); // �� ��
	vtx[23].pos = (vec3(size_div.x, size_div.y, size_div.z)); // �� ��
	vtx[20].uv = (vec2((uv.x + size.z + size.x + size.z) / texture_size.x, (uv.y + size.z + size.y) / texture_size.y));
	vtx[21].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, (uv.y + size.z + size.y) / texture_size.y));
	vtx[22].uv = (vec2((uv.x + size.z + size.x) / texture_size.x, (uv.y + size.z) / texture_size.y));
	vtx[23].uv = (vec2((uv.x + size.z + size.x + size.z) / texture_size.x, (uv.y + size.z) / texture_size.y));

	this->geometry->SetVertices(vtx);
	vector<uint32> idx(36);

	//����
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;

	//�Ʒ���
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;

	//�ո�
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;

	//�޸�
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;

	//���ʸ�
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;

	//�����ʸ�
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	this->geometry->SetIndices(idx);

	this->vertex_buffer = make_shared<Buffer<VertexUV>>(
		this->device,
		vtx.data(),
		static_cast<uint32>(vtx.size()),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->index_buffer = make_shared<Buffer<uint32>>(this->device, idx.data(), idx.size(), D3D11_BIND_INDEX_BUFFER);

}