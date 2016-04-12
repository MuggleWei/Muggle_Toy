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

# add subdirectory
add_subdirectory(src)
add_subdirectory(sample)