#include "vector_test.h"

TEST(VectorTest, DefaultConstructor) {
    Vector<int> v;

    ASSERT_EQ(v.size(), 0);

    SUCCEED();
}

TEST(VectorTest, ConstructorNSize) {
    Vector<int> v(5);

    ASSERT_EQ(v.size(), 5);

    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 0);
    EXPECT_EQ(v[2], 0);
    EXPECT_EQ(v[3], 0);
    EXPECT_EQ(v[4], 0);
}

TEST(VectorTest, ConstructorNSizeAndValue) {
    Vector<int> v(4, 10);

    ASSERT_EQ(v.size(), 4);

    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 10);
    EXPECT_EQ(v[2], 10);
    EXPECT_EQ(v[3], 10);
}

TEST(VectorTest, ConstructorInitializerList) {
    Vector<int> v{10, 2, 3, 5, 1};

    ASSERT_EQ(v.size(), 5);

    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 5);
    EXPECT_EQ(v[4], 1);
}

TEST(VectorTest, ConstructorVector) {
    Vector<int> v{10, 2, 3, 5, 1};
    Vector<int> v2(v);

    ASSERT_EQ(v.size(), v2.size());

    EXPECT_EQ(v[0], v2[0]);
    EXPECT_EQ(v[1], v2[1]);
    EXPECT_EQ(v[2], v2[2]);
    EXPECT_EQ(v[3], v2[3]);
    EXPECT_EQ(v[4], v2[4]);
}

TEST(VectorTest, ConstructorEqVector) {
    Vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Vector<int> v2 = v;

    ASSERT_EQ(v.size(), v2.size());

    for (std::size_t i = 0; i < 10; ++i) {
        EXPECT_EQ(v[i], v2[i]);
    }
}

TEST(VectorTest, InitializerListAssignment) {
    Vector<int> v;

    v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    ASSERT_EQ(v.size(), 10);
    for (std::size_t i = 0; i < 10; ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i + 1));
    }
}

TEST(VectorTest, CopyAssignment) {
    Vector<int> v1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Vector<int> v2;

    v2 = v1;

    ASSERT_EQ(v1.size(), v2.size());
    for (std::size_t i = 0; i < v1.size(); ++i) {
        EXPECT_EQ(v1[i], v2[i]);
    }

    v2[0] = 100;
    EXPECT_NE(v1[0], v2[0]);
}

TEST(VectorTest, ConstructorMoveVector) {
    Vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Vector<int> v2(std::move(v));

    ASSERT_EQ(v.size(), 0);
    ASSERT_EQ(v2.size(), 10);

    for (std::size_t i = 0; i < 10; ++i) {
        EXPECT_EQ(v2[i], i + 1);
    }
}

TEST(VectorTest, MoveAssignment) {
    Vector<int> v1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Vector<int> v2;

    v2 = std::move(v1);

    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v1.data(), nullptr);

    ASSERT_EQ(v2.size(), 10);
    for (std::size_t i = 0; i < 10; ++i) {
        EXPECT_EQ(v2[i], static_cast<int>(i + 1));
    }
}

TEST(VectorTest, SelfCopyAssignment) {
    Vector<int> v = {1, 2, 3, 4, 5};

    v = v;

    ASSERT_EQ(v.size(), 5);
    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i + 1));
    }
}

TEST(VectorTest, SelfMoveAssignment) {
    Vector<int> v = {1, 2, 3, 4, 5};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"

    v = std::move(v);

#pragma GCC diagnostic pop

    ASSERT_EQ(v.size(), 5);
    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i + 1));
    }
}

TEST(VectorTest, AssignmentChaining) {
    Vector<int> v1 = {1, 2, 3};
    Vector<int> v2, v3;

    v3 = v2 = v1;

    EXPECT_EQ(v1.size(), v2.size());
    EXPECT_EQ(v2.size(), v3.size());

    for (std::size_t i = 0; i < v1.size(); ++i) {
        EXPECT_EQ(v1[i], v2[i]);
        EXPECT_EQ(v2[i], v3[i]);
    }
}

TEST(VectorTest, AssignmentChangesSize) {
    Vector<int> v1 = {1, 2, 3};
    Vector<int> v2 = {10, 20, 30, 40, 50};

    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v2.size(), 5);

    v1 = v2;

    EXPECT_EQ(v1.size(), 5);
    EXPECT_EQ(v2.size(), 5);

    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(v1[i], v2[i]);
    }
}

TEST(VectorTest, ConstructorEqMoveVector) {
    Vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Vector<int> v2 = std::move(v);

    ASSERT_EQ(v.size(), 0);
    ASSERT_EQ(v2.size(), 10);

    for (std::size_t i = 0; i < 10; ++i) {
        EXPECT_EQ(v2[i], i + 1);
    }
}

TEST(VectorTest, Size) {
    Vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    ASSERT_EQ(v.size(), 10);
}

TEST(VectorTest, Capacity) {
    Vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    ASSERT_EQ(v.capacity(), 10);
}

TEST(VectorTest, EmptyTrue) {
    Vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    EXPECT_EQ(v.empty(), false);
}

TEST(VectorTest, EmptyFalse) {
    Vector<int> v;

    EXPECT_EQ(v.empty(), true);
}

TEST(VectorOperatorIndexTest, OperatorIndexValidAccess) {
    Vector<int> v = {10, 20, 30, 40, 50};

    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
    EXPECT_EQ(v[3], 40);
    EXPECT_EQ(v[4], 50);
}

TEST(VectorOperatorIndexTest, OperatorIndexModifyViaAt) {
    Vector<int> v = {1, 2, 3, 4, 5};

    v[0] = 100;
    v[2] = 300;
    v[4] = 500;

    EXPECT_EQ(v[0], 100);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 300);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 500);
}

TEST(VectorOperatorIndexTest, OperatorIndexConstVersion) {
    const Vector<int> v = {10, 20, 30};

    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(VectorAtTest, AtModifyVia) {
    Vector<int> v = {1, 2, 3, 4, 5};

    v.at(0) = 100;
    v.at(2) = 300;
    v.at(4) = 500;

    EXPECT_EQ(v.at(0), 100);
    EXPECT_EQ(v.at(1), 2);
    EXPECT_EQ(v.at(2), 300);
    EXPECT_EQ(v.at(3), 4);
    EXPECT_EQ(v.at(4), 500);
}

TEST(VectorAtTest, AtOutOfRangeThrows) {
    Vector<int> v = {1, 2, 3};

    EXPECT_THROW(v.at(3), std::out_of_range);
    EXPECT_THROW(v.at(10), std::out_of_range);
    EXPECT_THROW(v.at(100), std::out_of_range);
}

TEST(VectorAtTest, AtEmptyVectorThrows) {
    Vector<int> v;

    EXPECT_THROW(v.at(0), std::out_of_range);
    EXPECT_THROW(v.at(5), std::out_of_range);
}

TEST(VectorAtTest, AtBoundaryConditions) {
    Vector<int> v = {1, 2, 3, 4, 5};

    EXPECT_NO_THROW(v.at(0));
    EXPECT_EQ(v.at(0), 1);

    EXPECT_NO_THROW(v.at(4));
    EXPECT_EQ(v.at(4), 5);

    EXPECT_THROW(v.at(5), std::out_of_range);
}

TEST(VectorAtTest, AtConstVersion) {
    const Vector<int> v = {10, 20, 30};

    EXPECT_EQ(v.at(0), 10);
    EXPECT_EQ(v.at(1), 20);
    EXPECT_EQ(v.at(2), 30);
}

TEST(VectorFrontTest, FrontValidAccess) {
    Vector<int> v = {1, 2, 3};

    EXPECT_EQ(v.front(), 1);
}

TEST(VectorFrontTest, FrontModifyVia) {
    Vector<int> v = {1, 2, 3};

    v.front() = 4;

    EXPECT_EQ(v.front(), 4);
}

TEST(VectorFrontTest, FrontEmptyVectorThrows) {
    Vector<int> v;

    EXPECT_THROW(v.front(), std::out_of_range);
}

TEST(VectorFrontTest, FrontConstVersion) {
    const Vector<int> v = {10, 20, 30};

    EXPECT_EQ(v.front(), 10);
}

TEST(VectorBackTest, BackValidAccess) {
    Vector<int> v = {1, 2, 3};

    EXPECT_EQ(v.back(), 3);
}

TEST(VectorBackTest, BackModifyVia) {
    Vector<int> v = {1, 2, 3};

    v.back() = 4;

    EXPECT_EQ(v.back(), 4);
}

TEST(VectorBackTest, BackEmptyVectorThrows) {
    Vector<int> v;

    EXPECT_THROW(v.back(), std::out_of_range);
}

TEST(VectorBackTest, BackConstVersion) {
    const Vector<int> v = {10, 20, 30};

    EXPECT_EQ(v.back(), 30);
}

TEST(VectorDataTest, ReturnsPointerToData) {
    Vector<int> v = {10, 20, 30};

    int* ptr = v.data();

    ASSERT_NE(ptr, nullptr);

    EXPECT_EQ(ptr[0], 10);
    EXPECT_EQ(ptr[1], 20);
    EXPECT_EQ(ptr[2], 30);

    EXPECT_EQ(ptr, v.data());
}

TEST(VectorDataTest, ModifyThroughPointer) {
    Vector<int> v = {1, 2, 3, 4, 5};

    int* ptr = v.data();

    ptr[0] = 100;
    ptr[2] = 300;
    ptr[4] = 500;

    EXPECT_EQ(v[0], 100);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 300);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 500);
}

TEST(VectorDataTest, EmptyVectorReturnsNullptr) {
    Vector<int> v;

    int* ptr = v.data();

    EXPECT_EQ(ptr, nullptr);
}

TEST(VectorDataTest, AfterClearReturnsNullptr) {
    Vector<int> v = {1, 2, 3, 4, 5};

    v.clear();

    EXPECT_EQ(v.size(), 0);
}

TEST(VectorDataTest, ConstVersion) {
    Vector<int> v = {10, 20, 30};
    const Vector<int>& cv = v;

    const int* ptr = cv.data();

    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(ptr[0], 10);
    EXPECT_EQ(ptr[1], 20);
    EXPECT_EQ(ptr[2], 30);
}

TEST(VectorDataTest, ConstVersionEmpty) {
    const Vector<int> v;

    const int* ptr = v.data();

    EXPECT_EQ(ptr, nullptr);
}

TEST(VectorReserveTest, LowNewCapacity) {
    Vector<int> v{1, 2, 3, 4, 5};

    v.reserve(1);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 5);
}

TEST(VectorReserveTest, EqNewCapacity) {
    Vector<int> v{1, 2, 3, 4, 5};

    v.reserve(5);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 5);
}

TEST(VectorReserveTest, HighNewCapacity) {
    Vector<int> v{1, 2, 3, 4, 5};

    v.reserve(10);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 10);
}

TEST(VectorResizeTest, HighNewCapacity) {
    Vector<int> v{1, 2, 3, 4, 5};

    v.reserve(10);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 10);
}

// 1. Тест: увеличение размера (с значением по умолчанию)
TEST(VectorResizeTest, IncreaseSizeWithDefaultValue) {
    Vector<int> v = {1, 2, 3};

    v.resize(5);

    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 0);  // Значение по умолчанию для int
    EXPECT_EQ(v[4], 0);
}

// 2. Тест: уменьшение размера (обрезание)
TEST(VectorResizeTest, DecreaseSize) {
    Vector<int> v = {1, 2, 3, 4, 5};
    size_t old_capacity = v.capacity();

    v.resize(3);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), old_capacity);  // capacity не меняется
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

// 3. Тест: размер не меняется (new_size == size)
TEST(VectorResizeTest, SameSize) {
    Vector<int> v = {1, 2, 3};
    size_t old_size = v.size();
    size_t old_capacity = v.capacity();

    v.resize(3);

    EXPECT_EQ(v.size(), old_size);
    EXPECT_EQ(v.capacity(), old_capacity);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

// 4. Тест: изменение размера до 0
TEST(VectorResizeTest, ResizeToZero) {
    Vector<int> v = {1, 2, 3, 4, 5};
    size_t old_capacity = v.capacity();

    v.resize(0);

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), old_capacity);  // capacity сохраняется
}

// 5. Тест: resize с реаллокацией
TEST(VectorResizeTest, ResizeWithReallocation) {
    Vector<int> v;
    v.reserve(2);

    EXPECT_EQ(v.capacity(), 2);
    EXPECT_EQ(v.size(), 0);

    v.resize(10);

    EXPECT_EQ(v.size(), 10);
    EXPECT_GE(v.capacity(), 10);

    // Все элементы должны быть инициализированы нулями
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 0);
    }
}

// 6. Тест: resize после push_back
TEST(VectorResizeTest, ResizeAfterPushBack) {
    Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    v.resize(5);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
    EXPECT_EQ(v[3], 0);
    EXPECT_EQ(v[4], 0);
}

// 7. Тест: resize с разными типами (string)
TEST(VectorResizeTest, StringTypeDefaultValue) {
    Vector<std::string> v = {"hello", "world"};

    v.resize(4);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(v[1], "world");
    EXPECT_EQ(v[2], "");  // Пустая строка (значение по умолчанию)
    EXPECT_EQ(v[3], "");
}

// 8. Тест: resize с пользовательским типом
struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x_value, int y_value) : x(x_value), y(y_value) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

TEST(VectorResizeTest, CustomTypeDefaultValue) {
    Vector<Point> v;
    v.push_back(Point(1, 2));
    v.push_back(Point(3, 4));

    v.resize(4);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], Point(1, 2));
    EXPECT_EQ(v[1], Point(3, 4));
    EXPECT_EQ(v[2], Point(0, 0));  // Значение по умолчанию
    EXPECT_EQ(v[3], Point(0, 0));
}

// 9. Тест: несколько resize подряд
TEST(VectorResizeTest, MultipleResizeCalls) {
    Vector<int> v = {1, 2, 3};

    v.resize(5);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[3], 0);
    EXPECT_EQ(v[4], 0);

    v.resize(2);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);

    v.resize(4);
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 0);
    EXPECT_EQ(v[3], 0);
}

// 10. Тест: resize на пустом векторе
TEST(VectorResizeTest, ResizeEmptyVector) {
    Vector<int> v;

    v.resize(5);

    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 0);
    }
}

// 11. Тест: resize и сохранение данных
TEST(VectorResizeTest, DataPreservedAfterResize) {
    Vector<int> v;
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
    }

    v.resize(5);  // Уменьшаем

    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i));
    }

    v.resize(8);  // Увеличиваем

    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i));
    }
    for (size_t i = 5; i < 8; ++i) {
        EXPECT_EQ(v[i], 0);
    }
}

// ============================================
// ТЕСТЫ ДЛЯ resize(std::size_t new_size, const T& value)
// ============================================

// 12. Тест: увеличение размера с заданным значением
TEST(VectorResizeTest, IncreaseSizeWithValue) {
    Vector<int> v = {1, 2, 3};

    v.resize(5, 100);

    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 100);
    EXPECT_EQ(v[4], 100);
}

// 13. Тест: уменьшение размера с заданным значением (игнорируется)
TEST(VectorResizeTest, DecreaseSizeWithValue) {
    Vector<int> v = {1, 2, 3, 4, 5};
    size_t old_capacity = v.capacity();

    v.resize(3, 100);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), old_capacity);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

// 14. Тест: resize до 0 с заданным значением
TEST(VectorResizeTest, ResizeToZeroWithValue) {
    Vector<int> v = {1, 2, 3, 4, 5};

    v.resize(0, 100);

    EXPECT_EQ(v.size(), 0);
}

// 15. Тест: resize с разными значениями
TEST(VectorResizeTest, ResizeWithDifferentValues) {
    Vector<int> v = {10, 20};

    v.resize(4, 999);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 999);
    EXPECT_EQ(v[3], 999);

    v.resize(6, 777);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 999);
    EXPECT_EQ(v[3], 999);
    EXPECT_EQ(v[4], 777);
    EXPECT_EQ(v[5], 777);
}

// 16. Тест: строки с заданным значением
TEST(VectorResizeTest, StringTypeWithValue) {
    Vector<std::string> v = {"hello", "world"};

    v.resize(4, "foo");

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(v[1], "world");
    EXPECT_EQ(v[2], "foo");
    EXPECT_EQ(v[3], "foo");
}

// 17. Тест: пользовательский тип с заданным значением
TEST(VectorResizeTest, CustomTypeWithValue) {
    Vector<Point> v;
    v.push_back(Point(1, 2));
    v.push_back(Point(3, 4));

    v.resize(4, Point(10, 20));

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], Point(1, 2));
    EXPECT_EQ(v[1], Point(3, 4));
    EXPECT_EQ(v[2], Point(10, 20));
    EXPECT_EQ(v[3], Point(10, 20));
}

// 18. Тест: resize с реаллокацией и заданным значением
TEST(VectorResizeTest, ReallocationWithValue) {
    Vector<int> v;
    v.reserve(2);

    v.resize(5, 42);

    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 42);
    }
}

// ============================================================================
// ДОПОЛНИТЕЛЬНЫЕ КРАЕВЫЕ СЛУЧАИ
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor edge cases
// ----------------------------------------------------------------------------

TEST(VectorEdgeConstructorTest, ZeroSizeConstructor) {
    Vector<int> v(0);

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.data(), nullptr);
}

TEST(VectorEdgeConstructorTest, ZeroSizeFillConstructor) {
    Vector<int> v(0, 123);

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.data(), nullptr);
}

TEST(VectorEdgeConstructorTest, EmptyInitializerListConstructor) {
    Vector<int> v{};

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.data(), nullptr);
}

TEST(VectorEdgeConstructorTest, CopyEmptyVector) {
    Vector<int> source;
    Vector<int> copy(source);

    EXPECT_TRUE(copy.empty());
    EXPECT_EQ(copy.size(), 0);
    EXPECT_EQ(copy.capacity(), 0);
    EXPECT_EQ(copy.data(), nullptr);
}

TEST(VectorEdgeConstructorTest, MoveEmptyVector) {
    Vector<int> source;
    Vector<int> moved(std::move(source));

    EXPECT_TRUE(source.empty());
    EXPECT_EQ(source.data(), nullptr);

    EXPECT_TRUE(moved.empty());
    EXPECT_EQ(moved.size(), 0);
    EXPECT_EQ(moved.capacity(), 0);
    EXPECT_EQ(moved.data(), nullptr);
}

// ----------------------------------------------------------------------------
// Assignment edge cases
// ----------------------------------------------------------------------------

TEST(VectorEdgeAssignmentTest, AssignEmptyInitializerListToNonEmptyVector) {
    Vector<int> v{1, 2, 3};

    v = {};

    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
}

TEST(VectorEdgeAssignmentTest, AssignEmptyVectorToNonEmptyVector) {
    Vector<int> source;
    Vector<int> destination{1, 2, 3, 4};

    destination = source;

    EXPECT_TRUE(destination.empty());
    EXPECT_EQ(destination.size(), 0);
}

TEST(VectorEdgeAssignmentTest, MoveAssignIntoNonEmptyVectorDestroysOldContents) {
    Vector<std::string> source{"new-1", "new-2"};
    Vector<std::string> destination{"old-1", "old-2", "old-3"};

    auto* source_data = source.data();

    destination = std::move(source);

    ASSERT_EQ(destination.size(), 2);
    EXPECT_EQ(destination[0], "new-1");
    EXPECT_EQ(destination[1], "new-2");
    EXPECT_EQ(destination.data(), source_data);

    EXPECT_TRUE(source.empty());
    EXPECT_EQ(source.data(), nullptr);
}

// ----------------------------------------------------------------------------
// Reserve edge cases
// ----------------------------------------------------------------------------

TEST(VectorReserveEdgeTest, ReserveZeroOnEmptyVectorDoesNothing) {
    Vector<int> v;

    auto* old_data = v.data();

    v.reserve(0);

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_EQ(v.data(), old_data);
}

TEST(VectorReserveEdgeTest, ReserveZeroOnNonEmptyVectorDoesNothing) {
    Vector<int> v{1, 2, 3};

    auto* old_data = v.data();
    const auto old_capacity = v.capacity();

    v.reserve(0);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), old_capacity);
    EXPECT_EQ(v.data(), old_data);
}

TEST(VectorReserveEdgeTest, ReservePreservesStringValues) {
    Vector<std::string> v{"alpha", "beta", "gamma"};

    v.reserve(64);

    ASSERT_EQ(v.size(), 3);
    EXPECT_GE(v.capacity(), 64);
    EXPECT_EQ(v[0], "alpha");
    EXPECT_EQ(v[1], "beta");
    EXPECT_EQ(v[2], "gamma");
}

TEST(VectorReserveEdgeTest, RepeatedReserveOnlyGrowsCapacity) {
    Vector<int> v{1, 2, 3};

    v.reserve(8);
    const auto first_capacity = v.capacity();
    auto* first_data = v.data();

    v.reserve(4);

    EXPECT_EQ(v.capacity(), first_capacity);
    EXPECT_EQ(v.data(), first_data);

    v.reserve(32);

    EXPECT_GE(v.capacity(), 32);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

// ----------------------------------------------------------------------------
// Resize edge cases
// ----------------------------------------------------------------------------

TEST(VectorResizeEdgeTest, ResizeZeroToZero) {
    Vector<int> v;

    v.resize(0);

    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

TEST(VectorResizeEdgeTest, ResizeZeroToZeroWithValue) {
    Vector<int> v;

    v.resize(0, 42);

    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

TEST(VectorResizeEdgeTest, ResizeWithinExistingCapacityDoesNotReallocate) {
    Vector<int> v{1, 2, 3};

    v.reserve(20);
    auto* old_data = v.data();

    v.resize(10);

    EXPECT_EQ(v.data(), old_data);
    EXPECT_EQ(v.capacity(), 20);
    EXPECT_EQ(v.size(), 10);

    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    for (std::size_t i = 3; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 0);
    }
}

TEST(VectorResizeEdgeTest, ResizeDownThenUpWithinCapacityReusesStorage) {
    Vector<int> v{1, 2, 3, 4, 5};

    v.reserve(20);
    auto* old_data = v.data();

    v.resize(2);
    EXPECT_EQ(v.data(), old_data);
    EXPECT_EQ(v.capacity(), 20);

    v.resize(5);

    EXPECT_EQ(v.data(), old_data);
    EXPECT_EQ(v.capacity(), 20);
    ASSERT_EQ(v.size(), 5);

    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 0);
    EXPECT_EQ(v[3], 0);
    EXPECT_EQ(v[4], 0);
}

TEST(VectorResizeEdgeTest, ResizeDownThenUpWithValueConstructsNewTail) {
    Vector<int> v{1, 2, 3, 4};

    v.reserve(10);
    v.resize(2);
    v.resize(4, 99);

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 99);
    EXPECT_EQ(v[3], 99);
}

// ----------------------------------------------------------------------------
// shrink_to_fit edge cases
// ----------------------------------------------------------------------------

TEST(VectorShrinkToFitEdgeTest, EmptyWithoutCapacityIsNoOp) {
    Vector<int> v;

    v.shrink_to_fit();

    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_EQ(v.data(), nullptr);
}

TEST(VectorShrinkToFitEdgeTest, EmptyReservedVectorReleasesStorage) {
    Vector<int> v;
    v.reserve(32);

    ASSERT_EQ(v.size(), 0);
    ASSERT_GE(v.capacity(), 32);
    ASSERT_NE(v.data(), nullptr);

    v.shrink_to_fit();

    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_EQ(v.data(), nullptr);
}

TEST(VectorShrinkToFitEdgeTest, AlreadyTightVectorDoesNotReallocate) {
    Vector<int> v{1, 2, 3};

    ASSERT_EQ(v.size(), v.capacity());
    auto* old_data = v.data();

    v.shrink_to_fit();

    EXPECT_EQ(v.data(), old_data);
    EXPECT_EQ(v.capacity(), v.size());
}

TEST(VectorShrinkToFitEdgeTest, PreservesValuesAfterShrink) {
    Vector<std::string> v{"one", "two", "three"};

    v.reserve(50);
    ASSERT_GT(v.capacity(), v.size());

    v.shrink_to_fit();

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), 3);
    EXPECT_EQ(v[0], "one");
    EXPECT_EQ(v[1], "two");
    EXPECT_EQ(v[2], "three");
}

// ----------------------------------------------------------------------------
// clear edge cases
// ----------------------------------------------------------------------------

TEST(VectorClearEdgeTest, ClearEmptyVectorIsNoOp) {
    Vector<int> v;

    v.clear();

    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_EQ(v.data(), nullptr);
}

TEST(VectorClearEdgeTest, ClearKeepsReservedStorage) {
    Vector<int> v{1, 2, 3};
    v.reserve(32);

    const auto old_capacity = v.capacity();
    auto* old_data = v.data();

    v.clear();

    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), old_capacity);
    EXPECT_EQ(v.data(), old_data);
}

TEST(VectorClearEdgeTest, VectorCanBeReusedAfterClear) {
    Vector<int> v{1, 2, 3};
    v.reserve(16);

    auto* old_data = v.data();

    v.clear();
    v.push_back(42);

    ASSERT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
    EXPECT_EQ(v.data(), old_data);
}

// ----------------------------------------------------------------------------
// swap edge cases
// ----------------------------------------------------------------------------

TEST(VectorSwapEdgeTest, SwapEmptyWithNonEmpty) {
    Vector<int> empty;
    Vector<int> full{1, 2, 3};

    auto* full_data = full.data();

    empty.swap(full);

    ASSERT_EQ(empty.size(), 3);
    EXPECT_EQ(empty.data(), full_data);
    EXPECT_EQ(empty[0], 1);
    EXPECT_EQ(empty[1], 2);
    EXPECT_EQ(empty[2], 3);

    EXPECT_TRUE(full.empty());
    EXPECT_EQ(full.data(), nullptr);
}

TEST(VectorSwapEdgeTest, SwapTwoEmptyVectors) {
    Vector<int> a;
    Vector<int> b;

    a.swap(b);

    EXPECT_TRUE(a.empty());
    EXPECT_TRUE(b.empty());
    EXPECT_EQ(a.data(), nullptr);
    EXPECT_EQ(b.data(), nullptr);
}

TEST(VectorSwapEdgeTest, SelfSwapKeepsVectorValid) {
    Vector<int> v{1, 2, 3};
    auto* old_data = v.data();

    v.swap(v);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v.data(), old_data);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

// ----------------------------------------------------------------------------
// push_back / pop_back edge cases
// ----------------------------------------------------------------------------

TEST(VectorPushBackEdgeTest, FirstPushBackIntoEmptyVector) {
    Vector<int> v;

    v.push_back(7);

    ASSERT_EQ(v.size(), 1);
    EXPECT_GE(v.capacity(), 1);
    EXPECT_EQ(v[0], 7);
    EXPECT_EQ(v.front(), 7);
    EXPECT_EQ(v.back(), 7);
}

TEST(VectorPushBackEdgeTest, ManyPushBacksPreserveOrderAcrossReallocations) {
    Vector<int> v;

    constexpr int count = 1000;

    for (int i = 0; i < count; ++i) {
        v.push_back(i);
    }

    ASSERT_EQ(v.size(), static_cast<std::size_t>(count));
    EXPECT_GE(v.capacity(), v.size());

    for (int i = 0; i < count; ++i) {
        EXPECT_EQ(v[static_cast<std::size_t>(i)], i);
    }
}

TEST(VectorPushBackEdgeTest, LvaluePushBackCopiesValue) {
    Vector<std::string> v;
    std::string value = "hello";

    v.push_back(value);

    ASSERT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(value, "hello");
}

TEST(VectorPushBackEdgeTest, RvaluePushBackStoresValue) {
    Vector<std::string> v;

    v.push_back(std::string("hello"));

    ASSERT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], "hello");
}

TEST(VectorPopBackEdgeTest, RemoveOnlyElementMakesVectorEmpty) {
    Vector<int> v{42};
    const auto old_capacity = v.capacity();

    v.pop_back();

    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), old_capacity);
}

TEST(VectorPopBackEdgeTest, EmptyVectorThrows) {
    Vector<int> v;

    EXPECT_THROW(v.pop_back(), std::out_of_range);
}

TEST(VectorPopBackEdgeTest, CanPushAgainAfterPoppingEverything) {
    Vector<int> v{1, 2, 3};

    v.pop_back();
    v.pop_back();
    v.pop_back();

    ASSERT_TRUE(v.empty());

    v.push_back(100);

    ASSERT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 100);
}

// ----------------------------------------------------------------------------
// emplace_back edge cases
// ----------------------------------------------------------------------------

struct EmplaceProbe {
    int x;
    std::string text;

    EmplaceProbe(int x_value, std::string text_value) : x(x_value), text(std::move(text_value)) {}
};

TEST(VectorEmplaceBackEdgeTest, ConstructsObjectFromArguments) {
    Vector<EmplaceProbe> v;

    EmplaceProbe& ref = v.emplace_back(7, "seven");

    ASSERT_EQ(v.size(), 1);
    EXPECT_EQ(v[0].x, 7);
    EXPECT_EQ(v[0].text, "seven");
    EXPECT_EQ(&ref, &v[0]);
}

TEST(VectorEmplaceBackEdgeTest, ManyEmplacementsSurviveReallocation) {
    Vector<EmplaceProbe> v;

    for (int i = 0; i < 100; ++i) {
        v.emplace_back(i, std::to_string(i));
    }

    ASSERT_EQ(v.size(), 100);

    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(v[static_cast<std::size_t>(i)].x, i);
        EXPECT_EQ(v[static_cast<std::size_t>(i)].text, std::to_string(i));
    }
}

// ----------------------------------------------------------------------------
// Lifetime tests: проверяют реальные вызовы конструкторов/деструкторов
// ----------------------------------------------------------------------------

struct LifetimeTracker {
    static inline int alive = 0;
    static inline int constructions = 0;
    static inline int destructions = 0;
    static inline int copies = 0;
    static inline int moves = 0;

    int value = 0;

    LifetimeTracker() {
        ++alive;
        ++constructions;
    }

    explicit LifetimeTracker(int initial_value) : value(initial_value) {
        ++alive;
        ++constructions;
    }

    LifetimeTracker(const LifetimeTracker& other) : value(other.value) {
        ++alive;
        ++constructions;
        ++copies;
    }

    LifetimeTracker(LifetimeTracker&& other) noexcept : value(other.value) {
        ++alive;
        ++constructions;
        ++moves;
        other.value = -1;
    }

    LifetimeTracker& operator=(const LifetimeTracker&) = default;
    LifetimeTracker& operator=(LifetimeTracker&&) = default;

    ~LifetimeTracker() {
        --alive;
        ++destructions;
    }

    static void reset() {
        alive = 0;
        constructions = 0;
        destructions = 0;
        copies = 0;
        moves = 0;
    }
};

TEST(VectorLifetimeEdgeTest, DestructorDestroysEveryLiveElement) {
    LifetimeTracker::reset();

    {
        Vector<LifetimeTracker> v(10);
        EXPECT_EQ(LifetimeTracker::alive, 10);
    }

    EXPECT_EQ(LifetimeTracker::alive, 0);
    EXPECT_EQ(LifetimeTracker::constructions, LifetimeTracker::destructions);
}

TEST(VectorLifetimeEdgeTest, ResizeDownDestroysOnlyRemovedTail) {
    LifetimeTracker::reset();

    {
        Vector<LifetimeTracker> v(10);

        ASSERT_EQ(LifetimeTracker::alive, 10);

        v.resize(4);

        EXPECT_EQ(v.size(), 4);
        EXPECT_EQ(LifetimeTracker::alive, 4);
    }

    EXPECT_EQ(LifetimeTracker::alive, 0);
}

TEST(VectorLifetimeEdgeTest, ClearDestroysElementsButNotStorage) {
    LifetimeTracker::reset();

    {
        Vector<LifetimeTracker> v(5);
        v.reserve(20);

        const auto old_capacity = v.capacity();
        auto* old_data = v.data();

        v.clear();

        EXPECT_EQ(LifetimeTracker::alive, 0);
        EXPECT_EQ(v.size(), 0);
        EXPECT_EQ(v.capacity(), old_capacity);
        EXPECT_EQ(v.data(), old_data);
    }

    EXPECT_EQ(LifetimeTracker::alive, 0);
}

TEST(VectorLifetimeEdgeTest, PopBackDestroysExactlyOneElement) {
    LifetimeTracker::reset();

    {
        Vector<LifetimeTracker> v(3);

        ASSERT_EQ(LifetimeTracker::alive, 3);

        v.pop_back();

        EXPECT_EQ(LifetimeTracker::alive, 2);
        EXPECT_EQ(v.size(), 2);
    }

    EXPECT_EQ(LifetimeTracker::alive, 0);
}

TEST(VectorLifetimeEdgeTest, ReserveKeepsExactlySizeLiveObjects) {
    LifetimeTracker::reset();

    {
        Vector<LifetimeTracker> v;
        v.emplace_back(1);
        v.emplace_back(2);
        v.emplace_back(3);

        ASSERT_EQ(v.size(), 3);
        ASSERT_EQ(LifetimeTracker::alive, 3);

        v.reserve(100);

        EXPECT_EQ(v.size(), 3);
        EXPECT_EQ(LifetimeTracker::alive, 3);
        EXPECT_EQ(v[0].value, 1);
        EXPECT_EQ(v[1].value, 2);
        EXPECT_EQ(v[2].value, 3);
    }

    EXPECT_EQ(LifetimeTracker::alive, 0);
    EXPECT_EQ(LifetimeTracker::constructions, LifetimeTracker::destructions);
}

TEST(VectorLifetimeEdgeTest, ShrinkToFitKeepsExactlySizeLiveObjects) {
    LifetimeTracker::reset();

    {
        Vector<LifetimeTracker> v;
        v.emplace_back(10);
        v.emplace_back(20);
        v.emplace_back(30);

        v.reserve(100);
        ASSERT_EQ(LifetimeTracker::alive, 3);

        v.shrink_to_fit();

        EXPECT_EQ(v.size(), 3);
        EXPECT_EQ(v.capacity(), 3);
        EXPECT_EQ(LifetimeTracker::alive, 3);
        EXPECT_EQ(v[0].value, 10);
        EXPECT_EQ(v[1].value, 20);
        EXPECT_EQ(v[2].value, 30);
    }

    EXPECT_EQ(LifetimeTracker::alive, 0);
    EXPECT_EQ(LifetimeTracker::constructions, LifetimeTracker::destructions);
}

TEST(VectorLifetimeEdgeTest, MoveConstructorDoesNotConstructElementsAgain) {
    LifetimeTracker::reset();

    {
        Vector<LifetimeTracker> source;
        source.emplace_back(1);
        source.emplace_back(2);

        const int constructions_before_move = LifetimeTracker::constructions;
        auto* old_data = source.data();

        Vector<LifetimeTracker> destination(std::move(source));

        EXPECT_EQ(destination.data(), old_data);
        EXPECT_EQ(LifetimeTracker::constructions, constructions_before_move);
        EXPECT_EQ(LifetimeTracker::alive, 2);
        EXPECT_TRUE(source.empty());
    }

    EXPECT_EQ(LifetimeTracker::alive, 0);
}

TEST(VectorLifetimeEdgeTest, MoveAssignmentDoesNotMoveElementsIndividually) {
    LifetimeTracker::reset();

    {
        Vector<LifetimeTracker> source;
        source.emplace_back(1);
        source.emplace_back(2);

        Vector<LifetimeTracker> destination;
        destination.emplace_back(99);

        auto* source_data = source.data();
        const int moves_before = LifetimeTracker::moves;

        destination = std::move(source);

        EXPECT_EQ(destination.data(), source_data);
        EXPECT_EQ(LifetimeTracker::moves, moves_before);
        EXPECT_EQ(destination.size(), 2);
        EXPECT_EQ(LifetimeTracker::alive, 2);
        EXPECT_TRUE(source.empty());
    }

    EXPECT_EQ(LifetimeTracker::alive, 0);
}

// ----------------------------------------------------------------------------
// Exception-safety tests
// ----------------------------------------------------------------------------

struct ThrowingCopy {
    static inline int alive = 0;
    static inline int copy_attempts = 0;
    static inline int throw_on_copy_attempt = -1;

    int value = 0;

    ThrowingCopy() {
        ++alive;
    }

    explicit ThrowingCopy(int initial_value) : value(initial_value) {
        ++alive;
    }

    ThrowingCopy(const ThrowingCopy& other) : value(other.value) {
        ++copy_attempts;

        if (throw_on_copy_attempt > 0 && copy_attempts == throw_on_copy_attempt) {
            throw std::runtime_error("copy failed");
        }

        ++alive;
    }

    // Не noexcept специально: std::move_if_noexcept должен предпочесть копирование.
    ThrowingCopy(ThrowingCopy&& other) noexcept(false) : value(other.value) {
        ++alive;
    }

    ThrowingCopy& operator=(const ThrowingCopy&) = default;
    ThrowingCopy& operator=(ThrowingCopy&&) = default;

    ~ThrowingCopy() {
        --alive;
    }

    static void reset() {
        alive = 0;
        copy_attempts = 0;
        throw_on_copy_attempt = -1;
    }
};

TEST(VectorExceptionSafetyTest, CopyConstructorCleansUpPartialConstruction) {
    ThrowingCopy::reset();

    {
        Vector<ThrowingCopy> source;
        source.emplace_back(1);
        source.emplace_back(2);
        source.emplace_back(3);

        ASSERT_EQ(ThrowingCopy::alive, 3);

        ThrowingCopy::copy_attempts = 0;
        ThrowingCopy::throw_on_copy_attempt = 2;

        EXPECT_THROW({ Vector<ThrowingCopy> copy(source); }, std::runtime_error);

        // Исходный вектор обязан остаться полностью живым.
        EXPECT_EQ(source.size(), 3);
        EXPECT_EQ(source[0].value, 1);
        EXPECT_EQ(source[1].value, 2);
        EXPECT_EQ(source[2].value, 3);
        EXPECT_EQ(ThrowingCopy::alive, 3);

        ThrowingCopy::throw_on_copy_attempt = -1;
    }

    EXPECT_EQ(ThrowingCopy::alive, 0);
}

TEST(VectorExceptionSafetyTest, ReserveLeavesOriginalVectorValidWhenCopyThrows) {
    ThrowingCopy::reset();

    {
        Vector<ThrowingCopy> v;
        v.emplace_back(10);
        v.emplace_back(20);
        v.emplace_back(30);

        ASSERT_EQ(v.size(), 3);
        ASSERT_EQ(ThrowingCopy::alive, 3);

        auto* old_data = v.data();
        const auto old_capacity = v.capacity();

        ThrowingCopy::copy_attempts = 0;
        ThrowingCopy::throw_on_copy_attempt = 2;

        EXPECT_THROW(v.reserve(old_capacity + 100), std::runtime_error);

        // При strong exception guarantee старый буфер и значения остаются прежними.
        EXPECT_EQ(v.data(), old_data);
        EXPECT_EQ(v.capacity(), old_capacity);
        ASSERT_EQ(v.size(), 3);
        EXPECT_EQ(v[0].value, 10);
        EXPECT_EQ(v[1].value, 20);
        EXPECT_EQ(v[2].value, 30);
        EXPECT_EQ(ThrowingCopy::alive, 3);

        ThrowingCopy::throw_on_copy_attempt = -1;
    }

    EXPECT_EQ(ThrowingCopy::alive, 0);
}

TEST(VectorExceptionSafetyTest, ShrinkToFitLeavesOriginalVectorValidWhenCopyThrows) {
    ThrowingCopy::reset();

    {
        Vector<ThrowingCopy> v;
        v.emplace_back(1);
        v.emplace_back(2);
        v.emplace_back(3);

        v.reserve(20);
        ASSERT_GT(v.capacity(), v.size());

        auto* old_data = v.data();
        const auto old_capacity = v.capacity();

        ThrowingCopy::copy_attempts = 0;
        ThrowingCopy::throw_on_copy_attempt = 2;

        EXPECT_THROW(v.shrink_to_fit(), std::runtime_error);

        EXPECT_EQ(v.data(), old_data);
        EXPECT_EQ(v.capacity(), old_capacity);
        ASSERT_EQ(v.size(), 3);
        EXPECT_EQ(v[0].value, 1);
        EXPECT_EQ(v[1].value, 2);
        EXPECT_EQ(v[2].value, 3);
        EXPECT_EQ(ThrowingCopy::alive, 3);

        ThrowingCopy::throw_on_copy_attempt = -1;
    }

    EXPECT_EQ(ThrowingCopy::alive, 0);
}

// ----------------------------------------------------------------------------
// Const API edge cases
// ----------------------------------------------------------------------------

TEST(VectorConstEdgeTest, ConstDataPointsToSameStorage) {
    Vector<int> v{1, 2, 3};
    const Vector<int>& cv = v;

    EXPECT_EQ(cv.data(), v.data());
}

TEST(VectorConstEdgeTest, ConstFrontAndBackAfterReserve) {
    Vector<int> v{1, 2, 3};
    v.reserve(50);

    const Vector<int>& cv = v;

    EXPECT_EQ(cv.front(), 1);
    EXPECT_EQ(cv.back(), 3);
}

// ----------------------------------------------------------------------------
// Reuse/mixed-operation scenarios
// ----------------------------------------------------------------------------

TEST(VectorScenarioEdgeTest, ClearShrinkAndReuse) {
    Vector<int> v{1, 2, 3, 4};

    v.reserve(32);
    v.clear();

    EXPECT_EQ(v.size(), 0);
    EXPECT_GE(v.capacity(), 32);

    v.shrink_to_fit();

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_EQ(v.data(), nullptr);

    v.push_back(99);

    ASSERT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 99);
}

TEST(VectorScenarioEdgeTest, RepeatedGrowShrinkGrowPreservesCorrectValues) {
    Vector<int> v;

    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }

    v.resize(10);
    v.shrink_to_fit();

    ASSERT_EQ(v.size(), 10);
    EXPECT_EQ(v.capacity(), 10);

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(v[static_cast<std::size_t>(i)], i);
    }

    for (int i = 10; i < 200; ++i) {
        v.push_back(i);
    }

    ASSERT_EQ(v.size(), 200);
    for (int i = 0; i < 200; ++i) {
        EXPECT_EQ(v[static_cast<std::size_t>(i)], i);
    }
}

TEST(DopTest, InternalLvalueWithReallocation) {
    Vector<int> v;
    v.reserve(100);

    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }

    v.push_back(v[0]);

    EXPECT_EQ(v.back(), 0);
}

TEST(DopTest, InternalLvalueWithoutReallocation) {
    Vector<int> v;
    v.reserve(101);

    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }

    v.push_back(v[0]);

    EXPECT_EQ(v.back(), 0);
}

TEST(DopTest, InternalRvalueWithReallocation) {
    Vector<int> v;
    v.reserve(100);

    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }

    v.push_back(std::move(v[0]));

    EXPECT_EQ(v.back(), 0);
}

TEST(DopTest, InternalRvalueWithoutReallocation) {
    Vector<int> v;
    v.reserve(101);

    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }

    v.push_back(std::move(v[0]));

    EXPECT_EQ(v.back(), 0);
}

TEST(DopTest, PushBackInternalRvalueStringWithReallocation) {
    Vector<std::string> v;
    v.reserve(2);

    v.push_back("hello");
    v.push_back("world");

    v.push_back(std::move(v[0]));

    ASSERT_EQ(v.size(), 3);

    EXPECT_EQ(v.back(), "hello");
}

TEST(VectorExceptionTest, ReserveThrows) {
    ThrowOnCopy::disable_throwing();

    {
        Vector<ThrowOnCopy> v;

        v.emplace_back(1);
        v.emplace_back(2);
        v.emplace_back(3);

        ASSERT_EQ(v.size(), 3);

        auto* old_data = v.data();
        const auto old_capacity = v.capacity();

        // ТОЛЬКО СЕЙЧАС начинаем бросать.
        ThrowOnCopy::throw_on(2);

        EXPECT_THROW(v.reserve(100), std::runtime_error);

        // Старый Vector должен остаться корректным.
        EXPECT_EQ(v.data(), old_data);
        EXPECT_EQ(v.capacity(), old_capacity);

        ASSERT_EQ(v.size(), 3);

        EXPECT_EQ(v[0].value, 1);
        EXPECT_EQ(v[1].value, 2);
        EXPECT_EQ(v[2].value, 3);

        EXPECT_EQ(ThrowOnCopy::alive, 3);

        ThrowOnCopy::disable_throwing();
    }

    EXPECT_EQ(ThrowOnCopy::alive, 0);
}

TEST(VectorExceptionTest, CopyConstructorThrows) {
    ThrowOnCopy::disable_throwing();

    {
        Vector<ThrowOnCopy> original;

        original.emplace_back(10);
        original.emplace_back(20);
        original.emplace_back(30);

        ASSERT_EQ(original.size(), 3);
        ASSERT_EQ(ThrowOnCopy::alive, 3);

        ThrowOnCopy::throw_on(2);

        EXPECT_THROW({ Vector<ThrowOnCopy> copy(original); }, std::runtime_error);

        // Частично созданная копия должна полностью очиститься.
        // Живы только 3 элемента original.
        EXPECT_EQ(ThrowOnCopy::alive, 3);

        ASSERT_EQ(original.size(), 3);

        EXPECT_EQ(original[0].value, 10);
        EXPECT_EQ(original[1].value, 20);
        EXPECT_EQ(original[2].value, 30);

        ThrowOnCopy::disable_throwing();
    }

    EXPECT_EQ(ThrowOnCopy::alive, 0);
}

TEST(VectorExceptionTest, ShrinkToFitThrows) {
    ThrowOnCopy::disable_throwing();

    {
        Vector<ThrowOnCopy> v;

        v.emplace_back(1);
        v.emplace_back(2);
        v.emplace_back(3);

        // Сначала спокойно создаем excess capacity.
        v.reserve(100);

        ASSERT_EQ(v.size(), 3);
        ASSERT_EQ(v.capacity(), 100);

        auto* old_data = v.data();

        // И только после подготовки включаем исключение.
        ThrowOnCopy::throw_on(2);

        EXPECT_THROW(v.shrink_to_fit(), std::runtime_error);

        EXPECT_EQ(v.data(), old_data);
        EXPECT_EQ(v.capacity(), 100);
        EXPECT_EQ(v.size(), 3);

        EXPECT_EQ(v[0].value, 1);
        EXPECT_EQ(v[1].value, 2);
        EXPECT_EQ(v[2].value, 3);

        EXPECT_EQ(ThrowOnCopy::alive, 3);

        ThrowOnCopy::disable_throwing();
    }

    EXPECT_EQ(ThrowOnCopy::alive, 0);
}