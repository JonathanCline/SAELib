
add_executable(SAELibCodeGen_Strenum "strenum.cpp" "strenum.h")
target_include_directories(SAELibCodeGen_Strenum PUBLIC ".")
target_link_libraries(SAELibCodeGen_Strenum PUBLIC SAELib)
set_target_properties(SAELibCodeGen_Strenum PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED True)
target_compile_definitions(SAELibCodeGen_Strenum PRIVATE SOURCE_ROOT="${CMAKE_CURRENT_LIST_DIR}/")

install(TARGETS SAELibCodeGen_Strenum DESTINATION "codegen/")
