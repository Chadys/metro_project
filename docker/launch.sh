#!/bin/bash
source /opt/emsdk/emsdk_env.sh &&
emcmake cmake -H. -Bcmake-build-debug &&
emmake make -j4 -C ./cmake-build-debug &&
emrun --no_browser --hostname 0.0.0.0 --port 8084 --serve_root ./bin .