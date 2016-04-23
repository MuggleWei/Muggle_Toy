#ifndef __MUGGLE_APPLICATION_H__
#define __MUGGLE_APPLICATION_H__

#include "base/base.h"
#include "render/render_info.h"
#include <string>

typedef void (*App_SingleTestCallbackPtr)();

NS_MUGGLE_BEGIN

// application type
#define FOREACH_ENUM_APP_TYPE(_) \
	_(RawTest)
ENUM_STRUCT(AppType, FOREACH_ENUM_APP_TYPE)

// when app type is single test, must exsit function
#define FOREACH_ENUM_SINGLE_TEST_CALLBACK(_) \
	_(Init) \
	_(Update) \
	_(Render)
ENUM_STRUCT(SingleTestCallback, FOREACH_ENUM_SINGLE_TEST_CALLBACK)

// set fps lock type
#define FOREACH_ENUM_LOCK_FPS_TYPE(_) \
	_(None) \
	_(Timer_Lock) \
	_(API_Lock)
ENUM_STRUCT(LockFpsType, FOREACH_ENUM_LOCK_FPS_TYPE)

class Window;
class Renderer;
class Application;

MG_DLL Application* GetApplication();

class Application
{
public:
	MG_DLL Application();
	MG_DLL ~Application();

	Application(const Application&) = delete;
	Application(const Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(const Application&&) = delete;

	MG_DLL const char* getAppName();

	MG_DLL bool ParseCmdLine(const char* cmd_line);

	MG_DLL bool Initialize();
	MG_DLL void Destroy();
	MG_DLL void Run();

	MG_DLL Renderer* getRenderer()
	{
		return m_renderer;
	}

protected:
	bool LoadSingleTestDll();

	bool InitWindow();
	void DestroyWindow();

	bool InitRenderer();
	void DestroyRenderer();

	void HandleLockFps(double last_time);
	void HandleFixedUpdate(double& ref_accumulate_time);

	void Update();
	void FixUpdate();
	void Render();

protected:
	// window
	Window* m_win;

	// renderer
	Renderer* m_renderer;

	// status variable
	AppType::Enum m_app_type;
	RenderType::Enum m_render_type;
	LockFpsType::Enum m_lock_fps_type;
	double m_lock_fps_value;

	// AppType: RawTest
	std::string m_project_name;
	void* m_project_dll;
	App_SingleTestCallbackPtr m_st_callback[SingleTestCallback::Enum::Max];
};

NS_MUGGLE_END

#endif