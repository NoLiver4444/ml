# Первый запуск / после изменения CMake
cmake --preset debug

# После каждого изменения C++
cmake --build --preset debug

# Все тесты
ctest --test-dir build --output-on-failure

# Только текущий Model
./build/ml_tests --gtest_filter="ModelTest.*"

# Форматирование
cmake --build --preset debug --target format

# Иногда проверяем память/UB
cmake --build --preset asan
ctest --test-dir build-asan --output-on-failure