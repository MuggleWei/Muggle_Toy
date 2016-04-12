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
	cmd_line.append("-Mode SingleTest ");
	cmd_line.append("-Render D3D11 ");

	_muggle_main_(cmd_line.c_str());

	return 0;
}