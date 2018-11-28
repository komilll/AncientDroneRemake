#pragma once

#ifndef _UI_CONTROLLER_H_
#define _UI_CONTROLLER_H_

#include "modelclass.h"
#include "textureshadergeneralclass.h"
#include "graphicsclass.h"
#include "mouseclass.h"
#include <functional>

class UIController 
{
public:
	UIController();

	ModelClass* GetModel();
	TextureShaderGeneralClass* GetShader();
	void Init(GraphicsClass* graphicsClass, float width, float height, float originalWidth, float originalHeight, float positionX, float positionY, CHAR* filename, bool transparent = true);
	void Update();
	
	void SetProgress(float progress);
	void InitializeButton(MouseClass* mouseClass, float widthArea = 1, float heightArea = 1); //Percentage of image area that will be treated as button
	std::function<void()> EventOnPressButton;

private:
	ModelClass* m_model;
	TextureShaderGeneralClass* m_shader;
	float m_currentProgress = 1.0f;
	float m_originalTranslationX;

	//Button
	bool m_button = false;
	bool m_buttonPressed = false;
	float m_widthButton = -1;
	float m_heightButton = -1;
	MouseClass* m_mouse;
};


#endif // !_UI_CONTROLLER_H_