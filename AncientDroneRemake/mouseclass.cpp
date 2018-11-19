#include "mouseclass.h"

MouseClass::MouseClass()
{
	m_directInput = 0;
	m_mouse = 0;
}

MouseClass::MouseClass(const MouseClass &)
{
}

MouseClass::~MouseClass()
{
}

bool MouseClass::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result;

	m_screenWidth = 800;
	m_screenHeight = 600;

	m_mouseX = 0;
	m_mouseY = 0;

	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void MouseClass::Shutdown()
{
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	if (m_mouse)
	{
		m_mouse->Release();
		m_mouse = 0;
	}
}

bool MouseClass::Frame()
{
	bool result;

	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	ProcessInput();

	return true;
}

bool MouseClass::ReadMouse()
{
	HRESULT result;

	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void MouseClass::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	if (m_mouseX < 0)
		m_mouseX = 0;
	if (m_mouseY < 0)
		m_mouseY = 0;

	if (m_mouseX > m_screenWidth)
		m_mouseX = m_screenWidth;
	if (m_mouseY > m_screenHeight)
		m_mouseY = m_screenHeight;
}

void MouseClass::GetMouseLocation(int & mouseX, int & mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
}

bool MouseClass::SetMouseLocation(int mouseX, int mouseY)
{
	m_mouseState.lX += mouseX;
	m_mouseState.lY += mouseY;

	ProcessInput();

	return true;
}

bool MouseClass::GetLMBPressed()
{
	return m_mouseState.rgbButtons[0];
}

void MouseClass::SetLMBPressed(bool enable)
{
	m_mouseState.rgbButtons[0] = enable;
}
