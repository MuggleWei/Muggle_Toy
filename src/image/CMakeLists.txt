file(GLOB HEADER ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SOURCE_C ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

if (WIN32)
	add_library(${muggle_image_lib} ${MG_LIB_TYPE}
		${HEADER}
		${SOURCE_C}
		${SOURCE_CPP}
	)
else()
	add_library(${muggle_image_lib} ${MG_LIB_TYPE}
		${SOURCE_C}
		${SOURCE_CPP}
	)
endif()
set_target_properties(${muggle_image_lib} PROPERTIES 
	COMPILE_DEFINITIONS "${MG_LIB_MACRO}")
target_link_libraries(${muggle_image_lib} 
	${muggle_base_lib} 
	${zlib}
	${libpng}
)
add_dependencies(${muggle_image_lib} 
	${muggle_base_lib} 
	${zlib}
	${libpng} 
)