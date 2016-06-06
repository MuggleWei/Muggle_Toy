#include "multiple_light.h"
#include "application/application.h"
#include "render/renderer.h"
#include "glad/glad.h"
#include "utility/timer.h"
#include "shader_obj.h"
#include "shader_program_glsl.h"
#include "math/matrix4x4.h"
#include "math/quaternion.h"
#include "utility/math_utils.h"
#include "application/camera.h"
#include "mesh.h"

enum
{
	VBO_Vertex = 0,
	VBO_Index,
	VBO_MAX
};

static muggle::Renderer* renderer = nullptr;
static muggle::ShaderObj *vert_shader = nullptr, *frag_shader = nullptr;
static muggle::ShaderProgramGLSL shader_program;
static GLuint vao_handle = 0;
static GLuint vbo_handles[VBO_MAX] = { 0 };
static float angle_radian = 0.0f;
static muggle::Camera camera;
static muggle::MeshData* p_mesh = nullptr;
static muggle::matrix4f mat_model;
static muggle::matrix4f mat_view;
static muggle::matrix4f mat_projection;
static muggle::matrix4f mat_mv;
static muggle::matrix3f mat_normal;
static muggle::matrix4f mat_mvp;

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

	// camera initialize
	camera.Update();
}
void Update()
{
	camera.Update();

	mat_model = muggle::MathUtils::Scale(muggle::vec3f(10.0f, 10.0f, 10.0f));
	mat_view = camera.getViewMatrix();
	mat_projection = camera.getProjectionMatrix();

	mat_mv = mat_model * mat_view;
	mat_normal = (muggle::matrix3f)mat_mv;
	mat_normal = muggle::matrix3f::Inverse(mat_normal);
	mat_normal = muggle::matrix3f::Transpose(mat_normal);
	mat_mvp = mat_mv * mat_projection;
}
void Render()
{
	// enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	shader_program.Use();

	glBindVertexArray(vao_handle);

	muggle::vec4f light_position_in_eye;

	// set uniform variable in shader
	light_position_in_eye = mat_view.Multiply(muggle::vec4f(0.0f, 0.0f, 5.0f, 1.0f));
	shader_program.setUniform("lights[0].Position", light_position_in_eye);
	shader_program.setUniform("lights[0].Intensity", muggle::vec3f(0.3f, 0.0f, 0.0f));
	light_position_in_eye = mat_view.Multiply(muggle::vec4f(0.0f, 0.0f, -5.0f, 1.0f));
	shader_program.setUniform("lights[1].Position", light_position_in_eye);
	shader_program.setUniform("lights[1].Intensity", muggle::vec3f(0.0f, 0.3f, 0.0f));
	light_position_in_eye = mat_view.Multiply(muggle::vec4f(5.0f, 0.0f, 0.0f, 1.0f));
	shader_program.setUniform("lights[2].Position", light_position_in_eye);
	shader_program.setUniform("lights[2].Intensity", muggle::vec3f(0.0f, 0.0f, 0.3f));
	light_position_in_eye = mat_view.Multiply(muggle::vec4f(-5.0f, 0.0f, 0.0f, 1.0f));
	shader_program.setUniform("lights[3].Position", light_position_in_eye);
	shader_program.setUniform("lights[3].Intensity", muggle::vec3f(0.15f, 0.2f, 0.3f));
	light_position_in_eye = mat_view.Multiply(muggle::vec4f(5.0f, 5.0f, -5.0f, 1.0f));
	shader_program.setUniform("lights[4].Position", light_position_in_eye);
	shader_program.setUniform("lights[4].Intensity", muggle::vec3f(0.1f, 0.1f, 0.7f));
	shader_program.setUniform("Material.Ka", 0.7f, 0.7f, 0.7f);
	shader_program.setUniform("Material.Kd", 0.7f, 0.7f, 0.7f);
	shader_program.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
	shader_program.setUniform("Material.Shininess", 100.0f);

	shader_program.setUniform("ModelViewMatrix", mat_mv);
	shader_program.setUniform("NormalMatrix", mat_normal);
	shader_program.setUniform("MVP", mat_mvp);

	int index_type = (p_mesh->size_index == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT);
	glDrawElements(GL_TRIANGLES, p_mesh->num_index, index_type, (GLvoid*)NULL);
}
void Destroy()
{
	// delete vbo and vao
	glDeleteBuffers(VBO_MAX, vbo_handles);
	glDeleteVertexArrays(1, &vao_handle);

	// delete data
	delete p_mesh;

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
	p_mesh = muggle::Mesh::Load("res/Stanford 3D Scanning/bunny/reconstruction/bun_zipper.ply");

	CreateVBO();
	CreateVAO();
}
void PrepareShader()
{
	const char* vert_shader_name = "res_learn_opengl/shaders/multiple_light_vert.glsl";
	const char* frag_shader_name = "res_learn_opengl/shaders/multiple_light_frag.glsl";

	// create shader object
	vert_shader = muggle::CreateShaderObj(
		renderer, vert_shader_name, "main",
		muggle::ShaderStageType::VS, muggle::ShaderType::GLSL
	);
	frag_shader = muggle::CreateShaderObj(
		renderer, frag_shader_name, "main",
		muggle::ShaderStageType::PS, muggle::ShaderType::GLSL
	);

	// initialize shader program
	shader_program.Initialize();
	shader_program.Attach(vert_shader);
	shader_program.Attach(frag_shader);

	// link shader program
	shader_program.Link();
}

void CreateVBO()
{
	// create buffer object
	glGenBuffers(VBO_MAX, vbo_handles);
	MASSERT_MSG(vbo_handles[VBO_Vertex] != 0 && vbo_handles[VBO_Index] != 0, "Failed in Generate buffer object");

	// vertex buffer
	size_t vert_size = p_mesh->num_vertex * p_mesh->vertex_decl.stride;
	glBindBuffer(GL_ARRAY_BUFFER, vbo_handles[VBO_Vertex]);
	glBufferData(GL_ARRAY_BUFFER, vert_size, (void*)p_mesh->ptr_vertices, GL_STATIC_DRAW);

	// index buffer
	size_t index_size = p_mesh->size_index * p_mesh->num_index;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_handles[VBO_Index]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, (void*)p_mesh->ptr_indices, GL_STATIC_DRAW);

	// end bind vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void CreateVAO()
{
	// create and bind to a vertex array object
	glGenVertexArrays(1, &vao_handle);
	glBindVertexArray(vao_handle);

	// enable vertex attribute arrays
	glEnableVertexAttribArray(0);		// vertex position
	glEnableVertexAttribArray(1);		// vertex normal

										// map attribute index to buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo_handles[VBO_Vertex]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		p_mesh->vertex_decl.stride, (void*)p_mesh->vertex_decl.offsets[muggle::VertexAttribute::Position]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		p_mesh->vertex_decl.stride, (void*)p_mesh->vertex_decl.offsets[muggle::VertexAttribute::Normal]);

	// bind index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_handles[VBO_Index]);

	// end bind vao
	glBindVertexArray(0);
}