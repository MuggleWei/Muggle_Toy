#ifndef __MUGGLE_APPLICATION_H__
#define __MUGGLE_APPLICATION_H__

#include "base/base.h"
#include "render/render_info.h"
#include <vector>

NS_MUGGLE_BEGIN

#define FOREACH_ENUM_APP_TYPE(_) \
	_(SingleTest)
ENUM_STRUCT_DEFINE(AppType, FOREACH_ENUM_APP_TYPE)

class Application
{
public:
	MG_DLL Application();
	MG_DLL ~Application();

	Application(const Application&) = delete;
	Application(const Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(const Application&&) = delete;

	MG_DLL void ParseCmdLine(const char* cmd_line);

	MG_DLL bool Initialize();
	MG_DLL void Destroy();
	MG_DLL void Run();

protected:
	bool LoadDll();

protected:
	AppType::Enum m_app_type;
	RenderType::Enum m_render_type;

	std::vector<void*> m_dlls;
};

NS_MUGGLE_END

#endif