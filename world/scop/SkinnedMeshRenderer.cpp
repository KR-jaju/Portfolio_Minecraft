#include "pch.h"
#include "SkinnedMeshRenderer.h"

//void	SkinnedMeshRenderer::render(Graphics& graphics, SkinnedMesh& mesh, Armature const& armature)
//{
//	ComPtr<ID3D11DeviceContext> context = graphics.getContext();
//	std::vector<Transform> const& transforms = armature.bone_transform;
//	BoneData bone; // bone의 transform
//	bool	processed[32]; // 본이 이미 처리되었는가?
//
//	if (this->bone_buffer == nullptr || this->bindpose_buffer == nullptr)
//	{
//		this->bone_buffer = make_unique<ConstantBuffer>(graphics.getDevice(), context, bone);
//	}
//	
//	for (int i = 0; i < transforms.size(); ++i)
//	{
//		bone.matrix[i] = transforms[i].toMatrix();
//	}
//	
//}