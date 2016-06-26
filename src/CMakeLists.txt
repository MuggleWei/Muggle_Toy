cmake_minimum_required (VERSION 2.8.9)
project(MuggleToy)

# set compile parameter
if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra -std=c++11")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra -Wno-missing-field-initializers")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1 -D_UNICODE -DUNICODE)
endif()

# platform macro
if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	add_definitions(-DMG_PLATFORM_UNIX=1)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
	add_definitions(-DMG_PLATFORM_OSX=1)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	add_definitions(-DMG_PLATFORM_WINDOWS=1)
endif()

# set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set use folder in some IDE
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# architecture 32bit or 64bit
set(MG_ARCHITECTURE "")
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	set(MG_ARCHITECTURE x64)
else()
	set(MG_ARCHITECTURE x86)
endif()

# library type
set(MG_LIB_TYPE SHARED)
set(MG_LIB_MACRO MG_USE_DLL)

# include directories
include_directories(
	.
	base
	utility
	image
	render
	application
	dependencies
	dependencies/glad/include
)

# include dependencies 
set(glad_lib glad)

# lib png and zlib output name
if(UNIX)
	set(zlib_name z)
elseif(WIN32)
	if (zlibUseShared)
		set(zlib_name zlib)
	else()
		set(zlib_name zlibstatic)
	endif()
endif()
if (UNIX)
	set(libpng_name png)
else()
	if (lpngUseShared)
		set(libpng_name libpng)
	else()
		set(libpng_name libpng_static)		
	endif()	
endif()

# lib png and zlib target name
set(libpng png)
set(zlib zlib)

add_subdirectory(dependencies)

# all subdirectory
message("## Add muggle base lib")
add_subdirectory(${muggle_base})
message("## Add muggle utility lib")
add_subdirectory(${muggle_utility})
message("## Add muggle image lib")
add_subdirectory(${muggle_image})
message("## Add muggle render lib")
add_subdirectory(${muggle_render})
message("## Add muggle application lib")
add_subdirectory(${muggle_application})

# dependencies
add_dependencies(${muggle_utility_lib} ${muggle_base_lib})
add_dependencies(${muggle_image_lib} ${muggle_utility_lib})
add_dependencies(${muggle_render_lib} ${muggle_image_lib})
add_dependencies(${muggle_application_lib} ${muggle_render_lib} ${muggle_utility_lib} ${muggle_image_lib} ${muggle_base_lib})

set_target_properties(
	${muggle_base_lib}
	${muggle_utility_lib}
	${muggle_image_lib}
	${muggle_render_lib}
	${muggle_application_lib}
	PROPERTIES
	FOLDER "MuggleToy"
)