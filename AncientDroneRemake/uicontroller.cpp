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
}