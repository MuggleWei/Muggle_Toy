option(MUGGLE_TOY_BUILD_LEARN_OPENGL "Build sample code" ON)
option(MUGGLE_TOY_BUILD_LEARN_D3D11 "Build sample code" ON)

if (${MUGGLE_TOY_BUILD_LEARN_OPENGL})
	add_subdirectory(OpenGL)
endif()
if (${MUGGLE_TOY_BUILD_LEARN_D3D11})
	add_subdirectory(D3D11)
endif()