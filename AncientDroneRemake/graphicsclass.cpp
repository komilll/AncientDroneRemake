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
	m_TextureShaderBackground = 0;
	m_backgroundModel = 0;

	for (int i = 0; i < GROUND_MODEL_LENGTH; i++)
	{
		groundModel[i] = nullptr;
	}

	m_bombModels.reserve(10);
	m_TextureShaders.reserve(10);
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
	result = groundModel[0]->Initialize(m_D3D->GetDevice(), 5, 25);
	groundModel[0]->SetTranslation(-80.0f, -40, 0.0f);
	if (!result) return false;

	result = groundModel[1]->Initialize(m_D3D->GetDevice(), 5, 25);
	groundModel[1]->SetTranslation(80.0f, -90, 0.0f);
	if (!result) return false;

	//result = groundModel[1]->Initialize(m_D3D->GetDevice(), 10, 1);
	//groundModel[1]->SetTranslation(-60, -60.0f, 0.0f);
	//if (!result) return false;

	//result = groundModel[2]->Initialize(m_D3D->GetDevice(), 10, 1);
	//groundModel[2]->SetTranslation(60, -50, 0.0f);
	//if (!result) return false;

	//result = groundModel[3]->Initialize(m_D3D->GetDevice(), 10, 1);
	//groundModel[3]->SetTranslation(80, -70, 0.0f);
	//if (!result) return false;

	result = groundModel[2]->Initialize(m_D3D->GetDevice(), 200, 1);
	groundModel[2]->SetTranslation(0.0f, -90.0f, 0.0f);
	if (!result) return false;

	m_backgroundModel = new ModelClass;
	result = m_backgroundModel->Initialize(m_D3D->GetDevice(), 200, 112.5f);
	m_backgroundModel->SetTranslation(0.0f, 0.0f, 0.0f);
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

	//m_TextureShader = new TextureShaderClass;
	//if (!m_TextureShader)
	//{
	//	return false;
	//}

	//result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	//if (!result)
	//{
	//	MessageBox(hwnd, "Could not initialize the texture shader object.", "Error", MB_OK);
	//	return false;
	//}

	m_TextureShaderBackground = new TextureShaderGeneralClass;
	if (!m_TextureShaderBackground)
	{
		return false;
	}

	result = m_TextureShaderBackground->Initialize(m_D3D->GetDevice(), hwnd, "background.dds");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the texture shader object - Texture Shader General", "Error", MB_OK);
		return false;
	}
	//AddTextureShaderGeneral(m_TextureShaderBackground);

	//Spawn enemies

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the color shader object.
	if (m_TextureShaderBackground)
	{
		m_TextureShaderBackground->Shutdown();
		delete m_TextureShaderBackground;
		m_TextureShaderBackground = 0;
	}

	if (!m_TextureShaders.empty())
	{
		for (std::vector<TextureShaderClass*>::iterator shader_ = m_TextureShaders.begin(); shader_ != m_TextureShaders.end(); ++shader_)
		{
			(*(shader_._Ptr))->Shutdown();
			delete (*(shader_._Ptr));
			(*(shader_._Ptr)) = 0;
		}		
		
		//m_TextureShader->Shutdown();
		//delete m_TextureShader;
		//m_TextureShader = 0;
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

	if (m_backgroundModel)
	{
		m_backgroundModel->Shutdown();
		delete m_backgroundModel;
		m_backgroundModel = 0;
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

	for (int i = 0; i < m_TextureShaders.size(); i++)
		m_TextureShaders.at(i)->CheckNextFrame();	

	m_lastFrameCameraPosition = m_Camera->GetPosition();
	//m_Camera->SetPosition(m_Camera->GetPosition().x - 0.5f, m_Camera->GetPosition().y, m_Camera->GetPosition().z);

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

int GraphicsClass::GetGroundModelCount()
{
	return GROUND_MODEL_LENGTH;
}

void GraphicsClass::SetPlayerAnimation(int index, TextureShaderClass* shader)
{
	for (int i = 0; i < m_TextureShaders.size(); i++)
		if (shader == m_TextureShaders.at(i))
		{
			m_TextureShaders.at(i)->SetNewAnimation(index);
			break;
		}
}

void GraphicsClass::SetPlayerAnimationOneShot(int index, TextureShaderClass * shader, int previousIndex)
{
	for (int i = 0; i < m_TextureShaders.size(); i++)
		if (shader == m_TextureShaders.at(i))
		{
			m_TextureShaders.at(i)->SetNewAnimationOneShot(index, previousIndex);
			break;
		}
}

void GraphicsClass::AddEnemyModel(ModelClass * enemyModel)
{
	m_enemyModels.push_back(enemyModel);
}

void GraphicsClass::AddBombModel(ModelClass * bombModel)
{
	m_bombModels.push_back(bombModel);
}

void GraphicsClass::RemoveBombModel(ModelClass * bombModel)
{
	m_bombModels.erase(std::remove(m_bombModels.begin(), m_bombModels.end(), bombModel));
}

bool GraphicsClass::AddTextureShader(TextureShaderClass * textureShader)
{
	if (textureShader == nullptr)
		return false;

	//for (int i = 0; i < m_TextureShaders.size(); i++)
	//{
	//	if (m_TextureShaders.at(i) == nullptr)
	//		continue;

	//	if (m_TextureShaders.at(i)->GetAnimationSheetFilename() == textureShader->GetAnimationSheetFilename())
	//		return false;
	//}

	m_TextureShaders.push_back(textureShader);
	return true;
}

void GraphicsClass::RemoveTextureShader(TextureShaderClass * textureShader)
{
	m_TextureShaders.erase(std::remove(m_TextureShaders.begin(), m_TextureShaders.end(), textureShader));
}

bool GraphicsClass::AddTextureShaderGeneral(TextureShaderGeneralClass * textureShader)
{
	if (textureShader == nullptr)
		return false;

	m_TextureShadersGeneral.push_back(textureShader);
	return true;
}

void GraphicsClass::RemoveTextureShaderGeneral(TextureShaderGeneralClass * textureShader)
{
	m_TextureShadersGeneral.erase(std::remove(m_TextureShadersGeneral.begin(), m_TextureShadersGeneral.end(), textureShader));
}

bool GraphicsClass::AddColorShader(ColorShaderClass * colorShader)
{
	if (colorShader == nullptr)
		return false;

	m_ColorShaders.push_back(colorShader);
	return true;
}

void GraphicsClass::RemoveColorShader(ColorShaderClass * colorShader)
{
	m_ColorShaders.erase(std::remove(m_ColorShaders.begin(), m_ColorShaders.end(), colorShader));
}

bool GraphicsClass::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	m_D3D->TurnZBufferOff();

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	
	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, m_backgroundModel->GetTranslation().x, m_backgroundModel->GetTranslation().y, m_backgroundModel->GetTranslation().z);
	m_backgroundModel->Render(m_D3D->GetDeviceContext());

	result = m_TextureShaderBackground->Render(m_D3D->GetDeviceContext(), m_backgroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}


	m_D3D->TurnOnAlphaBlending();
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	D3DXMatrixTranslation(&worldMatrix, playerModel->GetTranslation().x, playerModel->GetTranslation().y, playerModel->GetTranslation().z);
	playerModel->Render(m_D3D->GetDeviceContext());

	// Render the model using the color shader.	
	result = m_TextureShaders.at(0)->Render(m_D3D->GetDeviceContext(), playerModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, playerModel->movingRight);
	if (!result)
	{
		return false;
	}

	for (int i = 1; i < m_TextureShaders.size(); i++)
	{
		int b = i;

		if (m_TextureShaders.at(i) == nullptr)
			continue;

		ModelClass* model = nullptr;

		for (int k = 0; k < m_TextureShaders.at(i)->GetModels().size(); k++)
		{
			if ((model = m_TextureShaders.at(i)->GetModels().at(k)) == nullptr)
				continue;

			m_D3D->GetWorldMatrix(worldMatrix);
			if (model->UseRotation())
			{
				D3DXMATRIX rotationMatrix;
				D3DXMatrixRotationZ(&rotationMatrix, model->GetRotation());
				D3DXMatrixTranslation(&worldMatrix, model->GetTranslation().x, model->GetTranslation().y, model->GetTranslation().z);
				D3DXMatrixMultiply(&worldMatrix, &rotationMatrix, &worldMatrix);
			}
			else
				D3DXMatrixTranslation(&worldMatrix, model->GetTranslation().x, model->GetTranslation().y, model->GetTranslation().z);
			if (!model->Render(m_D3D->GetDeviceContext()))
				continue;

			if (!m_TextureShaders.at(i)->Render(m_D3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->movingRight))
				return false;
		}
	}

	m_D3D->TurnOffAlphaBlending();

	for (int i = 0; i < m_TextureShadersGeneral.size(); i++)
	{
		int b = i;

		if (m_TextureShadersGeneral.at(i) == nullptr)
			continue;

		ModelClass* model = nullptr;

		if (m_TextureShadersGeneral.at(i)->IsTransparent())
			m_D3D->TurnOnAlphaBlending();

		for (int k = 0; k < m_TextureShadersGeneral.at(i)->GetModels().size(); k++)
		{
			if ((model = m_TextureShadersGeneral.at(i)->GetModels().at(k)) == nullptr)
				continue;

			m_D3D->GetWorldMatrix(worldMatrix);
			if (m_TextureShadersGeneral.at(i)->GetIsConstantOnScreen())
			{
				D3DXVECTOR3 posDiff = m_Camera->GetPosition() - m_lastFrameCameraPosition;
				model->SetTranslation(model->GetTranslation().x + posDiff.x, model->GetTranslation().y + posDiff.y, model->GetTranslation().z);
			}

			D3DXMatrixTranslation(&worldMatrix, model->GetTranslation().x, model->GetTranslation().y, model->GetTranslation().z);
			if (!model->Render(m_D3D->GetDeviceContext()))
				continue;

			if (!m_TextureShadersGeneral.at(i)->Render(m_D3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->movingRight))
				return false;
		}

		m_D3D->TurnOffAlphaBlending();		
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

	for (int i = 0; i < m_ColorShaders.size(); i++)
	{
		if (m_ColorShaders.at(i) == nullptr)
			continue;

		ModelClass* model = nullptr;

		for (int k = 0; k < m_ColorShaders.at(i)->GetModels().size(); k++)
		{
			if ((model = m_ColorShaders.at(i)->GetModels().at(k)) == nullptr)
				continue;

			m_D3D->GetWorldMatrix(worldMatrix);
			D3DXMatrixTranslation(&worldMatrix, model->GetTranslation().x, model->GetTranslation().y, model->GetTranslation().z);
			if (!model->Render(m_D3D->GetDeviceContext()))
				continue;

			if (!m_ColorShaders.at(i)->Render(m_D3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))
				return false;
		}
	}

	//for (int i = 0; i < m_enemyModels.size(); i++)
	//{
	//	//m_ColorShader->SetColor(D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
	//	m_D3D->GetWorldMatrix(worldMatrix);
	//	D3DXMatrixTranslation(&worldMatrix, m_enemyModels[i]->GetTranslation().x, m_enemyModels[i]->GetTranslation().y, m_enemyModels[i]->GetTranslation().z);
	//	m_enemyModels[i]->Render(m_D3D->GetDeviceContext());

	//	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_enemyModels[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	//	if (!result)
	//	{
	//		return false;
	//	}		
	//}

	for (int i = 0; i < m_bombModels.size(); i++)
	{
		m_ColorShader->SetColor(D3DXVECTOR4(1.0f, 0.5f, 0.3f, 1.0f));
		m_D3D->GetWorldMatrix(worldMatrix);
		D3DXMATRIX scaleMatrix;
		D3DXMatrixScaling(&scaleMatrix, m_bombModels[i]->GetScale().x, m_bombModels[i]->GetScale().y, 1.0f);
		D3DXMatrixTranslation(&worldMatrix, m_bombModels[i]->GetTranslation().x, m_bombModels[i]->GetTranslation().y, m_bombModels[i]->GetTranslation().z);
		D3DXMatrixMultiply(&worldMatrix, &scaleMatrix, &worldMatrix);

		m_bombModels[i]->Render(m_D3D->GetDeviceContext());

		result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_bombModels[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	m_D3D->TurnZBufferOn();

	return true;
}