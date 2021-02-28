
set(CMAKE_CXX_STANDARD 20)

add_executable(SAELib_FunctorTesting "test.cpp")

target_compile_definitions(SAELib_FunctorTesting 
	INTERFACE SAELIB_CXX_STANDARD=${CMAKE_CXX_STANDARD})

target_link_libraries(SAELib_FunctorTesting PRIVATE SAELib)
add_test("SAELib_FunctorTesting" SAELib_FunctorTesting)
