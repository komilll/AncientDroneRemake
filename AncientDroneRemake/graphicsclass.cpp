////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	playerModel = 0;	
	m_ColorShader = 0;
	m_TextureShader = 0;

	for (int i = 0; i < GROUND_MODEL_LENGTH; i++)
	{
		groundModel[i] = nullptr;
	}
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_hwnd = &hwnd;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize Direct3D.", "Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -256.0f);	

	// Create the model object.
	for (int i = 0; i < GROUND_MODEL_LENGTH; i++)
	{
		groundModel[i] = new ModelClass;
		if (!groundModel[i])
		{
			return false;
		}
	}

	//Initialize the model object.
	result = groundModel[0]->Initialize(m_D3D->GetDevice(), 10, 1);
	groundModel[0]->SetTranslation(-60.0f, -50, 0.0f);
	if (!result) return false;

	result = groundModel[1]->Initialize(m_D3D->GetDevice(), 10, 1);
	groundModel[1]->SetTranslation(-20, -60.0f, 0.0f);
	if (!result) return false;

	result = groundModel[2]->Initialize(m_D3D->GetDevice(), 10, 1);
	groundModel[2]->SetTranslation(20, -50, 0.0f);
	if (!result) return false;

	result = groundModel[3]->Initialize(m_D3D->GetDevice(), 10, 1);
	groundModel[3]->SetTranslation(50, -70, 0.0f);
	if (!result) return false;

	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the color shader object.", "Error", MB_OK);
		return false;
	}

	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the texture shader object.", "Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the color shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if (playerModel)
	{
		playerModel->Shutdown();
		delete playerModel;
		playerModel = 0;
	}

	if (groundModel)
	{
		for (int i = 0; i < GROUND_MODEL_LENGTH; i++)
		{
			groundModel[i]->Shutdown();
			delete groundModel[i];
			groundModel[i] = 0;
		}		
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;

	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	a++;
	if (a > 10)
	{
		a = 0;
		index++;
		if (index == 6)
			index = 0;
		m_TextureShader->SetAnimationData(index, 0, 64.0f, 64.0f, 1024.0f, 1024.0f);
	}

	return true;
}

D3DClass* GraphicsClass::GetD3D()
{
	return m_D3D;
}

HWND * GraphicsClass::GetHWND()
{
	return m_hwnd;
}

void GraphicsClass::SetPlayerModel(ModelClass * player)
{
	playerModel = player;
}

ModelClass *GraphicsClass::GetGroundModel(int index)
{
	return groundModel[index];
}


bool GraphicsClass::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ColorShader->SetColor(D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
	D3DXMatrixTranslation(&worldMatrix, playerModel->GetTranslation().x, playerModel->GetTranslation().y, playerModel->GetTranslation().z);
	playerModel->Render(m_D3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), playerModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	for (int i = 0; i < GROUND_MODEL_LENGTH; i++)
	{
		m_ColorShader->SetColor(D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
		m_D3D->GetWorldMatrix(worldMatrix);
		D3DXMatrixTranslation(&worldMatrix, groundModel[i]->GetTranslation().x, groundModel[i]->GetTranslation().y, groundModel[i]->GetTranslation().z);
		groundModel[i]->Render(m_D3D->GetDeviceContext());

		result = m_ColorShader->Render(m_D3D->GetDeviceContext(), groundModel[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}