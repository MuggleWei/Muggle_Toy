#include "math_utils.h"

NS_MUGGLE_BEGIN

bool g_math_use_gl;

void SetMathUseGL(bool flag)
{
	g_math_use_gl = flag;
}
bool GetMathUseGL()
{
	return g_math_use_gl;
}

NS_MUGGLE_END