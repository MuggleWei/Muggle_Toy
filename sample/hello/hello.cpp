#include "hello.h"
#include "application/application.h"
#include "render/renderer.h"

static Hello s_hello;

void Init()
{
	s_hello.Init();
}
void Update()
{
	s_hello.Update();
}
void Render()
{
	s_hello.Render();
}
void Destroy()
{}

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