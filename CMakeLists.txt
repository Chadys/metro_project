CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
PROJECT(metro)


SET(SOURCE_FILES display.c init_metro.c dijkstra.c main.c user_input.c utils.c)
ADD_EXECUTABLE(metro ${SOURCE_FILES})

SET_TARGET_PROPERTIES(metro
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        )

if (EMSCRIPTEN)
    SET(EMCC_LINKER_FLAGS "--emrun -O2 -s WASM=1 \
                       --preload-file ../tokyo.metro@/tokyo.metro \
                       -s NO_EXIT_RUNTIME=0")
    SET(CMAKE_EXE_LINKER_FLAGS ${EMCC_LINKER_FLAGS})
    SET(CMAKE_EXECUTABLE_SUFFIX ".html")
endif(EMSCRIPTEN)