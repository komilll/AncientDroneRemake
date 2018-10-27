////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include "animationimporter.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureShaderClass
////////////////////////////////////////////////////////////////////////////////
class TextureShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct TextureBufferType
	{
		int row;
		int column;
		float width;
		float height;
		float fullScreenWidth;
		float fullScreenHeight;

		float reverseX;
		float reverseY;
		//D3DXVECTOR2 padding;
	};

public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&);
	~TextureShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void SetColor(D3DXVECTOR4 newColor);
	void SetAnimationData(int row, int column, float imageWidth, float imageHeight, float fullScreenWidth, float fullScreenHeight);
	void SetNextFrame();
	void CheckNextFrame();

private:
	bool InitializeShader(ID3D11Device*, HWND, CHAR*, CHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, CHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	TextureBufferType m_texBufferType;
	AnimationImporter* m_animationImporter;
	int m_currentAnimationFrame = 0;
	int m_currentAnimationIndex = 0;
	int m_currentFrameTime = 0;	

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_textureBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11ShaderResourceView* texture;
};

#endif