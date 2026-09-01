cmake --preset debug
cmake --build --preset debug
ctest --test-dir build --output-on-failure

cmake --preset asan
cmake --build --preset asan
ctest --test-dir build-asan --output-on-failure

cmake --preset tidy
cmake --build --preset tidy

cmake --preset coverage
cmake --build --preset coverage --target coverage

cmake --build --preset debug --target format