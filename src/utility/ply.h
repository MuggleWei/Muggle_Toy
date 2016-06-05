#ifndef __MUGGLE_PLY_H__
#define __MUGGLE_PLY_H__

#include "base/base.h"

#define PLY_ASCII      1        /* ascii PLY file */
#define PLY_BINARY_BE  2        /* binary PLY file, big endian */
#define PLY_BINARY_LE  3        /* binary PLY file, little endian */

#define PLY_OKAY    0           /* ply routine worked okay */
#define PLY_ERROR  -1           /* error in ply routine */

/* scalar data types supported by PLY format */
#define PLY_START_TYPE 0
#define PLY_CHAR       1
#define PLY_SHORT      2
#define PLY_INT        3
#define PLY_UCHAR      4
#define PLY_USHORT     5
#define PLY_UINT       6
#define PLY_FLOAT      7
#define PLY_DOUBLE     8
#define PLY_END_TYPE   9

#define PLY_SCALAR  0
#define PLY_LIST    1

typedef struct PlyProperty_tag 
{
	/* description of a property */
	char *name;                           /* property name */
	int external_type;                    /* file's data type */
	int internal_type;                    /* program's data type */
	int offset;                           /* offset bytes of prop in a struct */
	int size;							// size in bytes

	int is_list;                          /* 1 = list, 0 = scalar */
	int count_external;                   /* file's count type */
	int count_internal;                   /* program's count type */
	int count_offset;                     /* offset byte for list count */

} PlyProperty;

typedef struct PlyElement_tag
{
	/* description of an element */
	char *name;                   /* element name */
	int num;                      /* number of elements in this object */
	int size;                     /* size of element (bytes) or -1 if variable */
	int nprops;                   /* number of properties for this element */
	PlyProperty **props;          /* list of properties in the file */
	char *store_prop;             /* flags: property wanted by user? */
	int other_offset;             /* offset to un-asked-for props, or -1 if none*/
	int other_size;               /* size of other_props structure */
	void* ptr_data;				// pointer to data
} PlyElement;

typedef struct PlyOtherProp_tag
{
	/* describes other properties in an element */
	char *name;                   /* element name */
	int size;                     /* size of other_props */
	int nprops;                   /* number of properties in other_props */
	PlyProperty **props;          /* list of properties in other_props */
} PlyOtherProp;

typedef struct OtherData_tag 
{
	/* for storing other_props for an other element */
	void *other_props;
} OtherData;

typedef struct OtherElem_tag
{
	/* data for one "other" element */
	char *elem_name;             /* names of other elements */
	int elem_count;              /* count of instances of each element */
	OtherData **other_data;      /* actual property data for the elements */
	PlyOtherProp *other_props;   /* description of the property data */
} OtherElem;

typedef struct PlyOtherElems_tag
{
	/* "other" elements, not interpreted by user */
	int num_elems;                /* number of other elements */
	OtherElem *other_list;        /* list of data for other elements */
} PlyOtherElems;

typedef struct PlyFile_tag
{
	/* description of PLY file */
	char* mesh_data;			// mesh data
	int file_type;                /* ascii or binary */
	float version;                /* version number of file */
	int nelems;                   /* number of elements of object */
	PlyElement **elems;           /* list of elements */
	int num_comments;             /* number of comments */
	char **comments;              /* list of comments */
	int num_obj_info;             /* number of items of object information */
	char **obj_info;              /* list of object info items */
	PlyElement *which_elem;       /* which element we're currently writing */
	PlyOtherElems *other_elems;   /* "other" elements from a PLY file */
} PlyFile;

typedef struct PlyVertex_tag
{
	float x, y, z;             /* the usual 3-space position of a vertex */
} PlyVertex;

typedef struct PlyFace_tag
{
	unsigned char intensity; /* this user attaches intensity to faces */
	unsigned char nverts;    /* number of vertex indices in list */
	int *verts;              /* vertex index list */
} PlyFace;

EXTERN_C_BEGIN

MG_DLL PlyFile* PlyLoad(char* data, long size);
MG_DLL void PlyDestroy(PlyFile* ply_file);
MG_DLL int PlyGetTypeSize(int type_idx);

EXTERN_C_END

#endif