find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found.")
else()
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

find_program(CLANG_FORMAT_EXE NAMES "clang-format")
if(NOT CLANG_FORMAT_EXE)
    message(STATUS "clang-format not found.")
else()
    message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")
    file(GLOB_RECURSE FILES ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/src/*.cpp)
    execute_process(
        COMMAND ${CLANG_FORMAT_EXE} --Werror --dry-run --style=file ${FILES}
        RESULT_VARIABLE CLANG_FORMAT_RESULT
        ERROR_VARIABLE CLANG_FORMAT_OUTPUT
    )
    if(NOT CLANG_FORMAT_RESULT EQUAL 0)
        message(STATUS "Formatting errors")
        message(FATAL_ERROR "${CLANG_FORMAT_OUTPUT}")
    else()
        message(DEBUG "${CLANG_FORMAT_OUTPUT}")
    endif()
endif()
