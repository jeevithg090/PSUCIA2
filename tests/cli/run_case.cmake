# Runs PROGRAM --quiet with INPUT on stdin and compares stdout with EXPECTED.
# Optional EXPECTED_CODE_FILE holds the expected exit code (default 0).
execute_process(
    COMMAND ${PROGRAM} --quiet
    INPUT_FILE ${INPUT}
    OUTPUT_VARIABLE actual
    ERROR_VARIABLE stderr
    RESULT_VARIABLE code)

set(expected_code 0)
if(EXISTS ${EXPECTED_CODE_FILE})
    file(READ ${EXPECTED_CODE_FILE} expected_code)
    string(STRIP "${expected_code}" expected_code)
endif()

file(READ ${EXPECTED} expected)
# Normalise line endings so the cases also pass on Windows.
string(REPLACE "\r\n" "\n" actual "${actual}")
string(REPLACE "\r\n" "\n" expected "${expected}")

if(NOT code STREQUAL expected_code)
    message(FATAL_ERROR "exit code ${code}, expected ${expected_code}\nstderr:\n${stderr}")
endif()
if(NOT actual STREQUAL expected)
    message(FATAL_ERROR "output mismatch\n--- expected ---\n${expected}--- actual ---\n${actual}--- stderr ---\n${stderr}")
endif()
