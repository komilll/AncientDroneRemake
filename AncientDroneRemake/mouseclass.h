#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include "graphicsclass.h"
#include "textureshadergeneralclass.h"
#include "modelclass.h"
#include "IEanimationobject.h"

class MouseClass 
{
public:
	MouseClass();
	MouseClass(const MouseClass&);
	~MouseClass();

public:
	bool Initialize(HINSTANCE hInstance, HWND hwnd, GraphicsClass *graphicsClass);
	void Shutdown();
	bool Frame();
	bool ReadMouse();
	void ProcessInput();
	void GetMouseLocation(int &mouseX, int &mouseY);
	bool SetMouseLocation(int mouseX, int mouseY);
	bool GetLMBPressed();
	void SetLMBPressed(bool enable);

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_mouse;

	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
	float m_mouseSpeedSlowdown = 4.0f;

	//Graphics object settings
	ModelClass* m_model;
	TextureShaderGeneralClass* m_shader;
};