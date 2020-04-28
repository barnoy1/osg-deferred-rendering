
set (TARGET_NAME unittest_suite_1)

print_status_title(${TARGET_NAME})


############################################
# assemble source files and headers
############################################

# set sources
set (SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (SOURCES
	${SOURCES_DIR}/main.cpp
	${SOURCES_DIR}/common_unittests.cpp
)

# define public headers
set (TARGET_INCLUDE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (HEADERS
	${TARGET_INCLUDE_DIR}/common_fixture.h
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


set( TARGET_INCLUDE_DIRS_PRIVATE 
	${gtest_SOURCE_DIR}/include
	${JSON_INCLUDE_DIRS}
	${UNITTEST_FWS_INCLUDE_DIR}
)


set( TARGET_INCLUDE_DIRS_PUBLIC 
	${UNITTESTS_INCLUDE_DIR}
)

set( TARGET_LINKED_LIBS
	engine
	${UNITTEST_FW_LIBRARY}
	gtest 
	gtest_main
)

target_include_and_link(${TARGET_NAME}
	"${TARGET_INCLUDE_DIRS_PRIVATE}"
	"${TARGET_INCLUDE_DIRS_PUBLIC}"
	"${TARGET_LINKED_LIBS}"
)

add_dependencies (${TARGET_NAME}  
	${TARGET_LINKED_LIBS}
)


############################################
# custom library setup
############################################

set_target_properties(${TARGET_NAME} PROPERTIES FOLDER ${CMAKE_PROJECT_NAME}/unittests)
