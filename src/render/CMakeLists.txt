file(GLOB HEADER ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SOURCE_C ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

if (${MUGGLE_TOY_USE_DX11})
	if(${CMAKE_SYSTEM_VERSION} LESS 6.2) # version < win8
		include_directories(
			${DirectX_D3DX11_INCLUDE_DIR}
		)
		link_directories(
			${DirectX_D3DX11_INCLUDE_DIR}/../Lib/${MG_ARCHITECTURE}
		)	
	endif()
else()	
	# exclude dx file
	list(REMOVE_ITEM SOURCE_CPP "${CMAKE_CURRENT_SOURCE_DIR}/renderer_d3d11.cpp")
	list(REMOVE_ITEM HEADER "${CMAKE_CURRENT_SOURCE_DIR}/renderer_d3d11.h")
endif()

if (WIN32)
	add_library(${muggle_render_lib} ${MG_LIB_TYPE}
		${HEADER}
		${SOURCE_C}
		${SOURCE_CPP}
	)
else()
	add_library(${muggle_render_lib} ${MG_LIB_TYPE}
		${SOURCE_C}
		${SOURCE_CPP}
	)
endif()	
set_target_properties(${muggle_render_lib} PROPERTIES 
	COMPILE_DEFINITIONS "${MG_LIB_MACRO}")
target_link_libraries(${muggle_render_lib}
	${muggle_base_lib}
	${muggle_utility_lib}
	${glad_lib}
	${muggle_image_lib}
)