#include "mouseclass.h"

MouseClass::MouseClass()
{
	m_directInput = 0;
	m_mouse = 0;
	m_mouseMaxX = 141;
	m_mouseMaxY = 106;
}

MouseClass::MouseClass(const MouseClass &)
{
}

MouseClass::~MouseClass()
{
}

bool MouseClass::Initialize(HINSTANCE hInstance, HWND hwnd, GraphicsClass* graphicsClass)
{
	HRESULT result;
	m_graphics = graphicsClass;

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

	if (graphicsClass == nullptr)
		return false;

	m_model = new ModelClass();
	if (!m_model)
		return false;

	m_shader = new TextureShaderGeneralClass();
	if (!m_shader)
		return false;

	if (!m_shader->Initialize(graphicsClass->GetD3D()->GetDevice(), hwnd, "crosshair.dds"))
		return false;

	if (!graphicsClass->AddTextureShaderGeneral(m_shader))
	{
		m_shader->Shutdown();
		delete m_shader;
		m_shader = 0;
	}
	
	m_model->Initialize(graphicsClass->GetD3D()->GetDevice(), 5.0f, 5.0f);
	m_model->SetTranslation(0.0f, 0.0f, 0.0f);
	m_shader->AddModel(m_model);
	m_shader->SetAsTransparent(true);

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
	//if (m_mouseState.lX / m_mouseSpeedSlowdown > 0.0f && m_mouseState.lX / m_mouseSpeedSlowdown < 1.0f)
	//	m_mouseX += 1;
	//else if (m_mouseState.lX / m_mouseSpeedSlowdown < 0.0f && m_mouseState.lX / m_mouseSpeedSlowdown > -1.0f)
	//	m_mouseX -= 1;
	//else

	float toAddX = (m_mouseState.lX / m_mouseSpeedSlowdown);
	float toAddY = (m_mouseState.lY / m_mouseSpeedSlowdown);

	if (toAddX > 0)
		toAddX = ceil(toAddX);
	else
		toAddX = floor(toAddX);

	if (toAddY > 0)
		toAddY = ceil(toAddY);
	else
		toAddY = floor(toAddY);

		m_mouseX += toAddX;

	//if (m_mouseState.lY / m_mouseSpeedSlowdown > 0.0f && m_mouseState.lY / m_mouseSpeedSlowdown < 1.0f)
	//	m_mouseY -= 1;
	//else if (m_mouseState.lY / m_mouseSpeedSlowdown < 0.0f && m_mouseState.lY / m_mouseSpeedSlowdown > -1.0f)
	//	m_mouseY += 1;
	//else
		m_mouseY -= toAddY;

		m_mouseX += m_graphics->GetPlayerPositionDiff().x;
		m_mouseY += m_graphics->GetPlayerPositionDiff().y;

	if (m_mouseX < -m_mouseMaxX + m_graphics->GetPlayerPosition().x)
		m_mouseX = -m_mouseMaxX + m_graphics->GetPlayerPosition().x;
	if (m_mouseY < -m_mouseMaxY + m_graphics->GetPlayerPosition().y)
		m_mouseY = -m_mouseMaxY + m_graphics->GetPlayerPosition().y;

	if (m_mouseX > m_mouseMaxX + m_graphics->GetPlayerPosition().x)
		m_mouseX = m_mouseMaxX + m_graphics->GetPlayerPosition().x;
	if (m_mouseY > m_mouseMaxY + m_graphics->GetPlayerPosition().y)
		m_mouseY = m_mouseMaxY + m_graphics->GetPlayerPosition().y;

	m_model->SetTranslation(m_mouseX, m_mouseY, 0.0f);
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

bool MouseClass::GetRMBPressed()
{
	return m_mouseState.rgbButtons[1];
}

void MouseClass::SetRMBPressed(bool enable)
{
	m_mouseState.rgbButtons[1] = enable;
}

void MouseClass::SetLMBPressed(bool enable)
{
	m_mouseState.rgbButtons[0] = enable;
}

D3DXVECTOR2 MouseClass::GetMouseModelLocation()
{
	float mouseMidX = m_model->GetBounds().min.x + m_model->GetAdditionalTranslation().x + (m_model->GetBounds().max.x - m_model->GetBounds().min.x) / 2.0f;
	float mouseMidY = m_model->GetBounds().min.y + m_model->GetAdditionalTranslation().y + (m_model->GetBounds().max.y - m_model->GetBounds().min.y) / 2.0f;

	return D3DXVECTOR2(mouseMidX, mouseMidY);
}
