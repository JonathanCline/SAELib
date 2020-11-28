cmake_minimum_required (VERSION 3.8)

set(SAE_LIB_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD ${SAE_LIB_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(SAELib 
	VERSION 0.0.1
	DESCRIPTION "Collection of handy tools that I won't feel bad about adding a dependency for"
	HOMEPAGE_URL "https://github.com/JonathanCline/SAELib")

include(CTest)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE "include")

add_subdirectory("codegen")

enable_testing()

add_subdirectory("tests")
