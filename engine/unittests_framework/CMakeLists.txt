


set (LIBRARY_NAME unittests_framework)

print_status_title(${LIBRARY_NAME})

############################################
# assemble source files and headers
############################################

# set sources
set (SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (SOURCES
	${SOURCES_DIR}/unittest_core.cpp
)

# define public headers
set (LIBRARY_INCLUDE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (HEADERS
	${LIBRARY_INCLUDE_DIR}/json_fixture.h
	${LIBRARY_INCLUDE_DIR}/unittest_core.h
	${JSON_INCLUDE_DIRS}/json.hpp
)
# define private headers (not exposed to users)
set (INTERNAL_HEADERS)

############################################
# define library target
############################################

create_library_target(${LIBRARY_NAME} 
	"${SOURCES}"
	"${HEADERS}"
	"${INTERNAL_HEADERS}"
) 


############################################
# set export header for c linkage
############################################

# generate export header
string (TOUPPER ${LIBRARY_NAME} UNITTEST_FW_LIBRARY_UPPERCASE)
string (TOLOWER ${LIBRARY_NAME} UNITTEST_FW_LIBRARY_LOWCASE)

set (GENERATED_HEADERS ${CMAKE_BINARY_DIR}/export)
set (EXPORT_MACRO_NAME "${UNITTEST_FW_LIBRARY_UPPERCASE}_EXPORT")
set (EXPORT_FILE_NAME "${UNITTEST_FW_LIBRARY_LOWCASE}_EXPORT.h")

include (GenerateExportHeader)
generate_export_header(${LIBRARY_NAME}
		EXPORT_MACRO_NAME ${EXPORT_MACRO_NAME}
		EXPORT_FILE_NAME ${GENERATED_HEADERS}/${EXPORT_FILE_NAME}
)

message("")
message(STATUS "---------------------------------")
message(STATUS "unittest library framework generated export header")
message(STATUS "	EXPORT_FILE_NAME: ${GENERATED_HEADERS}/${EXPORT_FILE_NAME}")
message(STATUS "	EXPORT_MACRO_NAME: ${EXPORT_MACRO_NAME}")
message("")

############################################
# define target interface and include dirs
############################################


set( TARGET_INCLUDE_DIRS_PRIVATE )

set( TARGET_INCLUDE_DIRS_PUBLIC 
	${gtest_SOURCE_DIR}/include
	${JSON_INCLUDE_DIRS}
)

set( TARGET_LINKED_LIBS
	gtest 
	gtest_main
)

target_include_and_link(${LIBRARY_NAME}
	"${TARGET_INCLUDE_DIRS_PRIVATE}"
	"${TARGET_INCLUDE_DIRS_PUBLIC}"
	"${TARGET_LINKED_LIBS}"
)

add_dependencies (${LIBRARY_NAME}  
	${TARGET_LINKED_LIBS}
)


############################################
# set install configuration
############################################

install_target(${LIBRARY_NAME})


############################################
# custom library setup
############################################

set (UNITTEST_FWS_INCLUDE_DIR ${LIBRARY_INCLUDE_DIR} CACHE STRING "is shared library" FORCE)
set (UNITTEST_FW_LIBRARY ${LIBRARY_NAME} CACHE STRING "is shared library" FORCE)
set_target_properties(${LIBRARY_NAME} PROPERTIES FOLDER ${CMAKE_PROJECT_NAME}/unittests)


