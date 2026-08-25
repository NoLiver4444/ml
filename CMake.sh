rm -rf build
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
cmake --build build --target format
cmake --build build --target format-check
cmake --build build --target coverage


cmake --preset debug
cmake --build --preset debug
ctest --preset debug
cmake --build --preset debug --target format
cmake --build --preset debug --target coverage