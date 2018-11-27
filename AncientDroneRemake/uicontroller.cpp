#include "uicontroller.h"

UIController::UIController()
{
	m_model = 0;
	m_shader = 0;
}

void UIController::Init(GraphicsClass* graphicsClass, float width, float height, float originalWidth, float originalHeight, float positionX, float positionY, CHAR* filename, bool transparent)
{
	m_model = new ModelClass();
	if (!m_model)
		return;

	if (!m_model->Initialize(graphicsClass->GetD3D()->GetDevice(), width, height))
		return;

	m_model->SetTranslation(positionX, positionY, 0.0f);

	m_shader = new TextureShaderGeneralClass();
	if (!m_shader)
		return;

	if (!m_shader->Initialize(graphicsClass->GetD3D()->GetDevice(), *graphicsClass->GetHWND(), filename))
		return;

	graphicsClass->AddTextureShaderGeneral(m_shader);
	m_shader->AddModel(m_model);
	m_shader->SetAsTransparent(transparent);
	m_shader->SetAsConstantOnScreen();

	m_originalTranslationX = positionX;
}

void UIController::Update()
{
	if (m_mouse == nullptr)
		return;

	if (m_mouse->GetMouseModelLocation().x >= m_model->GetTranslation().x - m_widthButton &&
		m_mouse->GetMouseModelLocation().x <= m_model->GetTranslation().x + m_widthButton &&
		m_mouse->GetMouseModelLocation().y >= m_model->GetTranslation().y - m_heightButton &&
		m_mouse->GetMouseModelLocation().y <= m_model->GetTranslation().y + m_heightButton &&
		m_mouse->GetLMBPressed())
	{
		PostQuitMessage(0);
	}
}

ModelClass * UIController::GetModel()
{
	return m_model;
}

TextureShaderGeneralClass * UIController::GetShader()
{
	return m_shader;
}

void UIController::SetProgress(float progress)
{
	if (progress == m_currentProgress)
		return;

	m_model->SetScale(progress, 1.0f, 1.0f);	
	m_model->SetTranslation(m_originalTranslationX - (1.0f - progress) * m_model->GetOriginalSize().x,
		m_model->GetTranslation().y, m_model->GetTranslation().z);
	m_currentProgress = progress;
}

void UIController::InitializeButton(MouseClass* mouseClass, float widthArea, float heightArea)
{
	m_mouse = mouseClass;
	m_button = true;
	m_buttonPressed = false;
	m_widthButton = m_model->GetSize().x * widthArea;
	m_heightButton = m_model->GetSize().y * heightArea;
}
