#pragma once
#ifndef _ANIMATIONIMPORTER_H_
#define _ANIMATIONIMPORTER_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

class AnimationImporter
{
public:
	struct AnimationPose
	{
		ID3D11ShaderResourceView* texture;
		D3DXVECTOR2 leftTop;
		D3DXVECTOR2 rightTop;
		D3DXVECTOR2 leftBottom;
		D3DXVECTOR2 rightBottom;
	};

public:
	AnimationImporter();
	AnimationImporter(const AnimationImporter&);
	~AnimationImporter();

	bool ImportFile(ID3D11Device* device);	

private:
	AnimationPose* animationPose[6];
};

#endif