message(${CMAKE_CURRENT_SOURCE_DIR})

set (SUBJECT_FOR_TEST_SOURCES_DIR .. )

include_directories(${SUBJECT_FOR_TEST_SOURCES_DIR}/inc)

message("SUBJECT_FOR_TEST_SOURCES_DIR = " ${SUBJECT_FOR_TEST_SOURCES_DIR})

add_executable(hex_gen_test	hex_gen_test.c
	                        ${SUBJECT_FOR_TEST_SOURCES_DIR}/hex_gen.c)

