///////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	SetScale(1.0f, 1.0f, 1.0f);
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, int width, int height)
{
	bool result;

	m_width = width;
	m_height = height;

	m_widthOriginal = m_width;
	m_heightOriginal = m_height;

	SetBounds(-m_width, m_width, -m_height, m_height);

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}	

	return true;
}


void ModelClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


bool ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	if (m_isVisibile == false)
		return false;

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return true;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

void ModelClass::SetTranslation(float x, float y, float z)
{
	m_translationX = x;
	m_translationY = y;
	m_translationZ = z;
}

D3DXVECTOR3 ModelClass::GetTranslation()
{
	return D3DXVECTOR3(m_translationX, m_translationY, m_translationZ);
}

void ModelClass::SetRotation(float z)
{
	m_rotationZ = z;

	m_useRotation = true;
}

float ModelClass::GetRotation()
{
	return m_rotationZ;
}

bool ModelClass::UseRotation()
{
	return m_useRotation;
}

void ModelClass::SetBounds(float minX, float maxX, float minY, float maxY)
{
	bounds.min = D3DXVECTOR2(minX, minY);
	bounds.max = D3DXVECTOR2(maxX, maxY);
}

Bounds ModelClass::GetBounds()
{
	Bounds localBounds;
	//if (m_useRotation)
	//{
	//	localBounds.min.x = bounds.min.x * cos(m_rotationZ) + m_translationX;
	//	localBounds.max.x = bounds.max.x * cos(m_rotationZ) + m_translationX;
	//	localBounds.min.y = bounds.min.y * sin(m_rotationZ) + m_translationY;
	//	localBounds.max.y = bounds.max.y * sin(m_rotationZ) + m_translationY;
	//}
	
	{
		localBounds.min.x = bounds.min.x + m_translationX;
		localBounds.max.x = bounds.max.x + m_translationX;
		localBounds.min.y = bounds.min.y + m_translationY;
		localBounds.max.y = bounds.max.y + m_translationY;
	}
	
	return localBounds;
}

void ModelClass::SetScale(float x, float y, float z)
{
	m_scaleX = x;
	m_scaleY = y;
	m_scaleZ = z;

	float newWidth = m_widthOriginal * m_scaleX;
	float newHeight = m_heightOriginal * m_scaleY;

	m_width = newWidth;
	m_height = newHeight;

	SetBounds(-newWidth, newWidth, -newHeight, newHeight);
}

D3DXVECTOR3 ModelClass::GetScale()
{
	return D3DXVECTOR3(m_scaleX, m_scaleY, m_scaleZ);
}

D3DXVECTOR2 ModelClass::GetSize()
{
	return D3DXVECTOR2(m_width, m_height);
}

void ModelClass::SetVisibility(bool enable)
{
	m_isVisibile = enable;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Set the number of vertices in the vertex array.
	m_vertexCount = 4;

	// Set the number of indices in the index array.
	m_indexCount = 6;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	float right = m_width;
	float left = -right;
	float top = m_height;
	float bottom = -top;

	//if (m_width > m_height)
	//{
	//	m_scale = m_width;
	//	right = 1.0f;
	//	top = m_height / m_width;
	//}
	//else if (m_height >= m_width)
	//{
	//	m_scale = m_height;
	//	top = 1.0f;
	//	right = m_width / m_height;
	//}

	//left = -right;
	//bottom = -top;

	//animationPose[0]->leftTop = D3DXVECTOR2(0.0, 0.0);
	//animationPose[0]->leftBottom = D3DXVECTOR2(0.0, 64.0f / 1024.0f);
	//animationPose[0]->rightBottom = D3DXVECTOR2(64.0f / 1024.0f, 64.0f / 1024.0f);
	//animationPose[0]->rightTop = D3DXVECTOR2(0.0f, 64.0f / 1024.0f);

	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[0].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[0].tex = D3DXVECTOR2(0.0, 0.0);

	vertices[1].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[1].tex = D3DXVECTOR2(1.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[2].tex = D3DXVECTOR2(1.0f, 1.0f);

	vertices[3].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	vertices[3].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[3].tex = D3DXVECTOR2(0.0, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.
	indices[3] = 0;  // Bottom left.
	indices[4] = 2;  // Top middle.
	indices[5] = 3;  // Bottom right.

					 // Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}