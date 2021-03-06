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

	//for (int i = 0; i < GROUND_MODEL_LENGTH; i++)
	//{
	//	groundModel[i] = nullptr;
	//}

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
	//for (int i = 0; i < GROUND_MODEL_LENGTH; i++)
	//{
	//	groundModel[i] = new ModelClass;
	//	if (!groundModel[i])
	//	{
	//		return false;
	//	}
	//}

	//Initialize the model object.
	//result = groundModel[0]->Initialize(m_D3D->GetDevice(), 5, 25);
	//groundModel[0]->SetTranslation(-80.0f, -40, 0.0f);
	//if (!result) return false;

	//result = groundModel[1]->Initialize(m_D3D->GetDevice(), 5, 25);
	//groundModel[1]->SetTranslation(80.0f, -90, 0.0f);
	//if (!result) return false;

	//result = groundModel[1]->Initialize(m_D3D->GetDevice(), 10, 1);
	//groundModel[1]->SetTranslation(-60, -60.0f, 0.0f);
	//if (!result) return false;

	//result = groundModel[2]->Initialize(m_D3D->GetDevice(), 10, 1);
	//groundModel[2]->SetTranslation(60, -50, 0.0f);
	//if (!result) return false;

	//result = groundModel[3]->Initialize(m_D3D->GetDevice(), 10, 1);
	//groundModel[3]->SetTranslation(80, -70, 0.0f);
	//if (!result) return false;

	//result = groundModel[2]->Initialize(m_D3D->GetDevice(), 200, 1);
	//groundModel[2]->SetTranslation(0.0f, -90.0f, 0.0f);
	//if (!result) return false;

	m_backgroundModel = new ModelClass;
	result = m_backgroundModel->Initialize(m_D3D->GetDevice(), 200, 112.5f);
	m_backgroundModel->SetTranslation(0.0f, 0.0f, 0.0f);

	if (!result) return false;

	TextureShaderGeneralClass* groundShader = new TextureShaderGeneralClass();
	groundShader->Initialize(m_D3D->GetDevice(), *m_hwnd, "Tile_1.dds");
	AddTextureShaderGeneral(groundShader);
	//for (int i = 0; i < GROUND_MODEL_LENGTH; i++)
	//	groundShader->AddModel(groundModel[i]);

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

	m_bombShader = new TextureShaderGeneralClass();
	m_bombShader->Initialize(m_D3D->GetDevice(), *m_hwnd, "DarkSphere_Core.dds", "darksphere.vs", "darksphere.ps");
	m_bombShader->SetAsTransparent(true);
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

	//if (groundModel)
	//{
	//	for (int i = 0; i < GROUND_MODEL_LENGTH; i++)
	//	{
	//		groundModel[i]->Shutdown();
	//		delete groundModel[i];
	//		groundModel[i] = 0;
	//	}		
	//}

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
	m_playerPosDiff = playerModel->GetTranslation() - m_lastPlayerPosition;
	m_lastPlayerPosition = playerModel->GetTranslation();
	m_Camera->SetPosition(m_Camera->GetPosition().x + m_playerPosDiff.x, m_Camera->GetPosition().y + m_playerPosDiff.y, m_Camera->GetPosition().z);
	m_backgroundModel->SetTranslation(m_backgroundModel->GetTranslation().x + m_playerPosDiff.x, m_backgroundModel->GetTranslation().y + m_playerPosDiff.y, m_backgroundModel->GetTranslation().z);

	if (m_backgrounds.size() == 7)
	{
		//background_sky
		ModelClass* model = m_backgrounds.at(0)->GetModels().at(0);
		model->SetTranslation(model->GetTranslation().x + m_playerPosDiff.x * 0.96f, model->GetTranslation().y + m_playerPosDiff.y, model->GetTranslation().z);
		model = m_backgrounds.at(1)->GetModels().at(0);
		model->SetTranslation(model->GetTranslation().x + m_playerPosDiff.x * 0.96f, model->GetTranslation().y + m_playerPosDiff.y, model->GetTranslation().z);
		//background_mountains
		model = m_backgrounds.at(2)->GetModels().at(0);
		model->SetTranslation(model->GetTranslation().x + m_playerPosDiff.x * 0.9f, model->GetTranslation().y + m_playerPosDiff.y, model->GetTranslation().z);
		model = m_backgrounds.at(3)->GetModels().at(0);
		model->SetTranslation(model->GetTranslation().x + m_playerPosDiff.x * 0.9f, model->GetTranslation().y + m_playerPosDiff.y, model->GetTranslation().z);
		//background_sand
		model = m_backgrounds.at(4)->GetModels().at(0);
		model->SetTranslation(model->GetTranslation().x + m_playerPosDiff.x * 0.85f, model->GetTranslation().y + m_playerPosDiff.y, model->GetTranslation().z);
		model = m_backgrounds.at(5)->GetModels().at(0);
		model->SetTranslation(model->GetTranslation().x + m_playerPosDiff.x * 0.85f, model->GetTranslation().y + m_playerPosDiff.y, model->GetTranslation().z);
		model = m_backgrounds.at(6)->GetModels().at(0);
		model->SetTranslation(model->GetTranslation().x + m_playerPosDiff.x * 0.85f, model->GetTranslation().y + m_playerPosDiff.y, model->GetTranslation().z);
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

//ModelClass *GraphicsClass::GetGroundModel(int index)
//{
//	return groundModel[index];
//}

int GraphicsClass::GetGroundModelCount()
{
	return m_groundModels.size();
}

D3DXVECTOR3 GraphicsClass::GetPlayerPositionDiff()
{
	return m_playerPosDiff;
}

D3DXVECTOR3 GraphicsClass::GetPlayerPosition()
{
	return m_lastPlayerPosition;
}

ModelClass * GraphicsClass::GetPlayerModel()
{
	return playerModel;
}

ModelClass * GraphicsClass::GetMainMenuBackground()
{
	return m_backgroundModel;
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

void GraphicsClass::ClearBombs()
{
	for (int i = 0; i < m_bombModels.size(); i++)
	{
		if (m_bombModels.at(i))
		{
			m_bombModels.at(i)->Shutdown();
			delete m_bombModels.at(i);
		}
	}
	m_bombModels.clear();
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

bool GraphicsClass::AddBackgroundShader(TextureShaderGeneralClass * textureShader)
{
	if (textureShader == nullptr)
		return false;

	m_backgrounds.push_back(textureShader);
	return true;
}

void GraphicsClass::RemoveBackgroundShader(TextureShaderGeneralClass * textureShader)
{
	m_backgrounds.erase(std::remove(m_backgrounds.begin(), m_backgrounds.end(), textureShader));
}

TextureShaderGeneralClass * GraphicsClass::GetBackgroundShader(int index)
{
	if (m_backgrounds.size() <= index)
		return nullptr;

	return m_backgrounds.at(index);
}

void GraphicsClass::ClearBackgrounds()
{
	m_backgrounds.clear();
}

ModelClass* GraphicsClass::AddGroundModel(int width, int height, float posX, float posY)
{
	m_groundModels.push_back(new ModelClass());
	m_groundModels.at(m_groundModels.size() - 1)->Initialize(m_D3D->GetDevice(), width, height);
	m_groundModels.at(m_groundModels.size() - 1)->SetTranslation(posX, posY, 0.0f);

	return m_groundModels.at(m_groundModels.size() - 1);
}

ModelClass * GraphicsClass::GetGroundModel(int index)
{
	return m_groundModels.at(index);
}

void GraphicsClass::ClearGrounds()
{
	m_groundModels.clear();
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
	float cameraPosLeftX = m_Camera->GetPosition().x - 175.0f;
	float cameraPosRightX = m_Camera->GetPosition().x + 175.0f;
	float xPos = 0.0f;

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

	for (int i = 0; i < m_backgrounds.size(); i++)
	{
		int b = i;

		if (m_backgrounds.at(i) == nullptr)
			continue;

		ModelClass* model = nullptr;

		if (m_backgrounds.at(i)->IsTransparent())
			m_D3D->TurnOnAlphaBlending();

		for (int k = 0; k < m_backgrounds.at(i)->GetModels().size(); k++)
		{
			if ((model = m_backgrounds.at(i)->GetModels().at(k)) == nullptr)
				continue;

			m_D3D->GetWorldMatrix(worldMatrix);
			if (m_backgrounds.at(i)->GetIsConstantOnScreen())
			{
				D3DXVECTOR3 posDiff = m_Camera->GetPosition() - m_lastFrameCameraPosition;
				model->SetAdditionalTranslation(model->GetAdditionalTranslation().x + posDiff.x, model->GetAdditionalTranslation().y + posDiff.y, model->GetAdditionalTranslation().z);
			}

			D3DXMATRIX scaleMatrix;
			D3DXMatrixScaling(&scaleMatrix, model->GetScale().x, model->GetScale().y, 1.0f);
			D3DXMatrixTranslation(&worldMatrix, model->GetTranslation().x + model->GetAdditionalTranslation().x, model->GetTranslation().y + model->GetAdditionalTranslation().y,
				model->GetTranslation().z + model->GetAdditionalTranslation().z);
			D3DXMatrixMultiply(&worldMatrix, &scaleMatrix, &worldMatrix);
			if (!model->Render(m_D3D->GetDeviceContext()))
				continue;

			if (!m_backgrounds.at(i)->Render(m_D3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->movingRight))
				return false;
		}

		m_D3D->TurnOffAlphaBlending();
	}

	m_D3D->TurnOnAlphaBlending();
	bool useSourceAlpha = false;
	for (int i = 1; i < m_TextureShaders.size(); i++)
	{
		int b = i;

		if (m_TextureShaders.at(i) == nullptr)
			continue;

		ModelClass* model = nullptr;

		if (m_TextureShaders.at(i)->GetUseAlphaSourceBlending() && !useSourceAlpha)
		{
			m_D3D->TurnOnAlphaBlendingUseAlpha();
			useSourceAlpha = true;
		}
		else if (!m_TextureShaders.at(i)->GetUseAlphaSourceBlending() && useSourceAlpha)
		{
			m_D3D->TurnOnAlphaBlending();
			useSourceAlpha = false;
		}

		for (int k = 0; k < m_TextureShaders.at(i)->GetModels().size(); k++)
		{
			if ((model = m_TextureShaders.at(i)->GetModels().at(k)) == nullptr)
				continue;

			xPos = model->GetTranslation().x;

			if (xPos < cameraPosLeftX|| xPos > cameraPosRightX)
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

	m_D3D->TurnOnAlphaBlending();
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.	
	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, playerModel->GetTranslation().x, playerModel->GetTranslation().y, playerModel->GetTranslation().z);
	if (playerModel->Render(m_D3D->GetDeviceContext()))
	{
		// Render the model using the color shader.	
		result = m_TextureShaders.at(0)->Render(m_D3D->GetDeviceContext(), playerModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, playerModel->movingRight);
		if (!result)
		{
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

			if (m_TextureShadersGeneral.at(i)->GetIsConstantOnScreen())
			{
				D3DXVECTOR3 posDiff = m_Camera->GetPosition() - m_lastFrameCameraPosition;
				model->SetAdditionalTranslation(model->GetAdditionalTranslation().x + posDiff.x, model->GetAdditionalTranslation().y + posDiff.y, model->GetAdditionalTranslation().z);
			}
			else
			{
				xPos = model->GetTranslation().x;
				if (xPos < cameraPosLeftX || xPos > cameraPosRightX)
				{
					model->isInCamera = false;
					continue;
				}
				model->isInCamera = true;
			}

			m_D3D->GetWorldMatrix(worldMatrix);
			D3DXMATRIX scaleMatrix;
			D3DXMatrixScaling(&scaleMatrix, model->GetScale().x, model->GetScale().y, 1.0f);
			D3DXMatrixTranslation(&worldMatrix, model->GetTranslation().x + model->GetAdditionalTranslation().x, model->GetTranslation().y + model->GetAdditionalTranslation().y,
				model->GetTranslation().z + model->GetAdditionalTranslation().z);
			D3DXMatrixMultiply(&worldMatrix, &scaleMatrix, &worldMatrix);
			if (!model || !m_D3D || !m_D3D->GetDeviceContext() || !model->Render(m_D3D->GetDeviceContext()))
				continue;

			if (!m_TextureShadersGeneral.at(i)->Render(m_D3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->movingRight))
				return false;
		}

		m_D3D->TurnOffAlphaBlending();
	}

	//for (int i = 0; i < GROUND_MODEL_LENGTH; i++)
	//{		
	//	m_ColorShader->SetColor(D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));
	//	m_D3D->GetWorldMatrix(worldMatrix);
	//	D3DXMatrixTranslation(&worldMatrix, groundModel[i]->GetTranslation().x, groundModel[i]->GetTranslation().y, groundModel[i]->GetTranslation().z);
	//	groundModel[i]->Render(m_D3D->GetDeviceContext());
	//	
	//	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), groundModel[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	//	if (!result)
	//	{
	//		return false;
	//	}
	//}

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

	m_D3D->TurnOnAlphaBlending();

	for (int i = 0; i < m_bombModels.size(); i++)
	{
		m_ColorShader->SetColor(D3DXVECTOR4(1.0f, 0.5f, 0.3f, 1.0f));
		m_D3D->GetWorldMatrix(worldMatrix);
		D3DXMATRIX scaleMatrix;
		D3DXMatrixScaling(&scaleMatrix, m_bombModels[i]->GetScale().x, m_bombModels[i]->GetScale().y, 1.0f);
		D3DXMatrixTranslation(&worldMatrix, m_bombModels[i]->GetTranslation().x, m_bombModels[i]->GetTranslation().y, m_bombModels[i]->GetTranslation().z);
		D3DXMatrixMultiply(&worldMatrix, &scaleMatrix, &worldMatrix);

		m_bombModels[i]->Render(m_D3D->GetDeviceContext());

		result = m_bombShader->Render(m_D3D->GetDeviceContext(), m_bombModels[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}
	}
	m_D3D->TurnOffAlphaBlending();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	m_D3D->TurnZBufferOn();

	return true;
}