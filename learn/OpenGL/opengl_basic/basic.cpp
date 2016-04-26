#include "basic.h"
#include "application/application.h"
#include "render/renderer.h"
#include "glad/glad.h"
#include "utility/timer.h"
#include "shader_obj.h"
#include "shader_program.h"

static muggle::Renderer* renderer = nullptr;
static muggle::ShaderObj *vert_shader = nullptr, *frag_shader = nullptr;
static muggle::ShaderProgram shader_program;
static GLuint vao_handle = 0;
static GLuint vbo_handles[2] = { 0 };
static GLuint num_vertex = 0;

void Init()
{
	// get application and renderer
	muggle::Application* app = muggle::GetApplication();
	renderer = app->getRenderer();
	renderer->setClearColor(muggle::vec4f(0.3f, 0.3f, 0.3f, 0.3f));

	// initialize glad
	if (!gladLoadGL())
	{
		MASSERT_MSG(0, "Failed in gladLoadGL");
		return;
	}

	PrepareData();
	PrepareShader();	
}
void Update()
{
	// MLOG("%f\n", muggle::Timer::DeltaTime());
}
void Render()
{
	glUseProgram(shader_program.gl_handle);
	glBindVertexArray(vao_handle);
	glDrawArrays(GL_TRIANGLES, 0, num_vertex);
}
void Destroy()
{
	// delete vbo and vao
	glDeleteBuffers(2, vbo_handles);
	glDeleteVertexArrays(1, &vao_handle);

	// destroy shader program and shader object
	shader_program.Destroy();

	if (vert_shader)
	{
		vert_shader->Destroy();
		delete vert_shader;
		vert_shader = nullptr;
	}
	if (frag_shader)
	{
		frag_shader->Destroy();
		delete frag_shader;
		frag_shader = nullptr;
	}
}

void PrepareData()
{
	// vertex data
	float position_data[] =
	{
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.0f, 0.8f, 0.0f
	};
	float color_data[] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	// create buffer object
	glGenBuffers(2, vbo_handles);
	GLuint position_buf_obj = vbo_handles[0];
	GLuint color_buf_obj = vbo_handles[1];
	MASSERT_MSG(position_buf_obj != 0 && color_buf_obj != 0, "Failed in Generate buffer object");

	// populate the position buffer and color buffer
	glBindBuffer(GL_ARRAY_BUFFER, position_buf_obj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position_data), position_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, color_buf_obj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_data), color_data, GL_STATIC_DRAW);

	// create and bind to a vertex array object
	num_vertex = sizeof(position_data) / (3 * sizeof(position_data[0]));
	glGenVertexArrays(1, &vao_handle);
	glBindVertexArray(vao_handle);

	// vertex attribute index: [0, GL_MAX_VERTEX_ATTRIBS ¨C 1]
	glEnableVertexAttribArray(0);	// vertex 
	glEnableVertexAttribArray(1);	// color
	glBindBuffer(GL_ARRAY_BUFFER, position_buf_obj);
	glVertexAttribPointer(
		0, num_vertex,
		GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, color_buf_obj);
	glVertexAttribPointer(
		1, num_vertex,
		GL_FLOAT, GL_FALSE, 0, NULL);
}
void PrepareShader()
{
	// create shader object
	vert_shader = muggle::CreateShaderObj(
		renderer, "res_learn_opengl/shaders/basic_vert.glsl", "main",
		muggle::ShaderStageType::VS, muggle::ShaderType::GLSL
	);
	frag_shader = muggle::CreateShaderObj(
		renderer, "res_learn_opengl/shaders/basic_frag.glsl", "main",
		muggle::ShaderStageType::PS, muggle::ShaderType::GLSL
	);

	// initialize shader program
	shader_program.Initialize();
	shader_program.Attach(vert_shader);
	shader_program.Attach(frag_shader);

	// bind shader input variable
	glBindAttribLocation(shader_program.gl_handle, 0, "VertexPosition");
	glBindAttribLocation(shader_program.gl_handle, 1, "VertexColor");

	// link shader program
	shader_program.Link();
}