#include "opengl_light.h"
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
#include "application/input.h"
#include "mesh.h"

enum
{
	VBO_Vertex = 0,
	VBO_Index,
	VBO_MAX
};

enum eTestLightType
{
	TLT_Position = 0,
	TLT_Direction,
	TLT_Spot,
	TLT_MAX,
};

enum eTestMeshObj
{
	TMO_Plane = 0,
	TMO_Sphere,
	TMO_Torus,
	TMO_MAX,
};

static muggle::Renderer* renderer = nullptr;
static muggle::ShaderObj *vert_shader = nullptr, *frag_shader = nullptr;
static muggle::ShaderProgramGLSL shader_program;
static GLuint vao_handles[TMO_MAX] = { 0 };
static GLuint vbo_handles[TMO_MAX][VBO_MAX] = { 0 };
static float angle_radian = 0.0f;
static muggle::Camera camera;
static muggle::MeshData* p_mesh[TMO_MAX] = { nullptr };
static muggle::matrix4f mat_model[TMO_MAX];
static muggle::matrix4f mat_view;
static muggle::matrix4f mat_projection;
static muggle::matrix4f mat_mv;
static muggle::matrix3f mat_normal;
static muggle::matrix4f mat_mvp;
static GLuint shade_model[TLT_MAX];
static GLuint shade_model_index = 0;

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

	mat_view = camera.getViewMatrix();
	mat_projection = camera.getProjectionMatrix();

	mat_model[TMO_Plane] = muggle::MathUtils::Rotate(muggle::quatf::FromYawPitchRoll(0.0f, (float)-HALF_PI, 0.0f));
	mat_model[TMO_Sphere] = muggle::MathUtils::Translate(muggle::vec3f(3.0f, 0.0f, -4.0f));
	mat_model[TMO_Torus] = muggle::MathUtils::Translate(muggle::vec3f(-3.0f, 0.0f, -4.0f));

	if (muggle::Input::GetKeyDown(muggle::eKeyCode::Number1) ||
		muggle::Input::GetKeyDown(muggle::eKeyCode::Key1))
	{
		shade_model_index = shade_model[TLT_Position];
	}
	if (muggle::Input::GetKeyDown(muggle::eKeyCode::Number2) ||
		muggle::Input::GetKeyDown(muggle::eKeyCode::Key2))
	{
		shade_model_index = shade_model[TLT_Direction];
	}
	if (muggle::Input::GetKeyDown(muggle::eKeyCode::Number3) ||
		muggle::Input::GetKeyDown(muggle::eKeyCode::Key3))
	{
		shade_model_index = shade_model[TLT_Spot];
	}
}
void Render()
{
	// enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	shader_program.Use();

	// set uniform variable in shader
	if (shade_model_index == shade_model[TLT_Position])
	{
		muggle::vec4f light_position_in_eye = mat_view.Multiply(muggle::vec4f(5.0f, 5.0f, -5.0f, 1.0f));
		shader_program.setUniform("PositionLight.Position", light_position_in_eye);
		shader_program.setUniform("PositionLight.Intensity", muggle::vec3f(0.7f, 0.7f, 0.7f));
	}
	else if (shade_model_index == shade_model[TLT_Direction])
	{
		muggle::vec4f light_dir_in_eye = mat_view.Multiply(muggle::vec4f(-5.0f, -5.0f, 5.0f, 0.0f));
		shader_program.setUniform("DirectionLight.Direction", light_dir_in_eye);
		shader_program.setUniform("DirectionLight.Intensity", muggle::vec3f(0.7f, 0.7f, 0.7f));
	}
	else if (shade_model_index == shade_model[TLT_Spot])
	{
		muggle::vec4f light_position_in_eye = mat_view.Multiply(muggle::vec4f(5.0f, 5.0f, -8.0f, 1.0f));
		muggle::vec4f light_dir_in_eye = mat_view.Multiply(muggle::vec4f(-1.0f, -1.0f, 1.0f, 0.0f));
		light_dir_in_eye.Normalize();
		shader_program.setUniform("SpotLight.Position", light_position_in_eye);
		shader_program.setUniform("SpotLight.Intensity", muggle::vec3f(0.7f, 0.7f, 0.7f));
		shader_program.setUniform("SpotLight.Direction", muggle::vec3f(light_dir_in_eye));
		shader_program.setUniform("SpotLight.Exponent", 30.0f);
		shader_program.setUniform("SpotLight.Cutoff", 45.0f);
	}
	shader_program.setUniform("Material.Ka", 0.9f, 0.5f, 0.3f);
	shader_program.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
	shader_program.setUniform("Material.Ks", 0.2f, 0.2f, 0.2f);
	shader_program.setUniform("Material.Shininess", 100.0f);

	for (size_t i = 0; i < TMO_MAX; ++i)
	{
		glBindVertexArray(vao_handles[i]);

		mat_mv = mat_model[i] * mat_view;
		mat_normal = (muggle::matrix3f)mat_mv;
		mat_normal = muggle::matrix3f::Inverse(mat_normal);
		mat_normal = muggle::matrix3f::Transpose(mat_normal);
		mat_mvp = mat_mv * mat_projection;

		shader_program.setUniform("ModelViewMatrix", mat_mv);
		shader_program.setUniform("NormalMatrix", mat_normal);
		shader_program.setUniform("MVP", mat_mvp);

		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &shade_model_index);
		int index_type = (p_mesh[i]->size_index == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT);
		glDrawElements(GL_TRIANGLES, p_mesh[i]->num_index, index_type, (GLvoid*)NULL);
	}
}
void Destroy()
{
	// delete data
	for (int i = 0; i < TMO_MAX; ++i)
	{
		// delete vbo and vao
		glDeleteBuffers(VBO_MAX, vbo_handles[i]);
		glDeleteVertexArrays(1, &vao_handles[i]);

		delete p_mesh[i];
	}


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
	p_mesh[TMO_Plane] = muggle::GeometryMesh::GeneratePlane(20.0f, 20.0f);
	p_mesh[TMO_Sphere] = muggle::GeometryMesh::GenerateSphere(2.0f, 30, 30);
	p_mesh[TMO_Torus] = muggle::GeometryMesh::GenerateTorus(2.0f, 1.0f, 30, 30);

	CreateVBO();
	CreateVAO();
}
void PrepareShader()
{
	const char* vert_shader_name = "res_learn_opengl/shaders/light_vert.glsl";
	const char* frag_shader_name = "res_learn_opengl/shaders/light_frag.glsl";

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

	// get subroutine index
	shade_model[TLT_Position] = glGetSubroutineIndex(shader_program.getHandle(), GL_FRAGMENT_SHADER, "PositionLightBlinnPhongModel");
	shade_model[TLT_Direction] = glGetSubroutineIndex(shader_program.getHandle(), GL_FRAGMENT_SHADER, "DirectionLightBlinnPhongModel");
	shade_model[TLT_Spot] = glGetSubroutineIndex(shader_program.getHandle(), GL_FRAGMENT_SHADER, "SpotLightBlinnPhongModel");
	for (int i = 0; i < TLT_MAX; ++i)
	{
		if (GL_INVALID_INDEX == shade_model[i])
		{
			MASSERT(0);
		}
	}
}

void CreateVBO()
{
	for (size_t i = 0; i < TMO_MAX; ++i)
	{
		// create buffer object
		glGenBuffers(VBO_MAX, vbo_handles[i]);
		MASSERT_MSG(vbo_handles[i][VBO_Vertex] != 0 && vbo_handles[i][VBO_Index] != 0, "Failed in Generate buffer object");

		// vertex buffer
		size_t vert_size = p_mesh[i]->num_vertex * p_mesh[i]->vertex_decl.stride;
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handles[i][VBO_Vertex]);
		glBufferData(GL_ARRAY_BUFFER, vert_size, (void*)p_mesh[i]->ptr_vertices, GL_STATIC_DRAW);

		// index buffer
		size_t index_size = p_mesh[i]->size_index * p_mesh[i]->num_index;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_handles[i][VBO_Index]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, (void*)p_mesh[i]->ptr_indices, GL_STATIC_DRAW);

		// end bind vbo
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
void CreateVAO()
{
	for (size_t i = 0; i < TMO_MAX; ++i)
	{
		// create and bind to a vertex array object
		glGenVertexArrays(1, &vao_handles[i]);
		glBindVertexArray(vao_handles[i]);

		// enable vertex attribute arrays
		glEnableVertexAttribArray(0);		// vertex position
		glEnableVertexAttribArray(1);		// vertex normal

											// map attribute index to buffer
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handles[i][VBO_Vertex]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			p_mesh[i]->vertex_decl.stride, (void*)p_mesh[i]->vertex_decl.offsets[muggle::VertexAttribute::Position]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			p_mesh[i]->vertex_decl.stride, (void*)p_mesh[i]->vertex_decl.offsets[muggle::VertexAttribute::Normal]);

		// bind index
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_handles[i][VBO_Index]);

		// end bind vao
		glBindVertexArray(0);
	}
}