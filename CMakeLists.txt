cmake_minimum_required(VERSION 3.5)
project(projrpi++ VERSION 1.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to 'Debug' as none was specified.")
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug"
		"Release"
		"MinSizeRel"
		"RelWithDebInfo")
endif()

include(GNUInstallDirs)

add_library(rpi++
	src/GPIO.cpp
	src/I2CDevice.cpp)

target_include_directories(rpi++ PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>/rpi++
    PRIVATE src)

set_target_properties(rpi++ PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(rpi++ PROPERTIES SOVERSION 1)
set_target_properties(rpi++ PROPERTIES CXX_STANDARD 14)
set_target_properties(rpi++ PROPERTIES CXX_STANDARD_REQUIRED ON)

set_property(TARGET rpi++ APPEND_STRING PROPERTY COMPILE_OPTIONS  -pedantic $<$<CONFIG:Debug>:-DDEBUG>)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_property(TARGET rpi++ APPEND_STRING PROPERTY COMPILE_OPTIONS -Wall -Wextra -Werror -pedantic -Wno-unused-lambda-capture)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set_property(TARGET rpi++ APPEND_STRING PROPERTY COMPILE_OPTIONS -Wall -Wextra -Werror -pedantic -Wno-psabi)
endif()

install(TARGETS rpi++ EXPORT librpi++
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
	
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rpi++)

install(EXPORT librpi++ FILE librpi++-config.cmake DESTINATION share/librpi++/cmake)
