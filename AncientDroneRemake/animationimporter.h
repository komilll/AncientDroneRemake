#pragma once
#ifndef _ANIMATIONIMPORTER_H_
#define _ANIMATIONIMPORTER_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

struct AnimationData
{
	int row;
	int column;
	float imageWidth;
	float imageHeight;
	float fullScreenWidth;
	float fullScreenHeight;
	int timePerFrame;
	bool loop;
};


class AnimationImporter
{
public:
	struct AnimationPose
	{
		D3DXVECTOR2 leftTop;
		D3DXVECTOR2 rightTop;
		D3DXVECTOR2 leftBottom;
		D3DXVECTOR2 rightBottom;
		int row;
		int frames;
		int timePerFrame;
		bool loop = true;
	};


public:
	AnimationImporter();
	AnimationImporter(const AnimationImporter&);
	~AnimationImporter();

	bool ImportFile(ID3D11Device* device, LPCSTR filename, int frameWidth, int frameHeight, int textureWidth, int textureHeight);
	bool CreateAnimation(int frames, int timePerFrame, int row = -1, bool loop = true);
	AnimationData* GetAnimationData(int index, int currentFrame);
	int GetMaxFrames(int index);

private:
	AnimationData* animationData;
	AnimationPose* animationPose[6];
	ID3D11ShaderResourceView* texture;
	int m_currentIndex = 0;
	int m_frameWidth = 0;
	int m_frameHeight = 0;
	int m_textureWidth = 0;
	int m_textureHeight = 0;
};

#endif