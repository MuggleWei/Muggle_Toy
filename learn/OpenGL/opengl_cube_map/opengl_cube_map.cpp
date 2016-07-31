#include "opengl_cube_map.h"
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
#include "timer.h"
#include "input.h"
#include "mesh.h"
#include "texture.h"

enum
{
	VBO_Vertex = 0,
	VBO_Index,
	VBO_MAX
};

enum eTestMeshObj
{
	TMO_Sky = 0,
	TMO_Obj,
	TMO_MAX,
};

static muggle::Renderer* renderer = nullptr;
static muggle::ShaderObj *vert_shader = nullptr, *frag_shader = nullptr;
static muggle::ShaderProgramGLSL shader_program;
static GLuint vao_handles[TMO_MAX] = { 0 };
static GLuint vbo_handles[TMO_MAX][VBO_MAX] = { 0 };
static muggle::Camera camera;
static muggle::MeshData* p_mesh[TMO_MAX] = { nullptr };
static muggle::matrix4f mat_model;
static muggle::matrix4f mat_view;
static muggle::matrix4f mat_projection;
static muggle::matrix4f mat_mv;
static muggle::matrix3f mat_normal;
static muggle::matrix4f mat_mvp;

static muggle::Texture* cube_tex = nullptr;

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

	// muggle::quatf quat = muggle::quatf::FromYawPitchRoll(0, (float)-HALF_PI, 0.0f);
	// mat_model = muggle::MathUtils::Rotate(quat);
	mat_model = muggle::matrix4f::identify;
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

	for (int i = 0; i < TMO_MAX; ++i)
	{
		glBindVertexArray(vao_handles[i]);

		// set uniform variable in shader
		muggle::vec3f camera_pos = camera.getPosition();
		shader_program.setUniform("worldCameraPos", camera_pos);

		shader_program.setUniform("ModelMatrix", mat_model);
		// shader_program.setUniform("ModelViewMatrix", mat_mv);
		// shader_program.setUniform("ProjectionMatrix", camera.getProjectionMatrix());
		shader_program.setUniform("MVP", mat_mvp);

		shader_program.setTexture("CubeMapTex", cube_tex, 0);

		shader_program.setUniform("ReflectFactor", 0.9f);
		muggle::vec4f material_color = muggle::vec4f(0.5f, 0.5f, 0.9f, 1.0f);
		shader_program.setUniform("MaterialColor", material_color);

		// draw cube
		shader_program.setUniform("DrawSkyBox", i == TMO_Sky ? true : false);
		if (p_mesh[i]->num_index > 0)
		{
			int index_type = (p_mesh[i]->size_index == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT);
			glDrawElements(GL_TRIANGLES, p_mesh[i]->num_index, index_type, (GLvoid*)NULL);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, p_mesh[i]->num_vertex);
		}
	}	
}
void Destroy()
{
	for (int i = 0; i < TMO_MAX; ++i)
	{
		// delete vbo and vao
		glDeleteBuffers(VBO_MAX, vbo_handles[i]);
		glDeleteVertexArrays(1, &vao_handles[i]);

		// delete mesh data
		if (p_mesh[i])
		{
			delete p_mesh[i];
		}
	}	

	// delete texture
	if (cube_tex)
	{
		delete cube_tex;
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
	// load texture
	static const char* cube_images[6] = 
	{
		"res/textures/uffizi/uffizi_px.png",
		"res/textures/uffizi/uffizi_nx.png",
		"res/textures/uffizi/uffizi_py.png",
		"res/textures/uffizi/uffizi_ny.png",
		"res/textures/uffizi/uffizi_pz.png",
		"res/textures/uffizi/uffizi_nz.png",
	};
	cube_tex = muggle::Texture::LoadCube(cube_images);
	MWARNING(cube_tex != nullptr, "Failed load cube texture %s\n", cube_images[0]);

	// load mesh
	p_mesh[TMO_Sky] = muggle::GeometryMesh::GenerateSphere(100.0f, 30, 30);
	p_mesh[TMO_Obj] = muggle::GeometryMesh::GenerateCuboid(5.0f, 5.0f, 5.0f);
	// MeshDataPrint(p_mesh);

	CreateVBO();
	CreateVAO();
}
void PrepareShader()
{
	// const char* vert_shader_name = "res_learn_opengl/shaders/cube_map_vert.glsl";
	// const char* frag_shader_name = "res_learn_opengl/shaders/cube_map_frag.glsl";
	const char* vert_shader_name = "res_learn_opengl/shaders/cube_map_pixel_vert.glsl";
	const char* frag_shader_name = "res_learn_opengl/shaders/cube_map_pixel_frag.glsl";

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
	for (int i = 0; i < TMO_MAX; ++i)
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
	for (int i = 0; i < TMO_MAX; ++i)
	{
		// create and bind to a vertex array object
		glGenVertexArrays(1, &vao_handles[i]);
		glBindVertexArray(vao_handles[i]);

		// enable vertex attribute arrays
		glEnableVertexAttribArray(0);		// vertex position
		glEnableVertexAttribArray(1);		// vertex normal
		glEnableVertexAttribArray(2);		// vertex texture coordinate

		// map attribute index to buffer
		glBindBuffer(GL_ARRAY_BUFFER, vbo_handles[i][VBO_Vertex]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			p_mesh[i]->vertex_decl.stride, (void*)p_mesh[i]->vertex_decl.offsets[muggle::VertexAttribute::Position]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			p_mesh[i]->vertex_decl.stride, (void*)p_mesh[i]->vertex_decl.offsets[muggle::VertexAttribute::Normal]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			p_mesh[i]->vertex_decl.stride, (void*)p_mesh[i]->vertex_decl.offsets[muggle::VertexAttribute::TexCoord0]);

		// bind index
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_handles[i][VBO_Index]);

		// end bind vao
		glBindVertexArray(0);
	}	
}