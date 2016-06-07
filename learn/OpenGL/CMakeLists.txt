# resources
file(GLOB_RECURSE LearnOpenGLRes ${CMAKE_CURRENT_SOURCE_DIR}/res_learn_opengl/*.*)

# if (WIN32)
# set(custom_consistent_res_command "${CMAKE_SOURCE_DIR}/consistent_res.bat")
# else()
# set(custom_consistent_res_command "${CMAKE_SOURCE_DIR}/consistent_res.sh")
# endif()
# 
# # keep resources consistent
# if (WIN32)
# add_custom_target(FAKE_LearnOpenGL ALL
# 	COMMAND ${custom_consistent_res_command} ${CMAKE_CURRENT_SOURCE_DIR}/res_learn_opengl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res_learn_opengl
# 	COMMAND ${custom_consistent_res_command} ${CMAKE_CURRENT_SOURCE_DIR}/res_learn_opengl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/res_learn_opengl
# 	COMMAND ${custom_consistent_res_command} ${CMAKE_CURRENT_SOURCE_DIR}/res_learn_opengl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/res_learn_opengl
# )
# else()
# add_custom_target(FAKE_LearnOpenGL ALL
# 	COMMAND ${custom_consistent_res_command} ${CMAKE_CURRENT_SOURCE_DIR}/res_learn_opengl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res_learn_opengl
# )
# endif()

consistent_files(FAKE_LearnOpenGL ${CMAKE_CURRENT_SOURCE_DIR}/res_learn_opengl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} res_learn_opengl)

# add samples
sample_code(opengl_hello Learn/OpenGL)
sample_code(opengl_basic Learn/OpenGL)
sample_code(opengl_simple Learn/OpenGL)
sample_code(opengl_test Learn/OpenGL)
sample_code(opengl_fade_cycle Learn/OpenGL)
sample_code(opengl_lambert Learn/OpenGL)
sample_code(opengl_ads Learn/OpenGL)
sample_code(opengl_two_side Learn/OpenGL)
sample_code(opengl_subroutine Learn/OpenGL)
sample_code(opengl_discard Learn/OpenGL)
sample_code(opengl_multiple_light Learn/OpenGL)
sample_code(opengl_light Learn/OpenGL)