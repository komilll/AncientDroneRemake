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
#include <vector>

#include "animationimporter.h"
#include "modelclass.h"
#include "IEanimationobject.h"

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

		D3DXVECTOR4 tint;
		//D3DXVECTOR2 padding;
	};

public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&);
	~TextureShaderClass();

	bool Initialize(ID3D11Device *device, HWND hwnd, CHAR* animationSheetFilename);
	bool Initialize(ID3D11Device *device, HWND hwnd, CHAR* animationSheetFilename, CHAR* vsFilename, CHAR* psFilename);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, bool movingRight = true);
	void SetColor(D3DXVECTOR4 newColor);
	void SetAnimationData(int row, int column, float imageWidth, float imageHeight, float fullScreenWidth, float fullScreenHeight);
	void SetNextFrame();
	void CheckNextFrame();
	void SetNewAnimation(int index);
	void SetNewAnimationOneShot(int index, int previousAnimation);
	void CreateNewAnimation(int frames, int timePerFrame, int row = -1, bool loop = true);
	void ImportFile(int frameWidth, int frameHeight, int textureWidth, int textureHeight);
	void PrepareImportFile(ID3D11Device * device, LPCSTR filename);
	CHAR* GetAnimationSheetFilename();
	void AddModel(ModelClass* model);
	std::vector<ModelClass*> GetModels();
	void SetAnimationObject(IEAnimationObject* animObject);
	void SetColorTint(D3DXVECTOR4 color);

private:
	bool InitializeShader(ID3D11Device*, HWND, CHAR*, CHAR*, CHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, CHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	//Helper variables
	HWND* m_hwnd;
	ID3D11Device* m_device;
	LPCSTR m_animationSheetFilename;
	IEAnimationObject* m_animObject; //Changing logical state of animation in PlayerAnimationStates

	//Animation variables
	TextureBufferType m_texBufferType;
	AnimationImporter* m_animationImporter;
	int m_currentAnimationFrame = 0;
	int m_currentAnimationIndex = 0;
	int m_currentFrameTime = 0;
	int m_savedAnimationToPlay = -1; //Animation that will be played in loop after playing one shot
	bool m_reverseX = false;
	D3DXVECTOR4 m_colorTint = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	//Buffers and resources
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_textureBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11ShaderResourceView* texture;

	//Rendering variables
	std::vector<ModelClass*> m_models;
};

#endif