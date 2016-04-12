file(GLOB HEADER ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SOURCE_C ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	set(platform_folder_name unix)
    file(GLOB SOURCE_CPP_PLATFORM ${CMAKE_CURRENT_SOURCE_DIR}/unix/*.cpp)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
	set(platform_folder_name mac)
    file(GLOB SOURCE_CPP_PLATFORM ${CMAKE_CURRENT_SOURCE_DIR}/mac/*.cpp)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	set(platform_folder_name win)
    file(GLOB SOURCE_CPP_PLATFORM ${CMAKE_CURRENT_SOURCE_DIR}/windows/*.cpp)
endif()

if (WIN32)
	add_library(${muggle_application_lib} ${MG_LIB_TYPE}
		${HEADER}
		${SOURCE_C}
		${SOURCE_CPP}
		${SOURCE_CPP_PLATFORM}
	)
else()
	add_library(${muggle_application_lib} ${MG_LIB_TYPE}
		${SOURCE_C}
		${SOURCE_CPP}
		${SOURCE_CPP_PLATFORM}
	)
endif()	

source_group(sources FILES ${HEADER} ${SOURCE_C} ${SOURCE_CPP})
source_group(sources\\${platform_folder_name} FILES ${SOURCE_CPP_PLATFORM})

set_target_properties(${muggle_application_lib} PROPERTIES 
	COMPILE_DEFINITIONS "${MG_LIB_MACRO}")
target_link_libraries(${muggle_application_lib} 
	${muggle_base_lib}
	${muggle_utility_lib}
	${muggle_render_lib}
)