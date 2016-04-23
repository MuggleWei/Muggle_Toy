#include "renderer_opengl.h"
#include "glad/glad.h"

#if MG_PLATFORM_WINDOWS
#pragma comment(lib, "Opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif

NS_MUGGLE_BEGIN

int MG_IMG_FMT_SIZE[ImageFormat::Enum::Max] =
{
	0,
	32,
	64
};

Renderer_OpenGL::Renderer_OpenGL()
{
	m_render_type = RenderType::Enum::OpenGL;
}
Renderer_OpenGL::~Renderer_OpenGL()
{}

bool Renderer_OpenGL::Initialize(const RenderInitParameter& init_param)
{
	MLOG("## Initialize OpenGL\n");

	// initialize parent member variable
	Renderer::Initialize(init_param);

	// create OpenGL context
#if MG_PLATFORM_WINDOWS
	bool result = Initialize_Windows(init_param);
	if (!result)
	{
		MASSERT_MSG(0, "Failed initialize opengl context");
		return false;
	}
#else
#endif

	// initialize glad
	if (!gladLoadGL()) 
	{
		MASSERT_MSG(0, "Failed initialize glad");
		return false;
	}

	// retrieve hardware and opengl information
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	MLOG("## GL Vendor : %s\n", vendor);
	MLOG("## GL Renderer : %s\n", renderer);
	MLOG("## GL Version (string) : %s\n", version);
	MLOG("## GL Version (integer) : %d.%d\n", major, minor);
	MLOG("## GLSL Version : %s\n", glslVersion);

	return true;
}
void Renderer_OpenGL::Destroy()
{
	MLOG("## Destroy OpenGL\n");

#if MG_PLATFORM_WINDOWS
	Destroy_Windows();
#endif
}
void Renderer_OpenGL::BeginScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void Renderer_OpenGL::EndScene()
{
	SwapBuffers((HDC)m_hdc);
}

void Renderer_OpenGL::setClearColor(const vec4f& color)
{
	Renderer::setClearColor(color);
	glClearColor(color.x, color.y, color.z, color.w);
}

#if MG_PLATFORM_WINDOWS
bool Renderer_OpenGL::Initialize_Windows(const RenderInitParameter& init_param)
{
	if (m_hWnd == nullptr)
	{
		MASSERT_MSG(0, "Need a hwnd to initialize openGL");
		return false;
	}

	// fill out PIXELFORMATDESCRIPTOR
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = MG_IMG_FMT_SIZE[init_param.rt_format];
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	// choose pixel format
	m_hdc = (void*)GetDC((HWND)m_hWnd);
	int n_pixel_format = ChoosePixelFormat((HDC)m_hdc, &pfd);
	if (n_pixel_format == 0)
	{
		MASSERT_MSG(0, "Can not match an appropriate pixel format supported by a device context");
		return false;
	}

	// set pixel format
	BOOL ret = SetPixelFormat((HDC)m_hdc, n_pixel_format, &pfd);
	if (!ret)
	{
		MASSERT_MSG(0, "Failed in sets the pixel format of the specified device context");
		return false;
	}

	// create and set context
	m_hglrc = (void*)wglCreateContext((HDC)m_hdc);
	if (m_hglrc == nullptr)
	{
		MASSERT_MSG(0, "Failed create context");
		return false;
	}
	wglMakeCurrent((HDC)m_hdc, (HGLRC)m_hglrc);

	return true;
}
void Renderer_OpenGL::Destroy_Windows()
{
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext((HGLRC)m_hglrc);
}
#else
#endif

NS_MUGGLE_END