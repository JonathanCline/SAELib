
add_executable(SAELib_FunctorTesting "test.cpp")
target_link_libraries(SAELib_FunctorTesting PRIVATE SAELib)
add_test("SAELib_FunctorTesting" SAELib_FunctorTesting)
