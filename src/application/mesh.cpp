#include "mesh.h"
#include "math/math_base.h"

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

/*************** GeometryMesh ***************/
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
		float cu = Math::Cos(u);
		float su = Math::Sin(u);
		for (int side = 0; side < nsides; side++)
		{
			// get pointer
			intptr_t p_vert = p_mesh_data->ptr_vertices + idx * p_mesh_data->vertex_decl.stride;
			float* pos = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Position]);
			float* norm = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::Normal]);
			float* tex_coord = (float*)(p_vert + p_mesh_data->vertex_decl.offsets[VertexAttribute::TexCoord0]);

			float v = side * sideFactor;
			float cv = Math::Cos(v);
			float sv = Math::Sin(v);
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
			float len = Math::Sqrt(norm[0] * norm[0] +
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
			indices[idx + 1] = (nextRingStart + side);
			indices[idx + 2] = (nextRingStart + nextSide);
			indices[idx + 3] = ringStart + side;
			indices[idx + 4] = nextRingStart + nextSide;
			indices[idx + 5] = (ringStart + nextSide);
			idx += 6;
		}
	}

	return p_mesh_data;
}

NS_MUGGLE_END