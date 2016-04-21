#include "hello.h"
#include "application/application.h"
#include "render/renderer.h"

static Hello s_hello;

MG_DLL void Init()
{
	s_hello.Init();
}
MG_DLL void Update()
{
	s_hello.Update();
}
MG_DLL void Render()
{
	s_hello.Render();
}

void Hello::Init()
{
	muggle::Application* app = muggle::GetApplication();
	muggle::Renderer* renderer = app->getRenderer();
	renderer->setClearColor(muggle::vec4f(0.3f, 0.3f, 0.3f, 0.3f));
}
void Hello::Update()
{}
void Hello::Render()
{}