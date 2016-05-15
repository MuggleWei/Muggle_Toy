#ifndef __MUGGLE_MESH_H__
#define __MUGGLE_MESH_H__

#include "base/base.h"

NS_MUGGLE_BEGIN

// vertex attribute
#define FOREACH_ENUM_VERTEX_ATTRIBUTE(_) \
	_(Position) \
	_(Normal) \
	_(Tangent) \
	_(bitangent) \
	_(Weight) \
	_(Color0) \
	_(Color1) \
	_(TexCoord0) \
	_(TexCoord1) \
	_(TexCoord2) \
	_(TexCoord3) \
	_(TexCoord4) \
	_(TexCoord5) \
	_(TexCoord6) \
	_(TexCoord7)
ENUM_STRUCT(VertexAttribute, FOREACH_ENUM_VERTEX_ATTRIBUTE)

// vertex attribute type
#define FOREACH_ENUM_VERTEX_ATTRIBUTE_TYPE(_) \
	_(Int8) \
	_(Int16) \
	_(Int32) \
	_(Float)
ENUM_STRUCT(VertexAttributeType, FOREACH_ENUM_VERTEX_ATTRIBUTE_TYPE)

// vertex declare
struct MG_DLL VertexDeclare
{
	void Init();
	void Add(VertexAttribute::Enum attri, uint8_t num, VertexAttributeType::Enum attri_type);

	uint16_t stride;
	int16_t offsets[VertexAttribute::Max];
	uint8_t attri_types[VertexAttribute::Max];
};

// mesh data
struct MG_DLL MeshData
{
	MeshData();
	~MeshData();

	uint32_t num_vertex;
	uint32_t num_index;
	intptr_t ptr_vertices;
	intptr_t ptr_indices;
	VertexDeclare vertex_decl;	
	int16_t size_index;			// size of per index data, int16 or int32
};

// geometry mesh object
class MG_DLL GeometryMesh
{
public:
	static MeshData* GenerateTorus(float outer_radius, float inner_radius, int nsides, int nrings);
};

NS_MUGGLE_END

#endif