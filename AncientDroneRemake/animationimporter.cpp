#include "animationimporter.h"

AnimationImporter::AnimationImporter()
{
	for (int i = 0; i < 6; i++)
		animationPose[i] = new AnimationPose;

	animationData = 0;
}

AnimationImporter::AnimationImporter(const AnimationImporter &)
{
}

AnimationImporter::~AnimationImporter()
{
}

bool AnimationImporter::ImportFile(ID3D11Device* device, LPCSTR filename, int frameWidth, int frameHeight, int textureWidth, int textureHeight)
{
	HRESULT result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &texture, NULL);

	if (FAILED(result))
	{
		return false;
	}

	m_frameWidth = frameWidth;
	m_frameHeight = frameHeight;
	m_textureWidth = textureWidth;
	m_textureHeight = textureHeight;

	animationData = new AnimationData();
	animationData->imageWidth = frameWidth;
	animationData->imageHeight = frameHeight;
	animationData->fullScreenWidth = textureWidth;
	animationData->fullScreenHeight = textureHeight;

	return true;
}

bool AnimationImporter::CreateAnimation(int frames, int row)
{
	animationPose[m_currentIndex]->leftTop = D3DXVECTOR2((m_frameWidth * frames) / m_textureWidth, (m_frameHeight * row) / m_textureHeight);
	animationPose[m_currentIndex]->leftBottom = D3DXVECTOR2((m_frameWidth * frames) / m_textureWidth, (m_frameHeight * (row + 1)) / m_textureHeight);
	animationPose[m_currentIndex]->rightBottom = D3DXVECTOR2((m_frameWidth * (frames + 1)) / m_textureWidth, (m_frameHeight * (row + 1)) / m_textureHeight);
	animationPose[m_currentIndex]->rightTop = D3DXVECTOR2((m_frameWidth * (frames + 1)) / m_textureWidth, (m_frameHeight * row) / m_textureHeight);

	animationPose[m_currentIndex]->row = row;
	animationPose[m_currentIndex]->frames = frames;

	m_currentIndex++;

	return true;
}

AnimationData* AnimationImporter::GetAnimationData(int index, int currentFrame)
{
	animationData->row = animationPose[index]->row;
	animationData->column = currentFrame % animationPose[index]->frames;

	return animationData;
}

int AnimationImporter::GetMaxFrames(int index)
{
	return animationPose[index]->frames;
}
