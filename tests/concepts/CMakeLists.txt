
set(CMAKE_CXX_STANDARD 17)

add_executable(SAELib_ConceptsTesting "concepts_test.cpp")
target_link_libraries(SAELib_ConceptsTesting PRIVATE SAELib)
add_test("SAELib_Concepts_test" SAELib_ConceptsTesting)
