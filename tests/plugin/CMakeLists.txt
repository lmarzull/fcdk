#===============================================================================
add_library(plugin SHARED test-plugin.cc)
target_include_directories(plugin PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(plugin PUBLIC fcdk)
