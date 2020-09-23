if ! [[ -d "build" ]]; then
    mkdir build
fi
cd build
cmake .. -G"MinGW Makefiles" && cmake --build .
