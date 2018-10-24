#include "animationimporter.h"

AnimationImporter::AnimationImporter()
{
	for (int i = 0; i < 6; i++)
		animationPose[i] = new AnimationPose;
}

AnimationImporter::AnimationImporter(const AnimationImporter &)
{
}

AnimationImporter::~AnimationImporter()
{
}

bool AnimationImporter::ImportFile(ID3D11Device* device)
{
	HRESULT result = D3DX11CreateShaderResourceViewFromFile(device, "ancient_ball.dds", NULL, NULL, &(animationPose[0]->texture), NULL);
	animationPose[0]->leftTop = D3DXVECTOR2(0.0, 0.0);
	animationPose[0]->leftBottom = D3DXVECTOR2(0.0, 64.0f / 1024.0f);
	animationPose[0]->rightBottom = D3DXVECTOR2(64.0f / 1024.0f, 64.0f / 1024.0f);
	animationPose[0]->rightTop = D3DXVECTOR2(0.0f, 64.0f / 1024.0f);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}
