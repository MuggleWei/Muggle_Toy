cmake_minimum_required (VERSION 2.8.9)
project(MuggleSample)

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

# set muggle toy path and dll name
set(MUGGLE_TOY_PATH "../src")
set(muggle_base base)
set(muggle_utility utility)
set(muggle_render render)
set(muggle_application application)

include_directories(
	${MUGGLE_TOY_PATH}
	${MUGGLE_TOY_PATH}/${muggle_base}
	${MUGGLE_TOY_PATH}/${muggle_utility}
	${MUGGLE_TOY_PATH}/${muggle_render}
	${MUGGLE_TOY_PATH}/${muggle_application}
)

# all muggle toy lib
set(muggle_all_exclude_app
	${muggle_base}
	${muggle_utility}
	${muggle_render}
)
set(muggle_all
	${muggle_all_exclude_app}
	${muggle_application}
)

# function for add sample code
function(sample_code name)
	file(GLOB ${name}_H ${name}/*.h)
	file(GLOB ${name}_C ${name}/*.c)
	file(GLOB ${name}_CPP ${name}/*.cpp)
	
	# exclude main cpp file
	set(${name}_Dll_CPP "")
	foreach (TMP_PATH ${${name}_CPP})
		string (FIND ${TMP_PATH} main.cpp Exclude_File_Found)
		if (NOT ${Exclude_File_Found} EQUAL -1)
			list(REMOVE_ITEM ${name}_CPP ${TMP_PATH})
		endif ()
	endforeach(TMP_PATH)
	
	# add shared lib
	if (WIN32)
		add_library(${name} SHARED
			${${name}_H}
			${${name}_C}
			${${name}_CPP}
		)
	else()
		add_library(${name} SHARED
			${${name}_C}
			${${name}_CPP}
		)
	endif()
	target_link_libraries(${name}
		${muggle_all_exclude_app}
	)
	set_target_properties(${name} PROPERTIES 
		COMPILE_DEFINITIONS MG_USE_DLL
		PREFIX lib
		OUTPUT_NAME ${name})
	
	# add entry executable
	add_executable(${name}_Entry
		${name}/main.cpp
	)
	target_link_libraries(${name}_Entry
		${muggle_all}
		${name}
	)
	
	set_target_properties(
		${name} ${name}_Entry
		PROPERTIES
		FOLDER "Sample"
	)
endfunction(sample_code) 

# add samples
sample_code(hello)


