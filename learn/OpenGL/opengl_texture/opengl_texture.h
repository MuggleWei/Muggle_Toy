#ifndef __OPENGL_TEXTURE_H__
#define __OPENGL_TEXTURE_H__

#include "base/base.h"

EXTERN_C_BEGIN

MG_DLL void Init();
MG_DLL void Update();
MG_DLL void Render();
MG_DLL void Destroy();

void PrepareData();
void PrepareShader();

void CreateVBO();
void CreateVAO();

EXTERN_C_END

#endif