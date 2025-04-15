#===============================================================================
find_package(spdlog REQUIRED)

add_library(fcdk SHARED
            fcdk/abi.cc
            fcdk/shared_library.cc
            fcdk/logger.cc
            fcdk/basic_uri.cc)
target_include_directories(fcdk PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                                       "$<INSTALL_INTERFACE:include>")
target_link_libraries(fcdk PUBLIC dl)
target_link_libraries(fcdk PUBLIC spdlog::spdlog)

add_library(FRATAL::fcdk ALIAS fcdk)

install(TARGETS fcdk EXPORT libfcdkExport LIBRARY)
install(EXPORT libfcdkExport
        DESTINATION lib/cmake/fcdk
        NAMESPACE FRATAL::)
