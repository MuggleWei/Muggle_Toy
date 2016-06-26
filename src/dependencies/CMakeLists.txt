message("## setup dependencies ")

# glad
message("## Add glad")
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

# zlib and png

# include check
include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)

option(zlibUseShared "use shared zlib" ON)
option(lpngUseShared "use shared lpng" ON)

include_directories(
	zlib
	lpng
)

# zlib
message("## Add zlib")
add_subdirectory(zlib)

# lib png
message("## Add lib png")
add_subdirectory(lpng)
add_dependencies(${libpng} ${zlib})


# set folder
set_target_properties(
	${glad_lib}
	${zlib}
	${libpng}
	PROPERTIES
	FOLDER "Dependencies"
)

message("## Complete dependencies setup")