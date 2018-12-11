////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURESHADERGENERAlCLASS_H_
#define _TEXTURESHADERGENERAlCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <vector>
#include "animationimporter.h"
#include "modelclass.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureShaderClass
////////////////////////////////////////////////////////////////////////////////
class TextureShaderGeneralClass
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
	TextureShaderGeneralClass();
	TextureShaderGeneralClass(const TextureShaderGeneralClass&);
	~TextureShaderGeneralClass();

	bool Initialize(ID3D11Device*, HWND, CHAR*);
	bool Initialize(ID3D11Device*, HWND, CHAR*, CHAR*, CHAR*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, bool movingRight = true);
	void SetColor(D3DXVECTOR4 newColor);
	void SetAsTransparent(bool isTransparent);
	void SetAsConstantOnScreen();
	bool GetIsConstantOnScreen();
	bool IsTransparent();
	void AddModel(ModelClass * model);
	std::vector<ModelClass*> GetModels();

private:
	bool InitializeShader(ID3D11Device*, HWND, CHAR*, CHAR*, CHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, CHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	TextureBufferType m_texBufferType;
	std::vector<ModelClass*> m_models;
	bool m_reverseX = false;
	bool m_isTransparent = false;
	bool m_constantPosition = false;

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_textureBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11ShaderResourceView* texture;
};

#endif