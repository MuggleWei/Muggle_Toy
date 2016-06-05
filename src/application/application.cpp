#include "application.h"
#include "base/dll.h"
#include "base/str_utils.h"
#include "utility/string_utils.h"
#include "utility/timer.h"
#include "utility/scope_time.h"
#include "utility/math_utils.h"
#include "window.h"
#include "input.h"
#include "renderer.h"

NS_MUGGLE_BEGIN

static Application* s_app = nullptr;

Application* GetApplication()
{
	return s_app;
}

Application::Application()
{
	m_win = nullptr;

	m_app_type = AppType::Enum::RawTest;
	m_render_type = RenderType::Enum::D3D11;
	m_render_version_major = 0;
	m_render_version_minor = 0;
	m_vsync = false;
	m_lock_fps = false;
	m_lock_fps_value = 16.666667;

	m_project_dll = nullptr;
	m_project_name = "";
	memset(m_raw_test_callback, 0, sizeof(m_raw_test_callback));
}
Application::~Application()
{}

const char* Application::getAppName()
{
	if (m_app_type == AppType::Enum::RawTest)
	{
		return m_project_name.c_str();
	}

	return "unknown";
}

bool Application::ParseCmdLine(const char* cmd_line)
{
	// get copy of command line
	size_t len = strlen(cmd_line);
	char* cmd_line_cpy = (char*)malloc(len + 1);
	memcpy(cmd_line_cpy, cmd_line, len);
	cmd_line_cpy[len] = '\0';

	char* p = cmd_line_cpy;
	char* q = p;

	// split into arguments vector
	std::vector<char*> args;
	while (1)
	{
		MG_SKIP_BLANK(p);

		if (*p == '\0')
		{
			break;
		}

		q = p;
		while (*p != '\0')
		{
			if (*p == ' ' || *p == '\t')
			{
				*p = '\0';
				++p;
				break;
			}
			++p;
		}

		args.push_back(q);
	}

	// parse command
	bool parse_result = true;
	for (size_t i = 0; i < args.size(); i++)
	{
		if (strcmp(args[i], "--Mode") == 0)
		{
			++i;
			if (i >= args.size())
			{
				MASSERT_MSG(0, "Failed parse command in option --Mode");
				parse_result = false;
				break;
			}
			AppType::Enum enum_index = AppType::StringToEnum(args[i]);
			if (enum_index == AppType::Enum::Max)
			{
				MASSERT_MSG(0, "--Mode %s is not exist enumerator", args[i]);
				parse_result = false;
				break;
			}
			m_app_type = enum_index;
		}
		else if (strcmp(args[i], "--RenderType") == 0)
		{
			++i;
			if (i >= args.size())
			{
				MASSERT_MSG(0, "Failed parse command in option --RenderType");
				parse_result = false;
				break;
			}
			RenderType::Enum enum_index = RenderType::StringToEnum(args[i]);
			if (enum_index == RenderType::Enum::Max)
			{
				MASSERT_MSG(0, "--RenderType %s is not exist enumerator", args[i]);
				parse_result = false;
				break;
			}
			m_render_type = enum_index;
		}
		else if (strcmp(args[i], "--RenderVersion.major") == 0)
		{
			++i;
			if (i >= args.size())
			{
				MASSERT_MSG(0, "Failed parse command in option --RenderVersion.major");
				parse_result = false;
				break;
			}
			if (!StringUtils::IsNumber(args[i]))
			{
				MASSERT_MSG(0, "--RenderVersion.major %s need number", args[i]);
				parse_result = false;
				break;
			}
			m_render_version_major = StringUtils::ToInt(args[i]);
		}
		else if (strcmp(args[i], "--RenderVersion.minor") == 0)
		{
			++i;
			if (i >= args.size())
			{
				MASSERT_MSG(0, "Failed parse command in option --RenderVersion.minor");
				parse_result = false;
				break;
			}
			if (!StringUtils::IsNumber(args[i]))
			{
				MASSERT_MSG(0, "--RenderVersion.minor %s need number", args[i]);
				parse_result = false;
				break;
			}
			m_render_version_minor = StringUtils::ToInt(args[i]);
		}
		else if (strcmp(args[i], "--project") == 0)
		{
			++i;
			if (i >= args.size())
			{
				MASSERT_MSG(0, "Failed parse command in option --project");
				parse_result = false;
				break;
			}
			m_project_name = args[i];
		}
		else if (strcmp(args[i], "--vsync") == 0)
		{
			++i;
			if (i >= args.size())
			{
				MASSERT_MSG(0, "Failed parse command in option --lockfps");
				parse_result = false;
				break;
			}
			if (strcmp(args[i], "on") == 0)
			{
				m_vsync = true;
			}
			else if (strcmp(args[i], "off") == 0)
			{
				m_vsync = false;
			}
			else
			{
				MASSERT_MSG(0, "--vsync %s is not a valid command", args[i]);
				parse_result = false;
				break;
			}
		}
		else if (strcmp(args[i], "--lockfps") == 0)
		{
			++i;
			if (i >= args.size())
			{
				MASSERT_MSG(0, "Failed parse command in option --lockfps");
				parse_result = false;
				break;
			}
			if (strcmp(args[i], "on") == 0)
			{
				m_lock_fps = true;
			}
			else if (strcmp(args[i], "off") == 0)
			{
				m_lock_fps = false;
			}
			else
			{
				MASSERT_MSG(0, "--lockfps %s is not a valid command", args[i]);
				parse_result = false;
				break;
			}
		}
		else if (strcmp(args[i], "--lockfps.value") == 0)
		{
			++i;
			if (i >= args.size())
			{
				MASSERT_MSG(0, "Failed parse command in option --lockfps.value");
				parse_result = false;
				break;
			}
			if (!StringUtils::IsNumber(args[i]))
			{
				MASSERT_MSG(0, "--lockfps.value %s is not number", args[i]);
				parse_result = false;
				break;
			}
			m_lock_fps_value = StringUtils::ToDouble(args[i]);
		}
	}

	// free copy of command line
	free(cmd_line_cpy);

	return parse_result;
}

bool Application::Initialize()
{
	s_app = this;

	switch (m_app_type)
	{
	case AppType::Enum::RawTest:
	{
		bool result = true;

		// intialize math rule
		if (m_render_type == RenderType::Enum::D3D11)
		{
			SetMathUseGL(false);
		}
		else
		{
			SetMathUseGL(true);
		}

		// load project dll
		if (!LoadRawTestDll())
		{
			MASSERT(0);
			return false;
		}

		// initialize window
		result = InitWindow();
		if (!result)
		{
			MASSERT_MSG(0, "Failed init window");
			return false;
		}

		// initialize renderer
		result = InitRenderer();

		// init project
		if (m_raw_test_callback[RawTestCallback::Init])
		{
			(*m_raw_test_callback[RawTestCallback::Init])();
		}
	}break;
	}
	
	return true;
}
void Application::Destroy()
{
	switch (m_app_type)
	{
	case AppType::Enum::RawTest:
	{
		// destroy project
		if (m_project_dll != nullptr)
		{
			if (m_raw_test_callback[RawTestCallback::Destroy])
			{
				(*m_raw_test_callback[RawTestCallback::Destroy])();
			}			

			Dll_Free(m_project_dll);
			m_project_dll = nullptr;
		}

		// destroy renderer
		DestroyRenderer();

		// destroy window
		DestroyWindow();
	}break;
	}

	s_app = nullptr;
}
void Application::Run()
{
#if MG_PLATFORM_WINDOWS
	MSG msg;
	bool done, result = true;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// lock fps
	if (m_lock_fps)
	{
		Timer::LockDelta(m_lock_fps_value);
	}
	else
	{
		Timer::LockDelta(-1.0);
	}

	// timer ready
	Timer::Reset();
	double accumulate_time = 0.0;
	Timer::Tick();
	double last_time = Timer::GetLastTickTime();

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		Input::Update();

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
			SCOPE_TIME_RESET;
			SCOPE_TIME_COUNT(sum);

			// lock fps
			HandleLockFps(last_time);

			// calculate time
			Timer::Tick();
			last_time = Timer::GetLastTickTime();
			accumulate_time += Timer::DeltaTime();

			// Otherwise do the frame processing.
			if (Input::GetKeyUp(eKeyCode::Escape))
			{
				result = false;
			}
			if (!result)
			{
				done = true;
			}

			// application loop
			Update();
			HandleFixedUpdate(accumulate_time);
			Render();
		}
	}
#endif

	return;
}

bool Application::LoadRawTestDll()
{
	std::string dll_path = "./lib" + m_project_name;
	const char* tmp = dll_path.c_str();
	m_project_dll = DLL_Load(tmp);
	if (m_project_dll == nullptr)
	{
		MASSERT_MSG(0, "Failed in load: %s dynamic lib", tmp);
		return false;
	}

	for (size_t i = 0; i < RawTestCallback::Max; i++)
	{
		const char* func_name = RawTestCallback::EnumToString((RawTestCallback::Enum)i);
		m_raw_test_callback[i] = (App_RawTestCallbackPtr)Dll_QueryFunc(m_project_dll, func_name);
		if (m_raw_test_callback[i] == nullptr)
		{
			MWARNING(0, "Failed in query funcion: %s in %s dynamic lib", func_name, tmp);
		}
	}

	return true;
}

bool Application::InitWindow()
{
	m_win = new Window();
	if (m_win == nullptr)
	{
		MASSERT_MSG(0, "Failed in create window object");
		return false;
	}

	bool result = m_win->Initialize(this);
	if (!result)
	{
		MASSERT_MSG(0, "Failed in initialize window object");
		return false;
	}

	return true;
}
void Application::DestroyWindow()
{
	if (m_win)
	{
		m_win->Destroy();
		delete m_win;
		m_win = nullptr;
	}
}

bool Application::InitRenderer()
{
	m_renderer = CreateRenderer(m_render_type);
	if (m_renderer == nullptr)
	{
		MASSERT_MSG(0, "Failed in create renderer object");
		return false;
	}

	// fill out RenderInitParameter
	RenderInitParameter param;
	memset(&param, 0, sizeof(param));
	WindowInfo win_info = m_win->getWinInfo();
	param.hWnd = (void*)win_info.hWnd;
	param.full_screen = win_info.full_screen;
	param.vsync = m_vsync;
	param.win_width = win_info.width;
	param.win_height = win_info.height;
	param.rt_format = ImageFormat::Enum::IMAGE_FMT_RGBA8_UNORM;
	param.version_major = m_render_version_major;
	param.version_minor = m_render_version_minor;

	bool result = m_renderer->Initialize(param);
	if (!result)
	{
		MASSERT_MSG(0, "Failed in initialize renderer object");
		return false;
	}

	return true;
}
void Application::DestroyRenderer()
{
	if (m_renderer)
	{
		m_renderer->Destroy();
		delete m_renderer;
		m_renderer = nullptr;
	}
}

void Application::HandleLockFps(double last_time)
{
	SCOPE_TIME_COUNT(lock_fps);
	if (Timer::IsLockDelta())
	{
		long int sleep_time = (long int)(last_time + Timer::GetLockDeltaTime() - Timer::GetMilliseconds());
		if (sleep_time > 0)
		{
			MSleep(sleep_time);
		}
	}
}
void Application::HandleFixedUpdate(double& accumulate_time)
{
	int count = 0;
	while (accumulate_time >= Timer::FixedDeltaTime())
	{
		Timer cur_fixed_delta;
		cur_fixed_delta.Start();
		FixUpdate();
		cur_fixed_delta.End();
		accumulate_time -= Timer::FixedDeltaTime();

		++count;
		if (count >= 10)
		{
			accumulate_time = 0.0;
			break;
		}
	}
}

void Application::Update()
{
	// MLOG("%f\n", Timer::DeltaTime());
	SCOPE_TIME_COUNT(update);
	switch (m_app_type)
	{
		case AppType::Enum::RawTest:
		{
			if (m_raw_test_callback[RawTestCallback::Update])
			{
				(*m_raw_test_callback[RawTestCallback::Update])();
			}
			
		}break;
	}
}
void Application::FixUpdate()
{
	SCOPE_TIME_COUNT(FixUpdate);
}
void Application::Render()
{
	SCOPE_TIME_COUNT(Render);
	switch (m_app_type)
	{
		case AppType::Enum::RawTest:
		{
			m_renderer->BeginScene();

			if (m_raw_test_callback[RawTestCallback::Render])
			{
				(*m_raw_test_callback[RawTestCallback::Render])();
			}		

			m_renderer->EndScene();
		}break;
	}
}

NS_MUGGLE_END