## This definition can be done into a do_stuff.cmake into a cmake dir
## Name : do_stuff
## Params: arg1, arg2
## Output: result
function(do_stuff lib_name result)
  set (${result} ${lib_name}_UPDATE PARENT_SCOPE)
endfunction()



## build a target as a library
## Params: LIBRARY_NAME SOURCES HEADERS INTERNAL_HEADERS
## Output: void
function(create_executable_target 
    TARGET_NAME 
    SOURCES 
    HEADERS 
    INTERNAL_HEADERS)

message("")
message(STATUS create_executable_target:)
message(STATUS -------------------------)
message(STATUS TARGET_NAME: ${TARGET_NAME})
message(STATUS SOURCES: ${SOURCES})
message(STATUS HEADERS: ${HEADERS})
message(STATUS INTERNAL_HEADERS: ${INTERNAL_HEADERS})
message(STATUS LIBRARY_TYPE: ${LIBRARY_TYPE})
message("")

# set target library
add_executable(${TARGET_NAME} 	
	# sources and private headers
	${SOURCES}
	${INTERNAL_HEADERS}
	# public interface
	${HEADERS}
)	

endfunction()

## build a target as a library
## Params: LIBRARY_NAME SOURCES HEADERS INTERNAL_HEADERS
## Output: void
function(create_library_target 
    LIBRARY_NAME 
    SOURCES 
    HEADERS 
    INTERNAL_HEADERS)

# set libarary type
if (BUILD_TARGET_LIBRARY_TYPE_AS_SHARED)
    set (LIBRARY_TYPE SHARED)
else()
    set (LIBRARY_TYPE STATIC)
endif()

set (LIBRARY_TYPE ${LIBRARY_TYPE} CACHE STRING "is shared library" FORCE)

message("")
message(STATUS create_library_target:)
message(STATUS ----------------------)
message(STATUS LIBRARY_NAME: ${LIBRARY_NAME})
message(STATUS SOURCES: ${SOURCES})
message(STATUS HEADERS: ${HEADERS})
message(STATUS INTERNAL_HEADERS: ${INTERNAL_HEADERS})
message(STATUS LIBRARY_TYPE: ${LIBRARY_TYPE})
message("")

# set target library
add_library (${LIBRARY_NAME} 	
	# shared or static
	${LIBRARY_TYPE}
	# sources and private headers
	${SOURCES}
	${INTERNAL_HEADERS}
	# public interface
	${HEADERS}
)	

endfunction()


function(target_include_and_link LIBRARY_NAME
	# header directories exposed to library only
	TARGET_INCLUDE_DIRS_PRIVATE
	# header directories (interface) exposed to client via library
	TARGET_INCLUDE_DIRS_PUBLIC
	# linked libraries for this target
	TARGET_LINKED_LIBS)

if (BUILD_BOOST_LIBS)
	list(APPEND TARGET_INCLUDE_DIRS_PRIVATE ${Boost_INCLUDE_DIRS})
	list(APPEND TARGET_LINKED_LIBS ${Boost_LIBRARIES})
endif()

message("")
message(STATUS target_include_and_link:)
message(STATUS ------------------------)
message(STATUS LIBRARY_NAME: ${LIBRARY_NAME})
message(STATUS TARGET_INCLUDE_DIRS_PRIVATE: ${TARGET_INCLUDE_DIRS_PRIVATE})
message(STATUS TARGET_INCLUDE_DIRS_PUBLIC: ${TARGET_INCLUDE_DIRS_PUBLIC})
message(STATUS TARGET_LINKED_LIBS: ${TARGET_LINKED_LIBS})
message("")

# set include directories
target_include_directories(${LIBRARY_NAME}
PUBLIC

	$<INSTALL_INTERFACE:
		include
		export>

	$<BUILD_INTERFACE:
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${GENERATED_HEADERS}
		${TARGET_INCLUDE_DIRS_PUBLIC}>
	
PRIVATE
	${TARGET_INCLUDE_DIRS_PRIVATE}
)

# set link libraries
target_link_libraries(${LIBRARY_NAME}
	${TARGET_LINKED_LIBS}
)
	
endfunction()

function(target_generate_export_header LIBRARY_NAME)

generate_export_header(${LIBRARY_NAME}
		BASE_NAME ${LIBRARIES_NAME_UPPERCASE}
		EXPORT_MACRO_NAME ${EXPORT_MACRO_NAME}
		EXPORT_FILE_NAME ${GENERATED_HEADERS}/${EXPORT_FILE_NAME}
)

message("")
message(STATUS target_generate_export_header:)
message(STATUS ------------------------------)
message(STATUS EXPORT_MACRO_NAME: ${EXPORT_MACRO_NAME})
message(STATUS EXPORT_FILE_NAME: ${EXPORT_FILE_NAME})
message("")

endfunction()

function (install_target ${LIBRARY_NAME})

message("")
message(STATUS install_target:)
message(STATUS ---------------)
message(STATUS DESTINATION: ${INSTALL_BIN_DIR})
message(STATUS DESTINATION: ${INSTALL_BIN_DIR})
message(STATUS DESTINATION: ${INSTALL_LIB_DIR})
message("")

install (TARGETS ${LIBRARY_NAME}
	EXPORT  ${LIBRARY_EXPORT} 
	RUNTIME DESTINATION  "${INSTALL_BIN_DIR}" COMPONENT bin
	LIBRARY DESTINATION  "${INSTALL_BIN_DIR}" COMPONENT shlib
	ARCHIVE DESTINATION  "${INSTALL_LIB_DIR}" COMPONENT stlib
	COMPONENT dev
)

install (FILES	
	${HEADERS}
	${GENERATED_HEADERS}/${EXPORT_FILE_NAME}
	DESTINATION
	${INSTALL_INCLUDE_DIR}/${LIBRARY_NAME}
)


endfunction()