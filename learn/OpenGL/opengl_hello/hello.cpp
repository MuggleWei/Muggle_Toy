#include "hello.h"
#include "application/application.h"
#include "render/renderer.h"
#include "glad/glad.h"

MG_DLL void Init()
{
	muggle::Application* app = muggle::GetApplication();
	muggle::Renderer* renderer = app->getRenderer();
	renderer->setClearColor(muggle::vec4f(0.3f, 0.3f, 0.3f, 0.3f));

	if (!gladLoadGL())
	{
		MASSERT_MSG(0, "Failed in gladLoadGL");
		return;
	}
}
MG_DLL void Update()
{}
MG_DLL void Render()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}