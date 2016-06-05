#include "mesh.h"
#include "math/math_base.h"
#include "base/str_utils.h"
#include "file_utility.h"
#include "utility/ply.h"
#include "math/vector3.h"

NS_MUGGLE_BEGIN

/*************** VertexDeclare ***************/
static int s_attri_type_size[VertexAttributeType::Max] = {
	1, 2, 4, 4
};

void VertexDeclare::Init()
{
	stride = 0;
	for (int i = 0; i < VertexAttribute::Max; ++i)
	{
		offsets[i] = -1;
	}
	memset(attri_types, 0, sizeof(attri_types));
}
void VertexDeclare::Add(VertexAttribute::Enum attri, uint8_t num, VertexAttributeType::Enum attri_type)
{
	offsets[attri] = stride;
	attri_types[attri] = attri_type;
	stride += num * s_attri_type_size[attri_type];
}
bool VertexDeclare::Exist(VertexAttribute::Enum attri)
{
	return (offsets[attri] != -1);
}

/*************** MeshData ***************/
MeshData::MeshData()
{
	num_vertex = 0;
	num_index = 0;
	ptr_vertices = 0;
	ptr_indices = 0;
	size_index = 0;
	vertex_decl.Init();
}
MeshData::~MeshData()
{
	if (ptr_vertices != 0)
	{
		free((void*)ptr_vertices);
	}
	if (ptr_indices != 0)
	{
		free((void*)ptr_indices);
	}
}

/*************** Mesh ***************/
void PlyInitMeshDataDeclare(MeshData* p_mesh_data, PlyFile* ply_file, bool *normal_exist)
{
	bool _x = false, _y = false, _z = false;
	int num_vert = 0, num_face = 0;
	int size_face = 0;

	if (ply_file->file_type == PLY_ASCII)
	{ 
		for (int i = 0; i < ply_file->nelems; ++i)
		{
			PlyElement* elem = ply_file->elems[i];
			if (strcmp(elem->name, "vertex") == 0)
			{
				num_vert = elem->num;

				for (int j = 0; j < elem->nprops; ++j)
				{
					PlyProperty* prop = elem->props[j];
					if (prop->is_list)
					{
						MASSERT_MSG(0, "Not yet implemented");
					}
					else
					{
						if (strcmp(prop->name, "x") == 0)
						{
							_x = true;
						}
						else if (strcmp(prop->name, "y") == 0)
						{
							_y = true;
						}
						else if (strcmp(prop->name, "z") == 0)
						{
							_z = true;
						}
					}
				}
			}
			else if (strcmp(elem->name, "face") == 0)
			{
				num_face = elem->num;

				for (int j = 0; j < elem->nprops; ++j)
				{
					PlyProperty* prop = elem->props[j];
					if (prop->is_list)
					{
						size_face = PlyGetTypeSize(prop->external_type);
					}
					else
					{
						MASSERT_MSG(0, "Not yet implemented");
					}
				}
			}
		}

		if (_x && _y)
		{
			if (_z)
			{
				p_mesh_data->vertex_decl.Add(VertexAttribute::Position, 3, VertexAttributeType::Float);
			}
			else
			{
				p_mesh_data->vertex_decl.Add(VertexAttribute::Position, 2, VertexAttributeType::Float);
			}
		}

		// always to generate normal
		if (!p_mesh_data->vertex_decl.Exist(VertexAttribute::Normal))
		{
			*normal_exist = false;
			p_mesh_data->vertex_decl.Add(VertexAttribute::Normal, 3, VertexAttributeType::Float);
		}
		else
		{
			*normal_exist = true;
		}

		p_mesh_data->num_vertex = num_vert;
		p_mesh_data->ptr_vertices = (intptr_t)malloc(p_mesh_data->vertex_decl.stride * p_mesh_data->num_vertex);
		memset((void*)p_mesh_data->ptr_vertices, 0, p_mesh_data->vertex_decl.stride * p_mesh_data->num_vertex);

		if (num_face > 0)
		{
			p_mesh_data->num_index = num_face * 3;
			p_mesh_data->size_index = size_face;
			MASSERT_MSG(p_mesh_data->size_index == 2 || p_mesh_data->size_index == 4, "Index size equal 2 or 4");
			p_mesh_data->ptr_indices = (intptr_t)malloc(p_mesh_data->size_index * p_mesh_data->num_index);
			memset((void*)p_mesh_data->ptr_indices, 0, p_mesh_data->size_index * p_mesh_data->num_index);
		}
	}
	else
	{ 
		MASSERT_MSG(0, "Not yet implemented");
	}
}
MeshData* ConvertPlyToMeshData(PlyFile* ply_file)
{
	// new MeshData
	MeshData *p_mesh_data = new MeshData();

	// initialize MeshData vertex declare and index
	bool normal_exist = false;
	PlyInitMeshDataDeclare(p_mesh_data, ply_file, &normal_exist);

	// convert ply data to MeshData
	if (ply_file->file_type == PLY_ASCII)
	{
		for (int i = 0; i < ply_file->nelems; ++i)
		{
			PlyElement* elem = ply_file->elems[i];
			if (strcmp(elem->name, "vertex") == 0)
			{
				char* p_data = (char*)elem->ptr_data;

				for (int j = 0; j < elem->num; ++j)
				{
					intptr_t p_vert = p_mesh_data->ptr_vertices + j * p_mesh_data->vertex_decl.stride;

					char* words[16];
					int word_idx = 0;
					p_data = StrUtils_SplitLineToWords(p_data, words, &word_idx, 16);

					for (int k = 0; k < elem->nprops; ++k)
					{
						PlyProperty* prop = elem->props[k];
						if (prop->is_list)
						{
							MASSERT_MSG(0, "Not yet implemented");
						}
						else
						{
							if (strcmp(prop->name, "x") == 0)
							{
								float* pos = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Position]);
								pos[0] = atof(words[k]);
							}
							else if (strcmp(prop->name, "y") == 0)
							{
								float* pos = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Position]);
								pos[1] = atof(words[k]);
							}
							else if (strcmp(prop->name, "z") == 0)
							{
								float* pos = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Position]);
								pos[2] = atof(words[k]);
							}
						}						
					}
				}
			}
			else if (strcmp(elem->name, "face") == 0)
			{
				char* p_data = (char*)elem->ptr_data;

				for (int j = 0; j < elem->num; ++j)
				{
					char* words[16];
					int word_idx = 0;
					p_data = StrUtils_SplitLineToWords(p_data, words, &word_idx, 16);

					for (int k = 0; k < elem->nprops; ++k)
					{
						PlyProperty* prop = elem->props[k];
						if (prop->is_list)
						{
							if (p_mesh_data->size_index == 2)
							{
								uint16_t* indices = (uint16_t*)p_mesh_data->ptr_indices + j * 3;
								indices[0] = atoi(words[1]);
								indices[1] = atoi(words[3]);
								indices[2] = atoi(words[2]);
							}
							else if (p_mesh_data->size_index == 4)
							{
								uint32_t* indices = (uint32_t*)p_mesh_data->ptr_indices + j * 3;
								indices[0] = atoi(words[1]);
								indices[1] = atoi(words[3]);
								indices[2] = atoi(words[2]);
							}
						}
						else
						{
							MASSERT_MSG(0, "Not yet implemented");
						}
					}
				}
			}
		}
	}
	else
	{ 
		MASSERT_MSG(0, "Not yet implemented");
	}

	// if normal not exist, auto generate
	if (!normal_exist)
	{
		Mesh::GenerateNormal(p_mesh_data);
	}

	return p_mesh_data;
}

void Mesh::GenerateNormal(MeshData *p_mesh_data)
{
	if (!p_mesh_data->vertex_decl.Exist(VertexAttribute::Enum::Normal))
	{
		MWARNING(0, "Need to add normal vertex decalre for generate normal\n");
		return;
	}

	if (p_mesh_data->num_index > 0)
	{
		int num_face = p_mesh_data->num_index / 3;
		int* num_norm = (int*)malloc(sizeof(int) * p_mesh_data->num_vertex);
		memset(num_norm, 0, sizeof(int) * p_mesh_data->num_vertex);

		for (int i = 0; i < num_face; ++i)
		{
			intptr_t p_vert0 = 0, p_vert1 = 0, p_vert2 = 0;

			if (p_mesh_data->size_index == 2)
			{
				uint16_t* indices = (uint16_t*)p_mesh_data->ptr_indices + i * 3;
				p_vert0 = p_mesh_data->ptr_vertices + indices[0] * p_mesh_data->vertex_decl.stride;
				p_vert1 = p_mesh_data->ptr_vertices + indices[1] * p_mesh_data->vertex_decl.stride;
				p_vert2 = p_mesh_data->ptr_vertices + indices[2] * p_mesh_data->vertex_decl.stride;
			}
			else if (p_mesh_data->size_index == 4)
			{
				uint32_t* indices = (uint32_t*)p_mesh_data->ptr_indices + i * 3;
				p_vert0 = p_mesh_data->ptr_vertices + indices[0] * p_mesh_data->vertex_decl.stride;
				p_vert1 = p_mesh_data->ptr_vertices + indices[1] * p_mesh_data->vertex_decl.stride;
				p_vert2 = p_mesh_data->ptr_vertices + indices[2] * p_mesh_data->vertex_decl.stride;
			}
			
			vec3f* pos0 = (vec3f*)(p_vert0 + p_mesh_data->vertex_decl.offsets[VertexAttribute::Position]);
			vec3f* pos1 = (vec3f*)(p_vert1 + p_mesh_data->vertex_decl.offsets[VertexAttribute::Position]);
			vec3f* pos2 = (vec3f*)(p_vert2 + p_mesh_data->vertex_decl.offsets[VertexAttribute::Position]);
			vec3f* norm0 = (vec3f*)(p_vert0 + p_mesh_data->vertex_decl.offsets[VertexAttribute::Normal]);
			vec3f* norm1 = (vec3f*)(p_vert1 + p_mesh_data->vertex_decl.offsets[VertexAttribute::Normal]);
			vec3f* norm2 = (vec3f*)(p_vert2 + p_mesh_data->vertex_decl.offsets[VertexAttribute::Normal]);

			vec3f edge1 = *pos1 - *pos0;
			vec3f edge2 = *pos2 - *pos0;

			vec3f norm = vec3f::Cross(edge2, edge1);
			norm.Normalize();

			*norm0 = *norm0 + norm;
			*norm1 = *norm1 + norm;
			*norm2 = *norm2 + norm;

			if (p_mesh_data->size_index == 2)
			{
				uint16_t* indices = (uint16_t*)p_mesh_data->ptr_indices + i * 3;
				++num_norm[indices[0]];
				++num_norm[indices[1]];
				++num_norm[indices[2]];
			}
			else if (p_mesh_data->size_index == 4)
			{
				uint32_t* indices = (uint32_t*)p_mesh_data->ptr_indices + i * 3;
				++num_norm[indices[0]];
				++num_norm[indices[1]];
				++num_norm[indices[2]];
			}
		}

		for (int i = 0; i < p_mesh_data->num_vertex; ++i)
		{
			intptr_t p_vert = p_mesh_data->ptr_vertices + i * p_mesh_data->vertex_decl.stride;
			vec3f* norm = (vec3f*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Normal]);

			if (num_norm[i] == 0)
			{
				MWARNING(0, "vertex[%d] has no normal\n", i);
				continue;
			}
			*norm = *norm / num_norm[i];
			norm->Normalize();
		}

		free(num_norm);
	}
	else
	{
		MASSERT_MSG(0, "Not yet implemented");
	}
}
MeshData* Mesh::Load(const char* file_name)
{
	MeshData* p_mesh_data = nullptr;
	if (StrUtils_EndsWith(file_name, ".ply"))
	{
		p_mesh_data = Mesh::LoadPly(file_name);
	}

	return p_mesh_data;
}
MeshData* Mesh::LoadPly(const char* file_name)
{
	// get mesh data
	char file_path_buffer[MG_MAX_PATH];
	File_GetAbsolutePath(file_name, file_path_buffer);
	char* mesh_data = nullptr;
	long size = 0;
	if (!File_Read(file_path_buffer, &mesh_data, &size))
	{
		MASSERT_MSG(0, "Failed in read mesh data from file %s", file_name);
		return false;
	}

	// load ply file
	PlyFile* ply_file = ::PlyLoad(mesh_data, size);

	// convert ply to mesh data
	MeshData* p_mesh_data = ConvertPlyToMeshData(ply_file);

	// destroy ply file
	::PlyDestroy(ply_file);

	return p_mesh_data;
}

/*************** GeometryMesh ***************/
MeshData* GeometryMesh::GeneratePlane(float len, float width)
{
	float half_l = len / 2.0f;
	float half_w = width / 2.0f;

	// initialize mesh data
	MeshData *p_mesh_data = new MeshData();
	p_mesh_data->vertex_decl.Add(VertexAttribute::Position, 3, VertexAttributeType::Float);
	p_mesh_data->vertex_decl.Add(VertexAttribute::Normal, 3, VertexAttributeType::Float);
	p_mesh_data->vertex_decl.Add(VertexAttribute::TexCoord0, 2, VertexAttributeType::Float);

	p_mesh_data->num_vertex = 4;
	p_mesh_data->num_index = 6;
	p_mesh_data->size_index = 2;
	p_mesh_data->ptr_vertices = (intptr_t)malloc(p_mesh_data->vertex_decl.stride * p_mesh_data->num_vertex);
	p_mesh_data->ptr_indices = (intptr_t)malloc(p_mesh_data->size_index * p_mesh_data->num_index);

	// fill out mesh data
	/*
	 *  z
	 *  ^
	 *  |
	 *  --->x
	 *
	 *  (-w, l)  3 - 2 (w, l)
	 *			 | / |
	 *  (-w, -l) 0 - 1 (w, -l)
	 *
	 */
	float *pos, *norm, *tex_coord;
	intptr_t p_vert;

	// vertex 0
	p_vert = p_mesh_data->ptr_vertices + 0 * p_mesh_data->vertex_decl.stride;
	pos = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Position]);
	norm = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Normal]);
	tex_coord = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::TexCoord0]);
	pos[0] = -half_w;
	pos[1] = 0.0f;
	pos[2] = -half_l;
	norm[0] = 0.0f;
	norm[1] = 1.0f;
	norm[2] = 0.0f;
	tex_coord[0] = 1.0f;
	tex_coord[1] = 0.0f;

	// vertex 1
	p_vert = p_mesh_data->ptr_vertices + 1 * p_mesh_data->vertex_decl.stride;
	pos = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Position]);
	norm = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Normal]);
	tex_coord = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::TexCoord0]);
	pos[0] = half_w;
	pos[1] = 0.0f;
	pos[2] = -half_l;
	norm[0] = 0.0f;
	norm[1] = 1.0f;
	norm[2] = 0.0f;
	tex_coord[0] = 1.0f;
	tex_coord[1] = 1.0f;

	// vertex 2
	p_vert = p_mesh_data->ptr_vertices + 2 * p_mesh_data->vertex_decl.stride;
	pos = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Position]);
	norm = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Normal]);
	tex_coord = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::TexCoord0]);
	pos[0] = half_w;
	pos[1] = 0.0f;
	pos[2] = half_l;
	norm[0] = 0.0f;
	norm[1] = 1.0f;
	norm[2] = 0.0f;
	tex_coord[0] = 0.0f;
	tex_coord[1] = 1.0f;

	// vertex 3
	p_vert = p_mesh_data->ptr_vertices + 3 * p_mesh_data->vertex_decl.stride;
	pos = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Position]);
	norm = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Normal]);
	tex_coord = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::TexCoord0]);
	pos[0] = -half_w;
	pos[1] = 0.0f;
	pos[2] = half_l;
	norm[0] = 0.0f;
	norm[1] = 1.0f;
	norm[2] = 0.0f;
	tex_coord[0] = 0.0f;
	tex_coord[1] = 0.0f;

	// index
	uint16_t* indices = (uint16_t*)p_mesh_data->ptr_indices;
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	return p_mesh_data;
}
MeshData* GeometryMesh::GenerateSphere(float radius, unsigned int rings, unsigned int segments)
{
	// init mesh data
	MeshData *p_mesh_data = new MeshData();
	p_mesh_data->vertex_decl.Add(VertexAttribute::Position, 3, VertexAttributeType::Float);
	p_mesh_data->vertex_decl.Add(VertexAttribute::Normal, 3, VertexAttributeType::Float);
	p_mesh_data->vertex_decl.Add(VertexAttribute::TexCoord0, 2, VertexAttributeType::Float);

	p_mesh_data->num_vertex = rings * segments;
	p_mesh_data->num_index = rings * segments * 6;
	p_mesh_data->size_index = 2;
	p_mesh_data->ptr_vertices = (intptr_t)malloc(p_mesh_data->vertex_decl.stride * p_mesh_data->num_vertex);
	p_mesh_data->ptr_indices = (intptr_t)malloc(p_mesh_data->size_index * p_mesh_data->num_index);

	// generate vertex data	
	float R = 1.0f / (float)(rings - 1);
	float S = 1.0f / (float)(segments - 1);
	unsigned int r, s;
	int idx = 0;
	for (r = 0; r < rings; r++)
	{
		float theta = -HALF_PI + PI * r * R;
		for (s = 0; s < segments; s++)
		{
			intptr_t p_vert = p_mesh_data->ptr_vertices + idx * p_mesh_data->vertex_decl.stride;

			float* pos = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Position]);
			float* norm = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Normal]);
			float* tex_coord = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::TexCoord0]);

			float phi = 2 * PI * s * S;

			float x = Math::Cos(theta) * Math::Cos(phi);
			float y = Math::Sin(theta);
			float z = Math::Cos(theta) * Math::Sin(phi);

			pos[0] = radius * x;
			pos[1] = radius * y;
			pos[2] = radius * z;
			norm[0] = x;
			norm[1] = y;
			norm[2] = z;
			tex_coord[0] = s * S;
			tex_coord[1] = r * R;

			++idx;
		}
	}

	// generate index data
	idx = 0;
	for (r = 0; r < rings - 1; r++)
	{
		for (s = 0; s < segments - 1; s++)
		{
			uint16_t* indices = (uint16_t*)p_mesh_data->ptr_indices + idx * 6;

			int curRow = r * segments;
			int nextRow = (r + 1) * segments;

			indices[0] = curRow + s;
			indices[1] = nextRow + (s + 1); 
			indices[2] = nextRow + s;
			indices[3] = curRow + s;
			indices[4] = curRow + (s + 1);
			indices[5] = nextRow + (s + 1); 

			++idx;
		}
	}
	
	return p_mesh_data;
}
MeshData* GeometryMesh::GenerateTorus(float outer_radius, float inner_radius, int nsides, int nrings)
{
	// get number of face and vertex
	int faces = nsides * nrings;
	int nVerts = nsides * (nrings + 1);

	// init mesh data
	MeshData *p_mesh_data = new MeshData();
	p_mesh_data->vertex_decl.Add(VertexAttribute::Position, 3, VertexAttributeType::Float);
	p_mesh_data->vertex_decl.Add(VertexAttribute::Normal, 3, VertexAttributeType::Float);
	p_mesh_data->vertex_decl.Add(VertexAttribute::TexCoord0, 2, VertexAttributeType::Float);

	p_mesh_data->num_vertex = nVerts;
	p_mesh_data->num_index = faces * 6;
	p_mesh_data->size_index = 2;
	p_mesh_data->ptr_vertices = (intptr_t)malloc(p_mesh_data->vertex_decl.stride * p_mesh_data->num_vertex);
	p_mesh_data->ptr_indices = (intptr_t)malloc(p_mesh_data->size_index * p_mesh_data->num_index);

	// generate vertex data	
	float ringFactor = (float)(2 * PI / nrings);
	float sideFactor = (float)(2 * PI / nsides);

	int idx = 0;
	for (int ring = 0; ring <= nrings; ring++)
	{
		float u = ring * ringFactor;
		float cu = (float)Math::Cos(u);
		float su = (float)Math::Sin(u);
		for (int side = 0; side < nsides; side++)
		{
			// get pointer
			intptr_t p_vert = p_mesh_data->ptr_vertices + idx * p_mesh_data->vertex_decl.stride;
			float* pos = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Position]);
			float* norm = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Normal]);
			float* tex_coord = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::TexCoord0]);

			float v = side * sideFactor;
			float cv = (float)Math::Cos(v);
			float sv = (float)Math::Sin(v);
			float r = (outer_radius + inner_radius * cv);
			pos[0] = r * cu;
			pos[1] = r * su;
			pos[2] = inner_radius * sv;
			norm[0] = cv * cu * r;
			norm[1] = cv * su * r;
			norm[2] = sv * r;
			tex_coord[0] = (float)(u / (2 * PI));
			tex_coord[1] = (float)(v / (2 * PI));
			// Normalize
			float len = (float)Math::Sqrt(norm[0] * norm[0] +
				norm[1] * norm[1] +
				norm[2] * norm[2]);
			norm[0] /= len;
			norm[1] /= len;
			norm[2] /= len;
			++idx;
		}
	}

	uint16_t* indices = (uint16_t*)p_mesh_data->ptr_indices;
	idx = 0;
	for (int ring = 0; ring < nrings; ring++)
	{
		int ringStart = ring * nsides;
		int nextRingStart = (ring + 1) * nsides;
		for (int side = 0; side < nsides; side++)
		{
			int nextSide = (side + 1) % nsides;
			// The quad
			indices[idx] = (ringStart + side);
			indices[idx + 1] = (nextRingStart + nextSide);
			indices[idx + 2] = (nextRingStart + side); 
			indices[idx + 3] = ringStart + side;
			indices[idx + 4] = (ringStart + nextSide);
			indices[idx + 5] = nextRingStart + nextSide; 
			idx += 6;
		}
	}

	return p_mesh_data;
}

NS_MUGGLE_END