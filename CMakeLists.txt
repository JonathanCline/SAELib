cmake_minimum_required (VERSION 3.8)

option(SAELIB_BUILD_TESTS ON)

project(SAELib 
	VERSION 0.0.1
	DESCRIPTION "Collection of handy tools that I won't feel bad about adding a dependency for"
	HOMEPAGE_URL "https://github.com/JonathanCline/SAELib")

include(CTest)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE "include")

if(NOT CMAKE_CXX_STANDARD)
	message(STATUS "c++ standard not defined")
endif()

target_compile_definitions(${PROJECT_NAME} 
	INTERFACE SAELIB_CXX_STANDARD=${CMAKE_CXX_STANDARD})

add_subdirectory("codegen")

enable_testing()

if(SAELIB_BUILD_TESTS)
	add_subdirectory("tests")
endif()
