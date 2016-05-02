#include "fade_cycle.h"
#include "application/application.h"
#include "render/renderer.h"
#include "glad/glad.h"
#include "utility/timer.h"
#include "shader_obj.h"
#include "shader_program_glsl.h"
#include "math/matrix4x4.h"
#include "math/quaternion.h"

static muggle::Renderer* renderer = nullptr;
static muggle::ShaderObj *vert_shader = nullptr, *frag_shader = nullptr;
static muggle::ShaderProgramGLSL shader_program;
static GLuint vao_handle = 0;
static GLuint vbo_handles[2] = { 0 };
static GLuint num_vertex = 0;
static float angle_radian = 0.0f;

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
	
	PrepareShader();
	PrepareData();
}
void Update()
{
	angle_radian += 0.01f;
	muggle::quatf quat = muggle::quatf::FromYawPitchRoll(0.0f, 0.0f, angle_radian);
	muggle::matrix4f rotate_mat = muggle::matrix4f::Rotate(quat);
}
void Render()
{
	shader_program.Use();
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
	// get uniform block
	GLuint block_index = glGetUniformBlockIndex(shader_program.getHandle(), "FadeSetting");
	if (block_index == GL_INVALID_INDEX)
	{
		MERROR(0, "Can't find uniform block: FadeSetting");
		return;
	}

	// allocate space for the buffer to contain the data for the uniform block
	GLint block_size;
	glGetActiveUniformBlockiv(shader_program.getHandle(), block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);
	GLubyte* block_buffer = (GLubyte *)malloc(block_size);

	// query for the offsets of each block variable
	const GLchar *names[] = {
		"InnerColor", "OuterColor",
		"RadiusInner", "RadiusOuter"
	};
	GLuint indices[4];
	glGetUniformIndices(shader_program.getHandle(), 4, names, indices);
	GLint offset[4];
	glGetActiveUniformsiv(shader_program.getHandle(), 4, indices, GL_UNIFORM_OFFSET, offset);

	// place the data into the buffer at the appropriate offsets
	GLfloat outer_color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat inner_color[] = { 1.0f, 1.0f, 0.75f, 1.0f };
	GLfloat inner_radius = 0.25f, outer_radius = 0.45f;
	memcpy(block_buffer + offset[0], inner_color, 4 * sizeof(GLfloat));
	memcpy(block_buffer + offset[1], outer_color, 4 * sizeof(GLfloat));
	memcpy(block_buffer + offset[2], &inner_radius, sizeof(GLfloat));
	memcpy(block_buffer + offset[3], &outer_radius, sizeof(GLfloat));

	// create the OpenGL buffer object and copy data into it
	GLuint ubo_handle;
	glGenBuffers(1, &ubo_handle);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle);
	glBufferData(GL_UNIFORM_BUFFER, block_size, block_buffer, GL_DYNAMIC_DRAW);

	// bind buffer object to uniform block
	glBindBufferBase(GL_UNIFORM_BUFFER, block_index, ubo_handle);

	// free block buffer
	free(block_buffer);

	// vertex data
	float position_data[] =
	{
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.8f,  0.8f, 0.0f,
		-0.8f, -0.8f, 0.0f,
		0.8f, 0.8f, 0.0f,
		-0.8f, 0.8f, 0.0f
	};
	float uv_data[] = 
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_data), uv_data, GL_STATIC_DRAW);

	// create and bind to a vertex array object
	num_vertex = sizeof(position_data) / (3 * sizeof(position_data[0]));
	glGenVertexArrays(1, &vao_handle);
	glBindVertexArray(vao_handle);

	// vertex attribute index: [0, GL_MAX_VERTEX_ATTRIBS ¨C 1]
	glEnableVertexAttribArray(0);	// position 
	glEnableVertexAttribArray(1);	// uv
	glBindBuffer(GL_ARRAY_BUFFER, position_buf_obj);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, color_buf_obj);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}
void PrepareShader()
{
	// create shader object
	vert_shader = muggle::CreateShaderObj(
		renderer, "res_learn_opengl/shaders/fade_cycle_vert.glsl", "main",
		muggle::ShaderStageType::VS, muggle::ShaderType::GLSL
	);
	frag_shader = muggle::CreateShaderObj(
		renderer, "res_learn_opengl/shaders/fade_cycle_frag.glsl", "main",
		muggle::ShaderStageType::PS, muggle::ShaderType::GLSL
	);

	// initialize shader program
	shader_program.Initialize();
	shader_program.Attach(vert_shader);
	shader_program.Attach(frag_shader);

	// link shader program
	shader_program.Link();
}