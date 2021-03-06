////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
	m_GameManager = 0;
	m_self = this;
}


SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}


bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	m_Input->Initialize();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	m_GameManager = new GameManager;
	m_Mouse = new MouseClass;

	m_tiledInterpreter = new TiledInterpreter;
	if (!m_tiledInterpreter)
		return false;

	if (!m_GameManager)	
		return false;	
	if (!m_GameManager->Initialize(GetInputController(), m_Mouse, m_Graphics->GetD3D(), m_Graphics))
		return false;

	m_tiledInterpreter->Initialize(m_Graphics, m_GameManager->GetPlayer(), m_GameManager);
	//if (m_GameManager->GetMenuStartGame() == nullptr)
	//	m_GameManager->SetMenuStartGame();
	m_GameManager->StartGameBuildLevel = [=]()-> void{ m_tiledInterpreter->Import(); };
	//m_GameManager->GetMenuStartGame()->EventOnPressButton = [=]() -> void { };

	if (!m_GameManager->SpawnObjects())
		return false;

	if (!m_Mouse) 	
		return false;
	if (!m_Mouse->Initialize(m_hinstance, m_hwnd, m_Graphics))
		return false;

	return true;
}


void SystemClass::Shutdown()
{
	// Release the graphics object.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	if (m_tiledInterpreter)
	{
		delete m_tiledInterpreter;
		m_tiledInterpreter = 0;
	}

	// Shutdown the window.
	ShutdownWindows();

	return;
}


void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}

	return;
}

InputClass * SystemClass::GetInputController()
{
	return m_Input;
}


bool SystemClass::Frame()
{
	bool result;

	m_Mouse->SetLMBPressed(false);
	m_Mouse->SetRMBPressed(false);
	m_Mouse->Frame();
	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	if (m_GameManager)
	{
		m_GameManager->Update();
	}

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	if (m_Mouse->GetLMBPressed())
	{
		m_GameManager->LMBPressed();
	}
	else if (m_Mouse->GetRMBPressed())
	{
		m_GameManager->RMBPressed();
	}
	m_GameManager->SetDroneRotation(m_Mouse->GetMouseModelLocation().x, m_Mouse->GetMouseModelLocation().y);
	
	if (m_Input->IsKeyDown(VK_P) || m_GameManager->GetPlayer()->DoRestartGame())
	{
		m_tiledInterpreter->RestartLevel();
		m_GameManager->RestartLevel();
	}
	else if (m_Input->IsKeyDown(VK_E))
		m_GameManager->SetDroneDestination(m_Mouse->GetMouseModelLocation().x, m_Mouse->GetMouseModelLocation().y);
	else if (m_Input->IsKeyDown(VK_R))
		m_GameManager->CallDroneToPlayer();

	if (m_GameManager->CheckNextLevel())
	{
		m_tiledInterpreter->LoadNextLevel();
	}

	if (m_tiledInterpreter)
	{
		m_tiledInterpreter->Update();
	}

	if (m_currentCheatLetter == 0)
	{
		if (m_Input->IsKeyDown(VK_T))
		{
			m_currentCheatLetter = 1;
			m_timeToTypeCheats = 10000;
		}
	}
	else if (m_currentCheatLetter == 1)
	{
		if (m_Input->IsKeyDown(VK_A))
			m_currentCheatLetter = 2;
	}
	else if (m_currentCheatLetter == 2)
	{
		if (m_Input->IsKeyDown(VK_N))
			m_currentCheatLetter = 3;
	}
	else if (m_currentCheatLetter == 3)
	{
		if (m_Input->IsKeyDown(VK_U))
			m_currentCheatLetter = 4;
	}
	else if (m_currentCheatLetter == 4)
	{
		if (m_Input->IsKeyDown(VK_K))
			m_currentCheatLetter = 5;
	}
	else if (m_currentCheatLetter == 5)
	{
		if (m_Input->IsKeyDown(VK_I))
		{
			UseGodMode();
			m_currentCheatLetter = 6;
		}
	}

	m_timeToTypeCheats -= (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - m_lastTime);
	m_lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if (m_timeToTypeCheats <= 0)
		m_currentCheatLetter = 0;

	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = "Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}


void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

void SystemClass::UseGodMode()
{
	m_GameManager->UseGodMode();
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}