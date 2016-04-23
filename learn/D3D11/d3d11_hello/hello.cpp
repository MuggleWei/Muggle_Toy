#include "hello.h"
#include "application/application.h"
#include "render/renderer.h"
#include "render/renderer_d3d11.h"

MG_DLL void Init()
{
	muggle::Application* app = muggle::GetApplication();
	muggle::Renderer* renderer = app->getRenderer();
	renderer->setClearColor(muggle::vec4f(0.3f, 0.3f, 0.3f, 0.3f));
}
MG_DLL void Update()
{}
MG_DLL void Render()
{}