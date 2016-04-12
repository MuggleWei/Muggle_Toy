#include "base/base.h"
#include "utility/error_code.h"
#include "application/application.h"

MG_DLL int _muggle_main_(const char* cmd_line)
{
	// detect memory leak
#if MG_PLATFORM_WINDOWS && MUGGLE_DEBUG
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);
#endif	

	// create application object
	MLOG("Create Application object\n");
	muggle::Application *app = new muggle::Application;
	if (app == nullptr)
	{
		MASSERT_MSG(0, "Failed in create application object");
		return muggle::ErrorCode::App_New;
	}

	// parse command line
	app->ParseCmdLine(cmd_line);

	// initialize application object
	MLOG("Initialize Application object\n");
	bool result = app->Initialize();
	if (!result)
	{
		MASSERT_MSG(0, "Failed in initialize application object");
		return muggle::ErrorCode::App_Init;
	}

	// run application
	MLOG("Run Application\n");
	app->Run();

	// destroy application
	MLOG("Destroy Application\n");
	app->Destroy();
	SAFE_DELETE(app);

	// detect memory leak
#if MG_PLATFORM_WINDOWS && MUGGLE_DEBUG
	_CrtMemCheckpoint(&s2);
	int mem_diff = _CrtMemDifference(&s3, &s1, &s2);
	if (mem_diff)
	{
		_CrtMemDumpStatistics(&s3);
		_CrtDumpMemoryLeaks();
	}
	MASSERT_MSG(mem_diff == 0, "memory leak!!!");
#endif

	return 0;
}