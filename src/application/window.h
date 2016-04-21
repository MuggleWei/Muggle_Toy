#ifndef __MUGGLE_WINDOW_H__
#define __MUGGLE_WINDOW_H__

#include "base/base.h"
#if MG_PLATFORM_WINDOWS
#include <windows.h>
#endif

NS_MUGGLE_BEGIN

#if MG_PLATFORM_WINDOWS

struct WindowInfo
{
	HWND hWnd;
	HINSTANCE hInstance;
	WCHAR app_name[512];	
	int width;
	int height;
	bool full_screen;
};

#else
#endif

class Application;

class MG_DLL Window
{
public:
	Window();
	~Window();

	Window(const Window&) = delete;
	Window(const Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(const Window&&) = delete;

	bool Initialize(Application* attached_app);
	void Destroy();

	WindowInfo& getWinInfo()
	{
		return m_window_info;
	}

#if MG_PLATFORM_WINDOWS
	LRESULT MessageLoop(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

protected:
	Application *m_attached_app;
	WindowInfo m_window_info;
};

NS_MUGGLE_END

#endif