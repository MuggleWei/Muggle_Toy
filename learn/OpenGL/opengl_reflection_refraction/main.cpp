#include "base.h"
#include "application.h"
#include <string>

extern int _muggle_main_(const char* cmd_line);

int main(int argc, char* argv[])
{
	// system command line
	std::string cmd_line = "";
	for (int i = 0; i < argc; ++i)
	{
		cmd_line.append(argv[i]);
		cmd_line.append(" ");
	}

	// hello command line
	cmd_line.append("--Mode RawTest ");
	cmd_line.append("--RenderType OpenGL ");
	cmd_line.append("--RenderVersion.major 4 ");
	cmd_line.append("--RenderVersion.minor 4 ");
	cmd_line.append("--project opengl_reflection_refraction ");
	cmd_line.append("--vsync on ");
	cmd_line.append("--lockfps off ");
	cmd_line.append("--lockfps.value 33.333333");

	_muggle_main_(cmd_line.c_str());

	return 0;
}