file(GLOB HEADER ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SOURCE_C ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

if (WIN32)
	add_library(${muggle_base_lib} ${MG_LIB_TYPE}
		${HEADER}
		${SOURCE_C}
	)
else()
	add_library(${muggle_base_lib} ${MG_LIB_TYPE}
		${SOURCE_C}
	)
endif()	
set_target_properties(${muggle_base_lib} PROPERTIES 
	COMPILE_DEFINITIONS "${MG_LIB_MACRO}"
	LINKER_LANGUAGE C)
target_link_libraries(${muggle_base_lib} ${CMAKE_DL_LIBS})