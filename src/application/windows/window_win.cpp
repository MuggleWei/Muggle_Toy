#include "window.h"
#include <windows.h>
#include "../application.h"
#include "../input.h"

NS_MUGGLE_BEGIN

Window* g_win = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
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
		default:
		{
			return g_win->MessageLoop(hWnd, uMsg, wParam, lParam);
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window()
{
	m_attached_app = nullptr;

	m_window_info.width = 0;
	m_window_info.height = 0;
	m_window_info.full_screen = false;
}
Window::~Window()
{
	MASSERT_MSG(
		m_attached_app == nullptr &&
		m_window_info.hWnd == nullptr &&
		m_window_info.hInstance == nullptr,
		"Maybe forgot destroy window object!"
	);
}

bool Window::Initialize(Application* attached_app)
{
	MLOG("## Initialize window\n");
	g_win = this;
	m_attached_app = attached_app;

	MultiByteToWideChar(CP_UTF8, 0, Application::GetSingleton()->getAppName(), -1, m_window_info.app_name, MG_MAX_PATH);
	m_window_info.hInstance = GetModuleHandle(NULL);

	// Setup the windows class with default settings.
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_window_info.hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_window_info.app_name;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	int posX, posY;
	m_window_info.width = GetSystemMetrics(SM_CXSCREEN);
	m_window_info.height = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (m_window_info.full_screen)
	{
		DEVMODE dmScreenSettings;

		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_window_info.width;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_window_info.height;
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
		m_window_info.width = 800;
		m_window_info.height = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - m_window_info.width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_window_info.height) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_window_info.hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_window_info.app_name, m_window_info.app_name,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, m_window_info.width, m_window_info.height, NULL, NULL, m_window_info.hInstance, NULL);

	// Bring the window up on the screen and focus it
	ShowWindow(m_window_info.hWnd, SW_SHOW);
	SetForegroundWindow(m_window_info.hWnd);
	SetFocus(m_window_info.hWnd);

	// Hide the mouse cursor.
	ShowCursor(true);

	return true;
}
void Window::Destroy()
{
	MLOG("## Destroy window\n");

	MASSERT_MSG(m_window_info.hWnd != nullptr && m_window_info.hInstance != nullptr, "Repeatedly destroy window");

	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (m_window_info.full_screen)
	{
		ChangeDisplaySettings(nullptr, 0);
	}

	// Remove the window.
	::DestroyWindow(m_window_info.hWnd);
	m_window_info.hWnd = nullptr;

	// Remove the application instance.
	UnregisterClass(m_window_info.app_name, m_window_info.hInstance);
	m_window_info.hInstance = nullptr;

	m_attached_app = nullptr;
	g_win = nullptr;
}

LRESULT Window::MessageLoop(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_KEYDOWN:
		{
			Input::SetKeyCode_Win((unsigned int)wParam, eButtonStatus::Press);
		}break;
		case WM_KEYUP:
		{
			Input::SetKeyCode_Win((unsigned int)wParam, eButtonStatus::Release);
		}break;
		case WM_MOUSEMOVE:
		{
			Input::SetMousePos(vec2i(LOWORD(lParam), HIWORD(lParam)));
		}break;
		case WM_MOUSEWHEEL:
		{
			Input::SetMouseWheel((int16_t)HIWORD(wParam));
		}break;
		case WM_LBUTTONDOWN:
		{
			Input::SetKeyCode_Win(VK_LBUTTON, eButtonStatus::Press);
		}break;
		case WM_MBUTTONDOWN:
		{
			Input::SetKeyCode_Win(VK_MBUTTON, eButtonStatus::Press);
		}break;
		case WM_RBUTTONDOWN:
		{
			Input::SetKeyCode_Win(VK_RBUTTON, eButtonStatus::Press);
		}break;
		case WM_LBUTTONUP:
		{
			Input::SetKeyCode_Win(VK_LBUTTON, eButtonStatus::Release);
		}break;
		case WM_MBUTTONUP:
		{
			Input::SetKeyCode_Win(VK_MBUTTON, eButtonStatus::Release);
		}break;
		case WM_RBUTTONUP:
		{
			Input::SetKeyCode_Win(VK_RBUTTON, eButtonStatus::Release);
		}break;
		default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	return 0;
}

NS_MUGGLE_END