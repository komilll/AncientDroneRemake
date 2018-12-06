////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>

struct Bounds
{
	D3DXVECTOR2 min;
	D3DXVECTOR2 max;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
		D3DXVECTOR2 tex;
	};	

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, int width, int height);
	void Shutdown();
	bool Render(ID3D11DeviceContext*);

	int GetIndexCount();
	void SetAdditionalTranslation(float x, float y, float z);
	D3DXVECTOR3 GetAdditionalTranslation();
	void SetTranslation(float x, float y, float z);
	D3DXVECTOR3 GetTranslation();
	void SetRotation(float z);
	float GetRotation();
	bool UseRotation();

	/* Physics based methods */
	void SetBounds(float minX, float maxX, float minY, float maxY);
	///<summary>
	///First argument is min bounds, second argument is max bounds
	///</summary>
	Bounds GetBounds();
	void SetScale(float x, float y, float z);
	D3DXVECTOR3 GetScale();
	D3DXVECTOR2 GetSize();
	D3DXVECTOR2 GetOriginalSize();
	void SetVisibility(bool enable);

public:
	/* Physics based components */
	Bounds bounds;
	bool movingRight = true;

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_width, m_height;
	int m_widthOriginal, m_heightOriginal;
	float m_scaleX, m_scaleY, m_scaleZ;
	float m_translationX, m_translationY, m_translationZ;
	float m_additionalTranslationX, m_additionalTranslationY, m_additionalTranslationZ;
	bool m_isVisibile = true;
	bool m_useRotation;
	float m_rotationX, m_rotationY, m_rotationZ;
};

#endif