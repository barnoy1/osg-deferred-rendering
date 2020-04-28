cmake_minimum_required (VERSION 3.11)
project (engine VERSION 1.0.0.0)

#set compiler settings
include (${CMAKE_SOURCE_DIR}/cmake/compiler_settings.cmake)

#set cmake project configuration for this project
include (${CMAKE_SOURCE_DIR}/cmake/configuration.cmake)

#set cmake project configuration for this project
include (${CMAKE_SOURCE_DIR}/cmake/utils.cmake)

############################################
# core library with unittests and executable
############################################
add_subdirectory (${CMAKE_PROJECT_NAME})