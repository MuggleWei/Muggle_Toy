#include "ply.h"
#include "base/str_utils.h"

#define PLY_MAX_WORD_IN_LINE 32

char *type_names[] = {
	"invalid",
	"char", "short", "int",
	"uchar", "ushort", "uint",
	"float", "double",
};
int type_size[] = {
	0,
	1, 2, 4, 
	1, 2, 4,
	4, 8
};

int PlyGetPropertyType(char *type_name)
{
	int i;

	for (i = PLY_START_TYPE + 1; i < PLY_END_TYPE; i++)
		if (strcmp(type_name, type_names[i]) == 0)
			return i;

	/* if we get here, we didn't find the type */
	return 0;
}

char* PlyAddElement(PlyFile* ply_file, char* line)
{
	PlyElement *elem;
	char* words[8];
	int word_idx = 0;
	
	// get words in this line
	char* p = StrUtils_SplitLineToWords(line, words, &word_idx, 8);
	if (word_idx != 3)
	{
		MWARNING(0, "Invalid element format");
		return NULL;
	}

	// get a new element
	elem = (PlyElement *)malloc(sizeof(PlyElement));
	memset(elem, 0, sizeof(PlyElement));
	elem->name = words[1];
	elem->num = atoi(words[2]);

	if (ply_file->nelems == 0)
	{
		ply_file->elems = (PlyElement **)malloc(sizeof(PlyElement *));
	}		
	else
	{
		ply_file->elems = (PlyElement **)realloc(ply_file->elems,
			sizeof(PlyElement *) * (ply_file->nelems + 1));
	}
		
	ply_file->elems[ply_file->nelems++] = elem;

	return p;
}

char* PlyAddProperty(PlyFile* ply_file, char* line)
{
	PlyProperty *prop;
	PlyElement *elem;
	char* words[8];
	int word_idx = 0;

	// get words in this line
	char* p = StrUtils_SplitLineToWords(line, words, &word_idx, 8);

	// get a new property
	prop = (PlyProperty *)malloc(sizeof(PlyProperty));
	memset(prop, 0, sizeof(PlyProperty));
	if (strcmp(words[1], "list") == 0)
	{
		/* is a list */
		prop->count_external = PlyGetPropertyType(words[2]);
		prop->external_type = PlyGetPropertyType(words[3]);
		prop->name = words[4];
		prop->is_list = 1;
	}
	else 
	{
		/* not a list */
		prop->external_type = PlyGetPropertyType(words[1]);
		prop->name = words[2];
		prop->is_list = 0;
	}

	/* add this property to the list of properties of the current element */
	elem = ply_file->elems[ply_file->nelems - 1];
	if (elem->nprops == 0)
	{
		elem->props = (PlyProperty **)malloc(sizeof(PlyProperty *));
	}		
	else
	{
		elem->props = (PlyProperty **)realloc(elem->props,
			sizeof(PlyProperty *) * (elem->nprops + 1));
	}

	prop->offset = elem->size;
	prop->size = PlyGetTypeSize(prop->external_type) + PlyGetTypeSize(prop->count_external);
	elem->size += prop->size;

	elem->props[elem->nprops++] = prop;

	return p;
}

char* PlyAddComment(PlyFile* ply_file, char* line)
{
	char *p = NULL, *q = NULL;
	p = line;
	MG_SKIP_TO_NEXT_WORD(p);
	q = p;
	MG_SKIP_TO_LINE_END(q);

	if (*q != '\0')
	{
		char* r = q;		
		MG_SKIP_TO_NEXT_WORD(q);
		*r = '\0';
	}

	if (ply_file->num_comments == 0)
	{
		ply_file->comments = (char **)malloc(sizeof(char *));
	}
	else 
	{
		ply_file->comments = (char **)realloc(ply_file->comments,
			sizeof(char *) * ply_file->num_comments + 1);
	}

	ply_file->comments[ply_file->num_comments++] = p;

	return q;
}

char* PlyParseHeader(PlyFile* ply_file, char* p)
{
	char *q = NULL;
	char* words[PLY_MAX_WORD_IN_LINE];
	int word_idx = 0;

	if (p == NULL)
	{
		MERROR(0, "ply file data is null");
		return NULL;
	}

	// ensure this data represent ply file
	if (*p == '\0')
	{
		MWARNING(0, "try to parse an empty ply file\n");
		return NULL;
	}
	q = p;
	MG_SKIP_TO_LINE_END(q);
	if (strncmp(p, "ply", 3) != 0 || q - p != 3)
	{
		MWARNING(0, "can't find ply magic number\n");
		return NULL;
	}
	MG_SKIP_BLANK_AND_LFCR(q);
	p = q;

	// parse each line
	while (1)
	{
		MG_SKIP_BLANK(p);
		if (*p == '\0')
		{
			break;
		}

		// parse word
		if (strncmp(p, "format", strlen("format")) == 0)
		{
			p = StrUtils_SplitLineToWords(p, words, &word_idx, PLY_MAX_WORD_IN_LINE);

			if (word_idx != 3)
			{
				MWARNING(0, "Failed parse ply header - format");
				return NULL;
			}

			if (strcmp(words[1], "ascii") == 0)
			{
				ply_file->file_type = PLY_ASCII;
			}
			else if (strcmp(words[1], "binary_big_endian") == 0)
			{
				ply_file->file_type = PLY_BINARY_BE;
			}
			else if (strcmp(words[1], "binary_little_endian") == 0)
			{
				ply_file->file_type = PLY_BINARY_LE;
			}
			else
			{
				MWARNING(0, "Failed parse ply header - format");
				return NULL;
			}
				
			ply_file->version = (float)atof(words[2]);
		}
		else if (strncmp(p, "property", strlen("property")) == 0)
		{
			p = PlyAddProperty(ply_file, p);
		}
		else if (strncmp(p, "element", strlen("element")) == 0)
		{
			p = PlyAddElement(ply_file, p);
		}
		else if (strncmp(p, "comment", strlen("comment")) == 0)
		{
			p = PlyAddComment(ply_file, p);
		}
		else if (strncmp(p, "end_header", strlen("end_header")) == 0)
		{
			MG_SKIP_TO_NEXT_WORD(p);
			break;
		}
		else
		{
			q = p;
			MG_SKIP_TO_LINE_END(p);
			if (*p != '\0')
			{
				char* r = p;
				MG_SKIP_TO_NEXT_WORD(p);
				*r = '\0';
			}
			MWARNING(0, "Unrecognized ply keyword in header: %s\n", q);
		}

		// break if arrive data end
		if (p == NULL || *p == '\0')
		{
			break;
		}
	}

	return p;
}

void PlyParseData(PlyFile* ply_file, char* p)
{
	int i = 0, j = 0;

	for (i = 0; i < ply_file->nelems; ++i)
	{
		PlyElement* elem = ply_file->elems[i];
		elem->ptr_data = p;
		if (ply_file->file_type == PLY_ASCII)
		{
			for (j = 0; j < elem->num; ++j)
			{
				MG_SKIP_TO_NEXT_LINE(p);
			}
		}
		else
		{
			p = p + elem->num * elem->size;
		}
	}
}

PlyFile* PlyLoad(char* data, long size)
{
	PlyFile* ply_file = NULL;
	char *p = NULL, *q = NULL;

	// create ply file structure
	ply_file = (PlyFile *)malloc(sizeof(PlyFile));
	memset(ply_file, 0, sizeof(PlyFile));
	ply_file->mesh_data = data;

	// read and parse the file's header
	p = PlyParseHeader(ply_file, data);

	// parse data
	PlyParseData(ply_file, p);

	return ply_file;
}
void PlyDestroy(PlyFile* ply_file)
{
	if (ply_file)
	{
		int i = 0, j = 0, k = 0;

		if (ply_file->mesh_data)
		{
			free(ply_file->mesh_data);
		}

		for (i = 0; i < ply_file->nelems; ++i)
		{
			for (j = 0; j < ply_file->elems[i]->nprops; ++j)
			{
				free(ply_file->elems[i]->props[j]);
			}

			free(ply_file->elems[i]->props);
			free(ply_file->elems[i]);
		}
		free(ply_file->elems);

		free(ply_file->comments);

		free(ply_file);
	}	
}
int PlyGetTypeSize(int type_idx)
{
	return type_size[type_idx];
}