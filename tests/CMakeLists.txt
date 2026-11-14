# Unit tests: one binary covering the library (arrays, matrix, strings, cli).
add_executable(unit_tests
    unit/main.cpp
    unit/test_arrays.cpp
    unit/test_matrix.cpp
    unit/test_strings.cpp
    unit/test_cli.cpp)
target_link_libraries(unit_tests PRIVATE psuc)
set_target_properties(unit_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
add_test(NAME unit_tests COMMAND unit_tests)

# End-to-end tests: every tests/cli/<program>/<case>.in is piped into
# <program> --quiet and its stdout + exit code compared with <case>.out
# (and <case>.code when the expected exit code is non-zero).
file(GLOB PSUC_CLI_CASES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cli/*/*.in)
foreach(input IN LISTS PSUC_CLI_CASES)
    get_filename_component(case_name ${input} NAME_WE)
    get_filename_component(case_dir ${input} DIRECTORY)
    get_filename_component(program ${case_dir} NAME)
    if(NOT TARGET ${program})
        message(FATAL_ERROR "tests/cli/${program} has no matching program")
    endif()
    add_test(NAME cli.${program}.${case_name}
        COMMAND ${CMAKE_COMMAND}
            -DPROGRAM=$<TARGET_FILE:${program}>
            -DINPUT=${input}
            -DEXPECTED=${case_dir}/${case_name}.out
            -DEXPECTED_CODE_FILE=${case_dir}/${case_name}.code
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cli/run_case.cmake)
endforeach()

# Every program must answer --help successfully.
foreach(program IN LISTS PSUC_PROGRAMS)
    add_test(NAME help.${program} COMMAND ${program} --help)
    set_tests_properties(help.${program} PROPERTIES PASS_REGULAR_EXPRESSION "Usage: ${program}")
endforeach()
