cmake_minimum_required (VERSION 2.8.9)
project(Muggle)

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

# set cmake module path
set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
message("## start search dx package")
find_package(DirectX)
message("## finish search dx package")

# Graphic API macro
if (${DirectX_D3DX11_FOUND})
	set(MUGGLE_TOY_USE_DX11 ON)
	add_definitions(-DMG_ENABLE_DX11=1)
else()
	set(MUGGLE_TOY_USE_DX11 OFF)
	add_definitions(-DMG_ENABLE_DX11=0)
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

# option
option(MUGGLE_TOY_BUILD_SAMPLE "Build sample code" ON)
option(MUGGLE_TOY_BUILD_LEARN "Build learn code" ON)

# set muggle toy path and dll name
set(MUGGLE_TOY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(muggle_base base)
set(muggle_math math)
set(muggle_image image)
set(muggle_utility utility)
set(muggle_render render)
set(muggle_application application)

set(muggle_base_lib muggle_base)
set(muggle_math_lib muggle_math)
set(muggle_utility_lib muggle_utility)
set(muggle_image_lib muggle_image)
set(muggle_render_lib muggle_render)
set(muggle_application_lib muggle_application)

# add muggle toy libs
add_subdirectory(src)

set(sample_include_dir
	${MUGGLE_TOY_PATH}
	${MUGGLE_TOY_PATH}/${muggle_base}
	${MUGGLE_TOY_PATH}/${muggle_math}
	${MUGGLE_TOY_PATH}/${muggle_utility}
	${MUGGLE_TOY_PATH}/${muggle_image}
	${MUGGLE_TOY_PATH}/${muggle_render}
	${MUGGLE_TOY_PATH}/${muggle_application}
	${MUGGLE_TOY_PATH}/dependencies
	${MUGGLE_TOY_PATH}/dependencies/glad/include
)

# all muggle toy lib
set(muggle_all_exclude_app
	${muggle_base_lib}
	${muggle_utility_lib}
	${muggle_image_lib}
	${muggle_render_lib}
)
set(muggle_all
	${muggle_all_exclude_app}
	${muggle_application_lib}
)

# function for add sample code
function(sample_code name folder_name)
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
	target_include_directories(${name} PUBLIC
		${sample_include_dir}
	)
	target_link_libraries(${name}
		${muggle_all}
	)
	set_target_properties(${name} PROPERTIES 
		COMPILE_DEFINITIONS MG_USE_DLL
		PREFIX lib
		OUTPUT_NAME ${name})
	
	# add entry executable
	add_executable(${name}_Entry
		${name}/main.cpp
	)
	target_include_directories(${name}_Entry PUBLIC
		${sample_include_dir}
	)
	target_link_libraries(${name}_Entry
		${muggle_all}
		${name}
	)
	
	set_target_properties(
		${name} ${name}_Entry
		PROPERTIES
		FOLDER "${folder_name}/${name}"
	)
	
	add_dependencies(${name}_Entry ${name})
	add_dependencies(${name} ${muggle_all})
endfunction(sample_code) 

# function for keep files consistent
function(consistent_files fake_proj_name source_dir target_dir target_name)	
	if (WIN32)
		set(custom_consistent_res_command "${CMAKE_SOURCE_DIR}/sync_dir.bat")
	else()
		set(custom_consistent_res_command "${CMAKE_SOURCE_DIR}/sync_dir.sh")
	endif()

	# keep resources consistent
	if (WIN32)
		add_custom_target(${fake_proj_name} ALL
			COMMAND ${custom_consistent_res_command} ${source_dir} ${target_dir}/${target_name}
			COMMAND ${custom_consistent_res_command} ${source_dir} ${target_dir}/Debug/${target_name}
			COMMAND ${custom_consistent_res_command} ${source_dir} ${target_dir}/Release/${target_name}
		)
	else()
		add_custom_target(${fake_proj_name} ALL
			COMMAND ${custom_consistent_res_command} ${source_dir} ${target_dir}/${target_name}
		)
	endif()
	
	set_target_properties(
		${fake_proj_name}
		PROPERTIES
		FOLDER "FAKE"
	)
endfunction(consistent_files) 

# add sample and learn code
if (${MUGGLE_TOY_BUILD_SAMPLE})
	add_subdirectory(sample)
endif()
if (${MUGGLE_TOY_BUILD_LEARN})
	add_subdirectory(learn)
endif()