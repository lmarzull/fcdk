#===============================================================================
find_package(spdlog REQUIRED)
add_library(fcdk SHARED
            fcdk/abi.cc
            fcdk/shared_library.cc
            fcdk/logger.cc
            fcdk/basic_uri.cc)
add_library(fcdk::fcdk ALIAS fcdk)
target_include_directories(fcdk PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                                       "$<INSTALL_INTERFACE:include>")
target_link_libraries(fcdk PUBLIC dl)
target_link_libraries(fcdk PUBLIC spdlog::spdlog)

install(TARGETS fcdk
        EXPORT fcdkTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
      )

install(EXPORT fcdkTargets
        FILE   fcdkTargets.cmake
        NAMESPACE fcdk::
        DESTINATION lib/cmake/fcdk
)
