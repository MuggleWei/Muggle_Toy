# glad
if (WIN32)
	add_library(${glad_lib}
		${CMAKE_CURRENT_SOURCE_DIR}/glad/include/glad/glad.h
		${CMAKE_CURRENT_SOURCE_DIR}/glad/include/KHR/khrplatform.h
		${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c
	)
else()
	add_library(${glad_lib}
		${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c
	)
endif()

# set folder
set_target_properties(
	${glad_lib}
	PROPERTIES
	FOLDER "Dependencies"
)