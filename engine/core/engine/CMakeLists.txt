

############################################
# set library properties
############################################

set (LIBRARY_NAME engine)

print_status_title(${LIBRARY_NAME})

# include library module function
include(${CMAKE_SOURCE_DIR}/cmake/library.config.cmake)

############################################
# assemble source files and headers
############################################


# set sources
set (SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (SOURCES	
	${SOURCES_DIR}/engine.cpp
)
# define public headers
set (LIBRARY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (HEADERS	
	${LIBRARY_INCLUDE_DIR}/engine.h
	${LIBRARY_INCLUDE_DIR}/structs.h
)
# define private headers (not exposed to users)
set (INTERNAL_HEADERS	
)


############################################
# define library target
############################################

create_library_target(${LIBRARY_NAME} 
	"${SOURCES}"
	"${HEADERS}"
	"${INTERNAL_HEADERS}"
) 


############################################
# generate export header for c linkage
############################################
target_generate_export_header(${LIBRARY_NAME})


######################################################################
# define target interface and include dirs (including generated files)
######################################################################

set( TARGET_INCLUDE_DIRS_PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

set( TARGET_INCLUDE_DIRS_PUBLIC 
	${GLEW_INCLUDE_DIRS}
	${OPENSCENEGRAPH_INCLUDE_DIRS}
)

set( TARGET_LINKED_LIBS
	${GLEW_LIBRARIES_DIR}
	${OPENSCENEGRAPH_LIBRARIES}
)

target_include_and_link(${LIBRARY_NAME}
	"${TARGET_INCLUDE_DIRS_PRIVATE}"
	"${TARGET_INCLUDE_DIRS_PUBLIC}"
	"${TARGET_LINKED_LIBS}"
)


############################################
# set install configuration
############################################

install_target(${LIBRARY_NAME})



############################################
# custom library section
############################################


# set library folder in solution
set_target_properties (${LIBRARY_NAME} PROPERTIES FOLDER ${CMAKE_PROJECT_NAME}/${CMAKE_PROJECT_NAME_SPACE})


# copy one file to output direcory using cmake copy
add_custom_command (TARGET ${LIBRARY_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_SOURCE_DIR}/README.md"
	$<TARGET_FILE_DIR:${LIBRARY_NAME}>
)

file(GLOB OSG_BIN 
	"${OSG_BIN_PATH}/*.dll"
	"${OSG_BIN_PATH}/*.pdb"
	"${OSG_PLUGIN_3RDPARTY}/*.dll")

file(COPY ${OSG_PLUGIN_PATH} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
file(COPY ${OSG_BIN} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
