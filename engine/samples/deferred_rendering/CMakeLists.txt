


set (TARGET_NAME deferred_rendering)

print_status_title(${TARGET_NAME})


############################################
# assemble source files and headers
############################################

# set sources
set (SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (SOURCES
	${SOURCES_DIR}/main.cpp
	${SOURCES_DIR}/Utilities.cpp
)

# define public headers
set (TARGET_INCLUDE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set ( HEADERS 
	${TARGET_INCLUDE_DIR}/ColorVisitor.h
	${TARGET_INCLUDE_DIR}/LightSource.h
	${TARGET_INCLUDE_DIR}/Utilities.h
	${TARGET_INCLUDE_DIR}/TangetSpaceComputeVisitor.h
)

# define private headers (not exposed to users)
set (INTERNAL_HEADERS)


############################################
# define library target
############################################

create_executable_target(${TARGET_NAME} 
	"${SOURCES}"
	"${HEADERS}"
	"${INTERNAL_HEADERS}"
) 


############################################
# define target interface and include dirs
############################################


set( TARGET_INCLUDE_DIRS_PRIVATE )


set( TARGET_INCLUDE_DIRS_PUBLIC )

set( TARGET_LINKED_LIBS
	engine
)

target_include_and_link(${TARGET_NAME}
	"${TARGET_INCLUDE_DIRS_PRIVATE}"
	"${TARGET_INCLUDE_DIRS_PUBLIC}"
	"${TARGET_LINKED_LIBS}"
)

add_dependencies (${TARGET_NAME}  
	${TARGET_LINKED_LIBS}
)

set_target_properties(${TARGET_NAME} PROPERTIES FOLDER ${CMAKE_PROJECT_NAME}/sample_apps)


file(COPY "resources" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})