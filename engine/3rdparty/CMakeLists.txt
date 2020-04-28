

############################################
# Import gtest project
############################################

if (BUILD_TESTS)
	add_subdirectory (gtest-1.6.0)
	set_target_properties (gtest_main PROPERTIES FOLDER ${CMAKE_PROJECT_NAME}/${LIBRARY_NAME}/3rdparty/gtest)
	set_target_properties (gtest PROPERTIES FOLDER ${CMAKE_PROJECT_NAME}/${LIBRARY_NAME}/3rdparty/gtest)
endif()

############################################
# boost libraries
############################################

# enable boost by defualt
if (BUILD_BOOST_LIBS)

	message("")
	print_status_title("Boost libraries")
	message(STATUS BOOST_ROOT: "${BOOST_ROOT}")

	set (BOOST_ROOT_PATH "C:/local/boost_1_70_0" CACHE STRING "boost root directory path" FORCE)

	set (BOOST_ROOT ${BOOST_ROOT_PATH} CACHE STRING "boot root directory" FORCE)
	set (Boost_USE_STATIC_LIBS 	ON CACHE BOOL "" FORCE)
	set (Boost_USE_MULTITHREADED ON CACHE BOOL "" FORCE)
	set (Boost_USE_STATIC_RUNTIME OFF CACHE BOOL "" FORCE)
	set (Boost_NO_SYSTEM_PATH ON CACHE BOOL "" FORCE)

	find_package (Boost REQUIRED 
		filesystem
	)
endif()


# Make the boost libararies / include directories cmake variable persistent by saving it to cache for later use
set (Boost_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} CACHE STRING "boost include directories" FORCE)
set (Boost_LIBRARIES ${Boost_LIBRARIES} CACHE STRING "boost libraries" FORCE)


message(STATUS Boost_INCLUDE_DIRS: "${Boost_INCLUDE_DIRS}")
message(STATUS Boost_LIBRARIES: "${Boost_LIBRARIES}")
message(STATUS Boost_USE_STATIC_LIBS: "${Boost_USE_STATIC_LIBS}")
message(STATUS Boost_USE_MULTITHREADED: "${Boost_USE_MULTITHREADED}")
message(STATUS Boost_USE_STATIC_RUNTIME: "${Boost_USE_STATIC_RUNTIME}")
message(STATUS Boost_NO_SYSTEM_PATH: "${Boost_NO_SYSTEM_PATH}")
message("")

############################################
# json libraries
############################################

if (BUILD_TESTS)

	set (JSON_INCLUDE_DIRS "${3RD_PARTY_ROOT}/json" CACHE STRING "json include directories" FORCE)

	message("")
	print_status_title("Json header")
	message(STATUS JSON_INCLUDE_DIRS: "${JSON_INCLUDE_DIRS}")
	message("")
endif()


############################################
# glew
############################################

set (GLEW_INCLUDE_DIRS "${3RD_PARTY_ROOT}/glew/include" CACHE STRING "glew include directories" FORCE)

set (GLEW_LIBRARIES_DIR "${3RD_PARTY_ROOT}/glew/lib")
set (GLEW_LIBRARIES 
	${GLEW_LIBRARIES_DIR}/glew32.lib 
	${GLEW_LIBRARIES_DIR}/glew32s.lib 
CACHE STRING "glew include libraries" FORCE)


message("")
print_status_title("glew libraries")
message(STATUS GLEW_INCLUDE_DIRS: "${GLEW_INCLUDE_DIRS}")
message(STATUS GLEW_LIBRARIES: "${GLEW_LIBRARIES}")
message("")


############################################
# OpenSceneGraph
############################################

message("")
print_status_title("Openscenegraph libraries")

# OSG_DIR is set to C:/Repo/OpenSceneGraph/build
set (OSG_DIR "$ENV{OSG_DIR}" CACHE STRING "root OpenSceneGraph directory path" FORCE)
set (OSG_FILE_PATH "${OSG_DIR}/data" CACHE STRING "OpenSceneGraph data directory path" FORCE)
set (OSG_BIN_PATH "${OSG_DIR}/bin" CACHE STRING "OpenSceneGraph bin path" FORCE)
set (OSG_PLUGIN_3RDPARTY "${3RD_PARTY_ROOT}/osg" CACHE STRING "OpenSceneGraph 3rd party bin path" FORCE)

find_package(OpenSceneGraph REQUIRED 
	OpenThreads 
	osg
	osgDB 
	osgUtil 
	osgGA	
	osgViewer 
	osgText
)
	
set (OPENSCENEGRAPH_INCLUDE_DIRS ${OPENSCENEGRAPH_INCLUDE_DIRS} CACHE STRING "" FORCE)

set (OPENSCENEGRAPH_LIBRARIES "${OPENSCENEGRAPH_LIBRARIES}" CACHE STRING "" FORCE)
set (OSG_PLUGIN_PATH "${OSG_BIN_PATH}/osgPlugins-${OPENSCENEGRAPH_VERSION}" CACHE STRING "" FORCE)


message(STATUS OSG_DIR: "${OSG_DIR}")
message(STATUS OPENSCENEGRAPH_INCLUDE_DIRS: "${OPENSCENEGRAPH_INCLUDE_DIRS}")
message(STATUS OPENSCENEGRAPH_LIBRARIES: "${OPENSCENEGRAPH_LIBRARIES}")
message(STATUS OPENSCENEGRAPH_FOUND : "${OPENSCENEGRAPH_FOUND}")
message(STATUS OPENSCENEGRAPH_VERSION : "${OPENSCENEGRAPH_VERSION}")
message(STATUS OSG_FILE_PATH: "${OSG_FILE_PATH}")
message(STATUS OSG_BIN_PATH: "${OSG_BIN_PATH}")
message(STATUS OSG_PLUGIN_PATH: "${OSG_PLUGIN_PATH}")
message("")
