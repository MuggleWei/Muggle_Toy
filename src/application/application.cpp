#include "application/application.h"
#include "base/dll.h"
#include "utility/string_utils.h"

typedef void (*InitType)();

NS_MUGGLE_BEGIN

ENUM_STRUCT_STRING(AppType, FOREACH_ENUM_APP_TYPE)

Application::Application()
{
	m_app_type = AppType::Enum::SingleTest;
	m_render_type = RenderType::Enum::D3D11;


}
Application::~Application()
{}

void Application::ParseCmdLine(const char* cmd_line)
{
	// TODO: parse command line
}

bool Application::Initialize()
{

	if (!LoadDll())
	{
		MASSERT(0);
		return false;
	}
	
	return true;
}
void Application::Destroy()
{
}
void Application::Run()
{
}

bool Application::LoadDll()
{
	const char* tmp = "./libhello";
	void* dll = DLL_Load(tmp);
	if (dll == nullptr)
	{
		MASSERT_MSG(0, "Failed in load: %s dynamic lib", tmp);
		return false;
	}

	const char* func_name = "PreInit_";
	InitType initFuncPtr = nullptr;
	initFuncPtr = (InitType)Dll_QueryFunc(dll, func_name);
	if (initFuncPtr == nullptr)
	{
		MASSERT_MSG(0, "Failed in query funcion: %s in %s dynamic lib", func_name, tmp);
		return false;
	}

	(*initFuncPtr)();

	if (!Dll_Free(dll))
	{
		MASSERT_MSG(0, "Failed in free %s dynamic lib", tmp);
		return false;
	}

	return true;
}

NS_MUGGLE_END