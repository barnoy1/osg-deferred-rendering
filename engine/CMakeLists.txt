

############################################
# 3rd party
############################################


print_title(3rdparty)
add_subdirectory (3rdparty)


############################################
# libraries source files
############################################


print_title(core libraries)
add_subdirectory (${CMAKE_PROJECT_NAME_SPACE}/engine)

############################################
# sample native application
############################################
print_title(samples)
add_subdirectory (samples)


############################################
# unittests
############################################

if (BUILD_TESTS)
	print_title(unittest)
	add_subdirectory (unittests_framework)
	add_subdirectory (unittests)
endif()
