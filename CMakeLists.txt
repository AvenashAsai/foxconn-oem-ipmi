cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

cmake_policy (SET CMP0054 NEW)

option (YOCTO "Use YOCTO depedencies system" OFF)
include (ExternalProject)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

project (foxconn-oem-ipmi CXX)

add_definitions (-DBOOST_ERROR_CODE_HEADER_ONLY)
add_definitions (-DBOOST_SYSTEM_NO_DEPRECATED)
add_definitions (-DBOOST_ALL_NO_LIB)
add_definitions (-DBOOST_NO_RTTI)
add_definitions (-DBOOST_NO_TYPEID)
add_definitions (-DBOOST_ASIO_DISABLE_THREADS)
add_definitions (-DBOOST_COROUTINES_NO_DEPRECATION_WARNING)
add_definitions (-Wno-psabi)

if (NOT YOCTO)
    include_directories (SYSTEM non-yocto)

    configure_file (CMakeLists.txt.in 3rdparty/CMakeLists.txt)
    execute_process (COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rdparty)
    execute_process (COMMAND ${CMAKE_COMMAND} --build .
                     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rdparty)

    set (CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/prefix ${CMAKE_PREFIX_PATH})
    include_directories (${CMAKE_BINARY_DIR}/prefix/include)
    link_directories (${CMAKE_BINARY_DIR}/prefix/lib)

    include_directories (${CMAKE_BINARY_DIR}/sdbusplus-src)
    link_directories (${CMAKE_BINARY_DIR}/sdbusplus-src/.libs)
    include_directories (${CMAKE_BINARY_DIR}/phosphor-logging-src)
    link_directories (${CMAKE_BINARY_DIR}/phosphor-logging-src/.libs)
    include_directories (SYSTEM ${CMAKE_BINARY_DIR}/phosphor-ipmi-host/include)
    include_directories (SYSTEM ${CMAKE_BINARY_DIR}/ipmid/user_channel)

    include_directories (${CMAKE_BINARY_DIR}) # link_directories
                                              # (${CMAKE_BINARY_DIR}/sdbusplus-
                                              # src/.libs)
endif ()

if (YOCTO)
    find_package (PkgConfig REQUIRED)
    pkg_check_modules (LOGGING phosphor-logging REQUIRED)
    include_directories (${LOGGING_INCLUDE_DIRS})
    link_directories (${LOGGING_LIBRARY_DIRS})

    pkg_check_modules (LIBIPMID libipmid REQUIRED)
    include_directories (SYSTEM ${LIBIPMID_INCLUDE_DIRS})
    link_directories (${LIBIPMID_LIBRARY_DIRS})

endif ()

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library (zfxnoemcmds
             SHARED src/systemcommands.cpp src/networkcommands.cpp
             src/fanctrlcommands.cpp src/cpldcommands.cpp
             src/buscommands.cpp src/filefwcommands.cpp
             src/misccommands.cpp src/bioscommands.cpp
	     src/utils.cpp)
set_target_properties (zfxnoemcmds PROPERTIES VERSION "0.1.0")
set_target_properties (zfxnoemcmds PROPERTIES SOVERSION "0")
target_link_libraries (zfxnoemcmds sdbusplus)
target_link_libraries (zfxnoemcmds ipmid)
target_link_libraries (zfxnoemcmds -luserlayer)
target_link_libraries (zfxnoemcmds -lchannellayer)
target_link_libraries (zfxnoemcmds ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (zfxnoemcmds phosphor_logging)

#set (PACKAGE_DIR /usr/share/lcd-debug/)
#set (CONFIG_FILES post_desc.json gpio_desc.json cri_sensors.json)

install (TARGETS zfxnoemcmds DESTINATION lib/ipmid-providers)
#install (FILES ${CONFIG_FILES} DESTINATION ${PACKAGE_DIR})
