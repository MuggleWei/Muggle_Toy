#includes "window.h"

NS_MUGGLE_BEGIN

Window* g_win = nullptr;

Window::Window()
{
	m_attached_app = nullptr;

	m_window_info.width = 0;
	m_window_info.height = 0;
	m_window_info.full_screen = false;
}
Window::~Window()
{}

bool Window::~Initialize(Application* attached_app)
{
	// TODO: 
	return false;
}
void Window::~Destroy()
{}


NS_MUGGLE_END