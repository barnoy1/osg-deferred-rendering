
###############################################
# project configuration
###############################################

# set core library name
set (CMAKE_PROJECT_NAME engine CACHE STRING "library name" FORCE)
set (CMAKE_PROJECT_NAME_SPACE core CACHE STRING "library folder" FORCE)


if (WIN32 AND NOT CYGWIN)
	set (DEF_INSTALL_CMAKE_DIR CMake)
else()
	set (DEF_INSTALL_CMAKE_DIR lib/CMake/${CMAKE_PROJECT_NAME})
endif()

############################################
# set deploy configurations
############################################

# set installation folders
set (INSTALL_LOCATION "${PROJECT_BINARY_DIR}/install" CACHE STRING "install directory" FORCE)
set (INSTALL_LIB_DIR "${INSTALL_LOCATION}/lib" CACHE PATH "Installation of libraries")
set (INSTALL_BIN_DIR "${INSTALL_LOCATION}/bin" CACHE PATH "Installation of executables")
set (INSTALL_INCLUDE_DIR "${INSTALL_LOCATION}/include" CACHE PATH "Installation of includes")


# Make relative path absolute
foreach (p LIB_BIN INCLUDE CMAKE)
	set (var INSTALL_{p}_DIR)
	if (NOT IS_ABSOLUTE "${${var}}")
		set (${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
	endif()
endforeach()

# set path of cmake generated files
set (PROJECT_CMAKE_FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY})

# set rpath to be used when installed
set (CMAKE_INSTALL_RPATH ${INSTALL_LIB_DIR})



############################################
# 3rd party configurations
############################################

# define 3rdparty root directory
if (NOT DEFINED 3RD_PARTY_ROOT)
	set (3RD_PARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/3rdparty" CACHE STRING "third party root" FORCE)
endif()



############################################
# set export header for c linkage
############################################

include (GenerateExportHeader)

string (TOUPPER ${CMAKE_PROJECT_NAME_SPACE} LIBRARIES_NAME_UPPERCASE CACHE STRING "" FORCE)
string (TOLOWER ${CMAKE_PROJECT_NAME_SPACE} LIBRARIES_NAME_LOWCASE CACHE STRING "" FORCE)

set (GENERATED_HEADERS ${CMAKE_BINARY_DIR}/export  CACHE STRING "" FORCE)
set (EXPORT_MACRO_NAME "${LIBRARIES_NAME_UPPERCASE}_EXPORT" CACHE STRING "" FORCE)
set (EXPORT_FILE_NAME "${LIBRARIES_NAME_LOWCASE}_EXPORT.h"  CACHE STRING "" FORCE)


#  set output directories
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


############################################
# generated addional h files
############################################


# adding a version.h file
configure_file(
	"${CMAKE_SOURCE_DIR}/cmake/version.h.in"
	"${GENERATED_HEADERS}/${CMAKE_PROJECT_NAME}_version.h" @ONLY)


############################################
# project options
############################################

# build target as shared lib so the static gtest library need to be shared as well in order to link correctly
option (BUILD_TESTS "build unittests" ON)
option (BUILD_TARGET_LIBRARY_TYPE_AS_SHARED "build target as shared" ON)

if (BUILD_TESTS)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

# boost option
option(BUILD_BOOST_LIBS "build solution with boost libraries" ON)


############################################
# confguration console output
############################################

message("")
message(STATUS "---------------------------------")
message(STATUS "${CMAKE_PROJECT_NAME} info")
message(STATUS "	CMAKE_PROJECT_NAME_SPACE: ${CMAKE_PROJECT_NAME_SPACE}")
message(STATUS "	BUILD_TARGET_LIBRARY_TYPE_AS_SHARED: ${BUILD_TARGET_LIBRARY_TYPE_AS_SHARED}")
message(STATUS "	PROJECT_CMAKE_FILES: ${PROJECT_CMAKE_FILES}")
message(STATUS "	INSTALL_LIB_DIR: ${INSTALL_LIB_DIR}")
message(STATUS "	INSTALL_BIN_DIR: ${INSTALL_BIN_DIR}")
message(STATUS "	INSTALL_INCLUDE_DIR: ${INSTALL_INCLUDE_DIR}")
message(STATUS "	CMAKE_ARCHIVE_OUTPUT_DIRECTORY: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "	CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "	CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message("")


message("")
message(STATUS "---------------------------------")
message(STATUS "${LIBRARY_NAME} configuration")
message(STATUS "	BUILD_BOOST_LIBS: ${BUILD_BOOST_LIBS}")
message(STATUS "	BUILD_TESTS: ${BUILD_TESTS}")
message("")





