# Locate yaml-cpp
#
# This module defines
#  YAMLCPP_FOUND, if false, do not try to link to yaml-cpp
#  YAMLCPP_LIBRARY, where to find yaml-cpp
#  YAMLCPP_INCLUDE_DIR, where to find yaml.h
#
# By default, the dynamic libraries of yaml-cpp will be found. To find the static ones instead,
# you must set the YAMLCPP_STATIC_LIBRARY variable to TRUE before calling find_package(YamlCpp ...).
#
# If yaml-cpp is not installed in a standard path, you can use the YAMLCPP_INSTALL_DIR CMake variable
# to tell CMake where yaml-cpp is.

cmessage(STATUS "Looking for yaml-cpp library...")
#if( YAMLCPP_INSTALL_DIR STREQUAL "" )
#    cmessage(STATUS "User provided YAMLCPP_INSTALL_DIR=${YAMLCPP_INSTALL_DIR}")
#endif()

# attempt to find static library first if this is set
if( YAMLCPP_STATIC_LIBRARY )
    set(YAMLCPP_LIB_FILE libyaml-cpp.a)
endif()

if( YAMLCPP_INSTALL_DIR )
    # find the yaml-cpp include directory
    find_path(YAMLCPP_INCLUDE_DIR yaml-cpp/yaml.h
            PATH_SUFFIXES include
            PATHS
            ${YAMLCPP_INSTALL_DIR}/include/
            )

    # find the yaml-cpp library
    find_library(YAMLCPP_LIBRARY
            NAMES ${YAMLCPP_STATIC} yaml-cpp
            PATH_SUFFIXES lib64 lib
            PATHS
            ${YAMLCPP_INSTALL_DIR}/lib
            ${YAMLCPP_INSTALL_DIR}/lib64
            )
else()
    # find the yaml-cpp include directory
    find_path(YAMLCPP_INCLUDE_DIR yaml-cpp/yaml.h
            PATH_SUFFIXES include
            PATHS
            ~/Library/Frameworks/yaml-cpp/include/
            /Library/Frameworks/yaml-cpp/include/
            /usr/local/include/
            /usr/include/
            /sw/yaml-cpp/         # Fink
            /opt/local/yaml-cpp/  # DarwinPorts
            /opt/csw/yaml-cpp/    # Blastwave
            /opt/yaml-cpp/)

    # find the yaml-cpp library
    find_library(YAMLCPP_LIBRARY
            NAMES ${YAMLCPP_STATIC} yaml-cpp
            PATH_SUFFIXES lib64 lib
            PATHS
            ~/Library/Frameworks
            /Library/Frameworks
            /usr/local
            /usr
            /sw
            /opt/local
            /opt/csw
            /opt)
endif()



# handle the QUIETLY and REQUIRED arguments and set YAMLCPP_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(YAMLCPP DEFAULT_MSG YAMLCPP_INCLUDE_DIR YAMLCPP_LIBRARY)
mark_as_advanced(YAMLCPP_INCLUDE_DIR YAMLCPP_LIBRARY)
