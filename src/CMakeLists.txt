#===============================================================================

ADD_LIBRARY(fcdk SHARED
            fcdk/abi.cc
            fcdk/SharedLibrary.cc)
ADD_LIBRARY(FRATAL::fcdk ALIAS fcdk)

INSTALL(TARGETS fcdk EXPORT libfcdkExport LIBRARY)

TARGET_INCLUDE_DIRECTORIES(fcdk PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                                       "$<INSTALL_INTERFACE:include>")

INSTALL(EXPORT libfcdkExport
        DESTINATION share/CMAKE_CURRENT_SOURCE_DIR
        NAMESPACE FRATAL::)
