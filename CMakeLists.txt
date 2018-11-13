cmake_minimum_required(VERSION 2.8)

project(opentherm)
message(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)

# The version number.
set (opentherm_VERSION_MAJOR 0)
set (opentherm_VERSION_MINOR 0)
set (opentherm_VERSION_PATCH 3)

set(HEADER_FILES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc")
message(${HEADER_FILES_DIR})

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${HEADER_FILES_DIR}/opentherm_version.h.in"
  "${HEADER_FILES_DIR}/opentherm_version.h"
  )

option(DESKTOP_TEST "Desktop test" ON)

message(STATUS "DESKTOP_TEST is " ${DESKTOP_TEST})

configure_file(
	"${HEADER_FILES_DIR}/config.h.in"
	"${HEADER_FILES_DIR}/config.h"
	@ONLY)

include_directories(${HEADER_FILES_DIR})

add_executable(${PROJECT_NAME} main.c
	                        opentherm.c
				opentherm_json.c
				opentherm_test.c
#				opentherm_simulator.c
                                message_table.c
				opentherm_master.c
				opentherm_slave.c
				hex_gen.c
				ftoa.c)
target_include_directories(${PROJECT_NAME} PRIVATE ${HEADER_FILES_DIR})

