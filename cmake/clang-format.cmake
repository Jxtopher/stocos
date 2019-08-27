## > clang-format ##################################################################
# FIND_PROGRAM(CLANGFORMAT_EXECUTABLE clangformat)
# if (NOT CLANGFORMAT_EXECUTABLE)
#     packageManager("clang-format")
#     FIND_PROGRAM(CLANGFORMAT_EXECUTABLE clangformat)
# endif(NOT CLANGFORMAT_EXECUTABLE)

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)
foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
    string(FIND ${SOURCE_FILE} ${PROJECT_TRDPARTY_DIR} PROJECT_TRDPARTY_DIR_FOUND)
    if (NOT ${PROJECT_TRDPARTY_DIR_FOUND} EQUAL -1)
        list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
    endif ()
endforeach ()

add_custom_target(
        clangformat
        COMMAND ${CLANGFORMAT_EXECUTABLE}
        -style=file
        -i
        ${ALL_SOURCE_FILES}
)
## < clang-format