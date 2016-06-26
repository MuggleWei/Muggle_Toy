cmake_minimum_required(VERSION 2.8.9)

set(VERSION "1.2.8")

check_include_file(sys/types.h 	HAVE_SYS_TYPES_H)
check_include_file(stdint.h    	HAVE_STDINT_H)
check_include_file(stddef.h    	HAVE_STDDEF_H)
check_include_file(unistd.h 	Z_HAVE_UNISTD_H)

#
# Check to see if we have large file support
#
set(CMAKE_REQUIRED_DEFINITIONS -D_LARGEFILE64_SOURCE=1)

check_type_size(off64_t OFF64_T)
if(HAVE_OFF64_T)
   add_definitions(-D_LARGEFILE64_SOURCE=1)
endif()

#
# Check for fseeko
#
check_function_exists(fseeko HAVE_FSEEKO)
if(NOT HAVE_FSEEKO)
    add_definitions(-DNO_FSEEKO)
endif()

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/zconf.h.cmakein
	${CMAKE_CURRENT_SOURCE_DIR}/zconf.h @ONLY)
		
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#============================================================================
# zlib
#============================================================================
set(ZLIB_PUBLIC_HDRS
    zconf.h
    zlib.h
)
set(ZLIB_PRIVATE_HDRS
    crc32.h
    deflate.h
    gzguts.h
    inffast.h
    inffixed.h
    inflate.h
    inftrees.h
    trees.h
    zutil.h
)
set(ZLIB_SRCS
    adler32.c
    compress.c
    crc32.c
    deflate.c
    gzclose.c
    gzlib.c
    gzread.c
    gzwrite.c
    inflate.c
    infback.c
    inftrees.c
    inffast.c
    trees.c
    uncompr.c
    zutil.c
)

# parse the full version number from zlib.h and include in ZLIB_FULL_VERSION
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/zlib.h _zlib_h_contents)
string(REGEX REPLACE ".*#define[ \t]+ZLIB_VERSION[ \t]+\"([-0-9A-Za-z.]+)\".*"
    "\\1" ZLIB_FULL_VERSION ${_zlib_h_contents})
	
if (zlibUseShared)
	add_library(${zlib} SHARED ${ZLIB_SRCS} ${ZLIB_ASMS} ${ZLIB_DLL_SRCS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})
	set_target_properties(${zlib} PROPERTIES DEFINE_SYMBOL ZLIB_DLL)
	set_target_properties(${zlib} PROPERTIES SOVERSION 1)
	if(NOT CYGWIN)
		# This property causes shared libraries on Linux to have the full version
		# encoded into their final filename.  We disable this on Cygwin because
		# it causes cygz-${ZLIB_FULL_VERSION}.dll to be created when cygz.dll
		# seems to be the default.
		#
		# This has no effect with MSVC, on that platform the version info for
		# the DLL comes from the resource file win32/zlib1.rc
		set_target_properties(${zlib} PROPERTIES VERSION ${ZLIB_FULL_VERSION})
	endif()
	
	if(UNIX)
		# On unix-like platforms the library is almost always called libz
		set_target_properties(${zlib} PROPERTIES OUTPUT_NAME z)
		if(NOT APPLE)
			set_target_properties(${zlib} PROPERTIES LINK_FLAGS "-Wl,--version-script,\"${CMAKE_CURRENT_SOURCE_DIR}/zlib.map\"")
		endif()
	# elseif(zlibUseShared AND WIN32)
	# 	# Creates zlib1.dll when building shared library version
	# 	set_target_properties(${zlib} PROPERTIES SUFFIX "1.dll")
	endif()
else()
	add_library(${zlib} STATIC ${ZLIB_SRCS} ${ZLIB_ASMS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})
	if(UNIX)
		# On unix-like platforms the library is almost always called libz
		set_target_properties(${zlib} PROPERTIES OUTPUT_NAME z)
	else(WIN32)
		set_target_properties(${zlib} PROPERTIES OUTPUT_NAME zlibstatic)
	endif()	
endif()