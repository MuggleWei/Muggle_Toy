#include "renderer_opengl.h"
#include "glad/glad.h"
#include "base/file_utility.h"
#include "shader_program.h"

#if MG_PLATFORM_WINDOWS
#include "GL/wglext.h"
#pragma comment(lib, "Opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif

NS_MUGGLE_BEGIN

void APIENTRY GLDebugCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam)
{
	// Uncomment this to print opengl debug callback message
	// MLOG("%s\n", message);
}

const int MG_IMG_FMT_SIZE[ImageFormat::Enum::Max] =
{
	0,
	32,
	64
};
const int MG_IMG_FMT_TO_WGL_FMT[ImageFormat::Enum::Max] =
{
	0,
	WGL_TYPE_RGBA_ARB,
	WGL_TYPE_RGBA_FLOAT_ARB,
};

Renderer_OpenGL::Renderer_OpenGL()
{
	m_render_type = RenderType::Enum::OpenGL;

#if MG_PLATFORM_WINDOWS
	m_hdc = nullptr;
	m_hglrc = nullptr;
	wglSwapIntervalEXT = nullptr;
	wglGetExtensionsStringEXT = nullptr;
#endif
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

#if MUGGLE_DEBUG
	// if gl version >= 4.3, enable debug callback
	GLint gl_version = major * 100 + minor;
	if (gl_version >= 403)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugCallback, nullptr);
		glDebugMessageControl(
			GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true
		);
	}
#endif

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

void Renderer_OpenGL::setShader(const ShaderProgram* shader_program)
{
	MASSERT(shader_program->gl_handle != 0);
	glUseProgram(shader_program->gl_handle);
}

bool Renderer_OpenGL::ExtensionSupported(const char* extension_name)
{
	// get extensions
	if (wglGetExtensionsStringEXT)
	{
		// if create proper context, use wglGetExtensionsStringEXT
		const char* extensions = ((PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetExtensionsStringEXT)();
		if (extensions == nullptr)
		{
			MASSERT_MSG(0, "Failed get wgl extensions");
			return false;
		}

		if (strstr((const char*)extensions, extension_name) == nullptr)
		{
			return false;
		}
	}
	else
	{
		// if create simple context only, use glGetString(GL_EXTENSIONS)
		const GLubyte* extensions = glGetString(GL_EXTENSIONS);
		if (extensions == nullptr)
		{
			MASSERT_MSG(0, "Failed get GL_EXTENSIONS");
			return false;
		}

		if (strstr((const char*)extensions, extension_name) == nullptr)
		{
			return false;
		}
	}	

	return true;
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

	// create simple context
	CreateSimpleContext_Windows(init_param, &pfd);

	// initialize glad
	if (!gladLoadGL())
	{
		MASSERT_MSG(0, "Failed initialize glad");
		return false;
	}

	// create proper context
	if (!CreateProperContext_Windows(init_param, &pfd))
	{
		return false;
	}

	// verify swap interval support
	if (!ExtensionSupported("WGL_EXT_swap_control"))
	{
		MASSERT_MSG(0, "Can't find WGL_EXT_swap_control in GL Extension supported");
		return false;
	}
	wglSwapIntervalEXT = (void*)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT == nullptr)
	{
		MASSERT_MSG(0, "Don't support wglSwapIntervalEXT");
		return false;
	}

	// set vsync
	if (init_param.vsync)
	{
		((PFNWGLSWAPINTERVALEXTPROC)wglSwapIntervalEXT)(1);
	}
	else
	{
		((PFNWGLSWAPINTERVALEXTPROC)wglSwapIntervalEXT)(0);
	}

	return true;
}
void Renderer_OpenGL::Destroy_Windows()
{
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext((HGLRC)m_hglrc);
}
bool Renderer_OpenGL::CreateSimpleContext_Windows(const RenderInitParameter& init_param, void* p_pfd)
{
	// choose pixel format
	m_hdc = (void*)GetDC((HWND)m_hWnd);
	int n_pixel_format = ChoosePixelFormat((HDC)m_hdc, (PIXELFORMATDESCRIPTOR*)p_pfd);
	if (n_pixel_format == 0)
	{
		MASSERT_MSG(0, "Can not match an appropriate pixel format supported by a device context");
		return false;
	}

	// set pixel format
	BOOL ret = SetPixelFormat((HDC)m_hdc, n_pixel_format, (PIXELFORMATDESCRIPTOR*)p_pfd);
	if (!ret)
	{
		MASSERT_MSG(0, "Failed in sets the pixel format of the specified device context");
		return false;
	}

	// create and set simple context
	m_hglrc = (void*)wglCreateContext((HDC)m_hdc);
	if (m_hglrc == nullptr)
	{
		MASSERT_MSG(0, "Failed create simple context");
		return false;
	}
	wglMakeCurrent((HDC)m_hdc, (HGLRC)m_hglrc);

	return true;
}
bool Renderer_OpenGL::CreateProperContext_Windows(const RenderInitParameter& init_param, void* p_pfd)
{
	// choose pixel format
	int pixel_type = MG_IMG_FMT_TO_WGL_FMT[init_param.rt_format];
	int color_bits = MG_IMG_FMT_SIZE[init_param.rt_format];	

	const int attribList[] =
	{
		// WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		// Uncomment this to use sRGB
//		WGL_ARB_framebuffer_sRGB, ...,
		// Uncomment this to use multisample
//		WGL_ARB_multisample, ...,
		WGL_PIXEL_TYPE_ARB, pixel_type,
		WGL_COLOR_BITS_ARB, color_bits,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0 //End
	};

	int pixel_format;
	UINT num_formats;

	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if (wglChoosePixelFormatARB == nullptr)
	{
		MASSERT_MSG(0, "Failed get wglChoosePixelFormatARB");
		return false;
	}
	BOOL result = wglChoosePixelFormatARB((HDC)m_hdc, attribList, NULL, 1, &pixel_format, &num_formats);
	if (!result)
	{
		MASSERT_MSG(0, "Failed in wglChoosePixelFormatARB");
		return false;
	}
	if (num_formats == 0)
	{
		MASSERT_MSG(0, "Can't support this pixel format");
		return false;
	}

	SetPixelFormat((HDC)m_hdc, pixel_format, (PIXELFORMATDESCRIPTOR*)p_pfd);

	// determine the version that we will use
	MLOG("## OpenGL Version support max %d.%d loaded by glad in this computer\n", GLVersion.major, GLVersion.minor);
	if (init_param.version_major * 100 + init_param.version_minor >
		GLVersion.major * 100 + GLVersion.minor)
	{
		MASSERT_MSG(0, "Specified version is beyond the max version supported");
		return false;
	}
	int major = GLVersion.major, minor = GLVersion.minor;
	if (init_param.version_major != 0)
	{
		major = init_param.version_major;
		minor = init_param.version_minor;
	}

	GLint attribs[] =
	{
		// OpenGL Version
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,
		// Uncomment this for forward compatibility mode
		// WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		// Uncomment this for Compatibility profile
		// WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		// using Core profile here
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0 //End
	};

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (wglCreateContextAttribsARB == nullptr)
	{
		MASSERT_MSG(0, "Failed to get wglCreateContextAttribsARB");
		return false;
	}
	HGLRC hrc = wglCreateContextAttribsARB((HDC)m_hdc, 0, attribs);
	if (!(hrc && wglMakeCurrent((HDC)m_hdc, hrc)))
	{
		MASSERT_MSG(0, "Failed in create proper opengl context");
		return false;
	}

	// set hrc
	m_hglrc = hrc;

	// get wglGetExtensionsStringEXT
	wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)
		wglGetProcAddress("wglGetExtensionsStringEXT");
	if (wglGetExtensionsStringEXT == nullptr)
	{
		MASSERT_MSG(0, "Failed get wglGetExtensionsStringEXT");
		return false;
	}

	return true;
}
#else
#endif

NS_MUGGLE_END