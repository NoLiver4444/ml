#include <gtest/gtest.h>

#include <cstddef>
#include <ml/math/matrix.hpp>

#include <stdexcept>
#include <string>
#include <utility>

// ============================================================================
// Constructors
// ============================================================================

TEST(MatrixConstructorTest, DefaultConstructor) {
    Matrix<int> matrix;

    EXPECT_EQ(matrix.rows(), 0);
    EXPECT_EQ(matrix.cols(), 0);
    EXPECT_EQ(matrix.size(), 0);
    EXPECT_TRUE(matrix.empty());
    EXPECT_EQ(matrix.data(), nullptr);
}

TEST(MatrixConstructorTest, RowsAndColumnsConstructor) {
    Matrix<int> matrix(2, 3);

    EXPECT_EQ(matrix.rows(), 2);
    EXPECT_EQ(matrix.cols(), 3);
    EXPECT_EQ(matrix.size(), 6);
    EXPECT_FALSE(matrix.empty());

    for (std::size_t i = 0; i < matrix.rows(); ++i) {
        for (std::size_t j = 0; j < matrix.cols(); ++j) {
            EXPECT_EQ(matrix(i, j), 0);
        }
    }
}

TEST(MatrixConstructorTest, FillConstructor) {
    Matrix<int> matrix(2, 3, 42);

    EXPECT_EQ(matrix.rows(), 2);
    EXPECT_EQ(matrix.cols(), 3);
    EXPECT_EQ(matrix.size(), 6);

    for (std::size_t i = 0; i < matrix.rows(); ++i) {
        for (std::size_t j = 0; j < matrix.cols(); ++j) {
            EXPECT_EQ(matrix(i, j), 42);
        }
    }
}

TEST(MatrixConstructorTest, InitializerList) {
    Matrix<int> matrix{{1, 2, 3}, {4, 5, 6}};

    EXPECT_EQ(matrix.rows(), 2);
    EXPECT_EQ(matrix.cols(), 3);
    EXPECT_EQ(matrix.size(), 6);

    EXPECT_EQ(matrix(0, 0), 1);
    EXPECT_EQ(matrix(0, 1), 2);
    EXPECT_EQ(matrix(0, 2), 3);

    EXPECT_EQ(matrix(1, 0), 4);
    EXPECT_EQ(matrix(1, 1), 5);
    EXPECT_EQ(matrix(1, 2), 6);
}

TEST(MatrixConstructorTest, EmptyInitializerList) {
    Matrix<int> matrix{};

    EXPECT_EQ(matrix.rows(), 0);
    EXPECT_EQ(matrix.cols(), 0);
    EXPECT_EQ(matrix.size(), 0);
    EXPECT_TRUE(matrix.empty());
}

TEST(MatrixConstructorTest, UnequalRowsThrow) {
    EXPECT_THROW((Matrix<int>{{1, 2, 3}, {4, 5}}), std::invalid_argument);
}

// ============================================================================
// Zero dimensions
// ============================================================================

TEST(MatrixConstructorEdgeTest, ZeroRows) {
    Matrix<int> matrix(0, 5);

    EXPECT_EQ(matrix.rows(), 0);
    EXPECT_EQ(matrix.cols(), 5);
    EXPECT_EQ(matrix.size(), 0);
    EXPECT_TRUE(matrix.empty());
}

TEST(MatrixConstructorEdgeTest, ZeroColumns) {
    Matrix<int> matrix(5, 0);

    EXPECT_EQ(matrix.rows(), 5);
    EXPECT_EQ(matrix.cols(), 0);
    EXPECT_EQ(matrix.size(), 0);
    EXPECT_TRUE(matrix.empty());
}

TEST(MatrixConstructorEdgeTest, ZeroRowsAndColumns) {
    Matrix<int> matrix(0, 0);

    EXPECT_EQ(matrix.rows(), 0);
    EXPECT_EQ(matrix.cols(), 0);
    EXPECT_EQ(matrix.size(), 0);
    EXPECT_TRUE(matrix.empty());
}

// ============================================================================
// Copy constructor
// ============================================================================

TEST(MatrixCopyTest, CopyConstructor) {
    Matrix<int> original{{1, 2}, {3, 4}};

    Matrix<int> copy(original);

    EXPECT_EQ(copy.rows(), 2);
    EXPECT_EQ(copy.cols(), 2);

    EXPECT_EQ(copy(0, 0), 1);
    EXPECT_EQ(copy(0, 1), 2);
    EXPECT_EQ(copy(1, 0), 3);
    EXPECT_EQ(copy(1, 1), 4);
}

TEST(MatrixCopyTest, CopyIsIndependent) {
    Matrix<int> original{{1, 2}, {3, 4}};

    Matrix<int> copy(original);

    copy(0, 0) = 100;

    EXPECT_EQ(original(0, 0), 1);
    EXPECT_EQ(copy(0, 0), 100);
}

TEST(MatrixCopyTest, CopyEmptyMatrix) {
    Matrix<int> original;
    Matrix<int> copy(original);

    EXPECT_TRUE(copy.empty());
    EXPECT_EQ(copy.rows(), 0);
    EXPECT_EQ(copy.cols(), 0);
}

// ============================================================================
// Copy assignment
// ============================================================================

TEST(MatrixCopyAssignmentTest, CopyAssignment) {
    Matrix<int> source{{1, 2, 3}, {4, 5, 6}};

    Matrix<int> destination{{100}};

    destination = source;

    EXPECT_EQ(destination.rows(), 2);
    EXPECT_EQ(destination.cols(), 3);

    EXPECT_EQ(destination(0, 0), 1);
    EXPECT_EQ(destination(0, 1), 2);
    EXPECT_EQ(destination(0, 2), 3);

    EXPECT_EQ(destination(1, 0), 4);
    EXPECT_EQ(destination(1, 1), 5);
    EXPECT_EQ(destination(1, 2), 6);
}

TEST(MatrixCopyAssignmentTest, SelfAssignment) {
    Matrix<int> matrix{{1, 2}, {3, 4}};

    matrix = matrix;

    EXPECT_EQ(matrix.rows(), 2);
    EXPECT_EQ(matrix.cols(), 2);

    EXPECT_EQ(matrix(0, 0), 1);
    EXPECT_EQ(matrix(0, 1), 2);
    EXPECT_EQ(matrix(1, 0), 3);
    EXPECT_EQ(matrix(1, 1), 4);
}

TEST(MatrixCopyAssignmentTest, AssignmentChangesShape) {
    Matrix<int> destination(10, 10);

    Matrix<int> source{{1, 2, 3}};

    destination = source;

    EXPECT_EQ(destination.rows(), 1);
    EXPECT_EQ(destination.cols(), 3);
    EXPECT_EQ(destination.size(), 3);
}

// ============================================================================
// Move semantics
// ============================================================================

TEST(MatrixMoveTest, MoveConstructor) {
    Matrix<int> source{{1, 2}, {3, 4}};

    auto* original_data = source.data();

    Matrix<int> destination(std::move(source));

    EXPECT_EQ(destination.data(), original_data);

    EXPECT_EQ(destination.rows(), 2);
    EXPECT_EQ(destination.cols(), 2);

    EXPECT_EQ(destination(0, 0), 1);
    EXPECT_EQ(destination(1, 1), 4);

    EXPECT_EQ(source.rows(), 0);
    EXPECT_EQ(source.cols(), 0);
    EXPECT_EQ(source.size(), 0);
    EXPECT_TRUE(source.empty());
    EXPECT_EQ(source.data(), nullptr);
}

TEST(MatrixMoveTest, MoveAssignment) {
    Matrix<int> source{{1, 2, 3}, {4, 5, 6}};

    Matrix<int> destination{{100, 200}};

    auto* source_data = source.data();

    destination = std::move(source);

    EXPECT_EQ(destination.data(), source_data);

    EXPECT_EQ(destination.rows(), 2);
    EXPECT_EQ(destination.cols(), 3);

    EXPECT_EQ(destination(0, 0), 1);
    EXPECT_EQ(destination(1, 2), 6);

    EXPECT_TRUE(source.empty());
    EXPECT_EQ(source.rows(), 0);
    EXPECT_EQ(source.cols(), 0);
    EXPECT_EQ(source.data(), nullptr);
}

TEST(MatrixMoveTest, SelfMoveAssignment) {
    Matrix<int> matrix{{1, 2}, {3, 4}};

    matrix = std::move(matrix);

    EXPECT_EQ(matrix.rows(), 2);
    EXPECT_EQ(matrix.cols(), 2);

    EXPECT_EQ(matrix(0, 0), 1);
    EXPECT_EQ(matrix(1, 1), 4);
}

// ============================================================================
// Shape
// ============================================================================

TEST(MatrixShapeTest, Rows) {
    Matrix<int> matrix(3, 7);

    EXPECT_EQ(matrix.rows(), 3);
}

TEST(MatrixShapeTest, Columns) {
    Matrix<int> matrix(3, 7);

    EXPECT_EQ(matrix.cols(), 7);
}

TEST(MatrixShapeTest, Size) {
    Matrix<int> matrix(3, 7);

    EXPECT_EQ(matrix.size(), 21);
}

TEST(MatrixShapeTest, Empty) {
    Matrix<int> empty;
    Matrix<int> non_empty(2, 2);

    EXPECT_TRUE(empty.empty());
    EXPECT_FALSE(non_empty.empty());
}

// ============================================================================
// operator()
// ============================================================================

TEST(MatrixAccessTest, OperatorParenthesesRead) {
    Matrix<int> matrix{{1, 2, 3}, {4, 5, 6}};

    EXPECT_EQ(matrix(0, 0), 1);
    EXPECT_EQ(matrix(0, 2), 3);
    EXPECT_EQ(matrix(1, 0), 4);
    EXPECT_EQ(matrix(1, 2), 6);
}

TEST(MatrixAccessTest, OperatorParenthesesWrite) {
    Matrix<int> matrix(2, 2);

    matrix(0, 0) = 10;
    matrix(0, 1) = 20;
    matrix(1, 0) = 30;
    matrix(1, 1) = 40;

    EXPECT_EQ(matrix(0, 0), 10);
    EXPECT_EQ(matrix(0, 1), 20);
    EXPECT_EQ(matrix(1, 0), 30);
    EXPECT_EQ(matrix(1, 1), 40);
}

TEST(MatrixAccessTest, ConstOperatorParentheses) {
    const Matrix<int> matrix{{1, 2}, {3, 4}};

    EXPECT_EQ(matrix(0, 0), 1);
    EXPECT_EQ(matrix(1, 1), 4);
}

// ============================================================================
// at()
// ============================================================================

TEST(MatrixAtTest, ValidAccess) {
    Matrix<int> matrix{{10, 20}, {30, 40}};

    EXPECT_EQ(matrix.at(0, 0), 10);
    EXPECT_EQ(matrix.at(1, 1), 40);
}

TEST(MatrixAtTest, AllowsModification) {
    Matrix<int> matrix(2, 2);

    matrix.at(1, 1) = 42;

    EXPECT_EQ(matrix(1, 1), 42);
}

TEST(MatrixAtTest, RowOutOfRangeThrows) {
    Matrix<int> matrix(2, 3);

    EXPECT_THROW(matrix.at(2, 0), std::out_of_range);
    EXPECT_THROW(matrix.at(100, 0), std::out_of_range);
}

TEST(MatrixAtTest, ColumnOutOfRangeThrows) {
    Matrix<int> matrix(2, 3);

    EXPECT_THROW(matrix.at(0, 3), std::out_of_range);
    EXPECT_THROW(matrix.at(0, 100), std::out_of_range);
}

TEST(MatrixAtTest, BothIndicesOutOfRangeThrow) {
    Matrix<int> matrix(2, 3);

    EXPECT_THROW(matrix.at(100, 100), std::out_of_range);
}

TEST(MatrixAtTest, EmptyMatrixThrows) {
    Matrix<int> matrix;

    EXPECT_THROW(matrix.at(0, 0), std::out_of_range);
}

TEST(MatrixAtTest, ConstVersion) {
    const Matrix<int> matrix{{1, 2}, {3, 4}};

    EXPECT_EQ(matrix.at(0, 1), 2);
}

// ============================================================================
// data()
// ============================================================================

TEST(MatrixDataTest, ReturnsContiguousRowMajorData) {
    Matrix<int> matrix{{1, 2, 3}, {4, 5, 6}};

    int* ptr = matrix.data();

    ASSERT_NE(ptr, nullptr);

    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[1], 2);
    EXPECT_EQ(ptr[2], 3);
    EXPECT_EQ(ptr[3], 4);
    EXPECT_EQ(ptr[4], 5);
    EXPECT_EQ(ptr[5], 6);
}

TEST(MatrixDataTest, ModificationThroughDataPointer) {
    Matrix<int> matrix(2, 2);

    int* ptr = matrix.data();

    ptr[3] = 100;

    EXPECT_EQ(matrix(1, 1), 100);
}

TEST(MatrixDataTest, ConstVersion) {
    const Matrix<int> matrix{{1, 2}, {3, 4}};

    const int* ptr = matrix.data();

    ASSERT_NE(ptr, nullptr);

    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[3], 4);
}

TEST(MatrixDataTest, EmptyMatrixReturnsNullptr) {
    Matrix<int> matrix;

    EXPECT_EQ(matrix.data(), nullptr);
}

// ============================================================================
// fill()
// ============================================================================

TEST(MatrixFillTest, FillEntireMatrix) {
    Matrix<int> matrix(3, 4);

    matrix.fill(7);

    for (std::size_t i = 0; i < matrix.rows(); ++i) {
        for (std::size_t j = 0; j < matrix.cols(); ++j) {
            EXPECT_EQ(matrix(i, j), 7);
        }
    }
}

TEST(MatrixFillTest, FillOverwritesExistingValues) {
    Matrix<int> matrix{{1, 2}, {3, 4}};

    matrix.fill(100);

    EXPECT_EQ(matrix(0, 0), 100);
    EXPECT_EQ(matrix(0, 1), 100);
    EXPECT_EQ(matrix(1, 0), 100);
    EXPECT_EQ(matrix(1, 1), 100);
}

TEST(MatrixFillTest, FillStringMatrix) {
    Matrix<std::string> matrix(2, 2);

    matrix.fill("hello");

    EXPECT_EQ(matrix(0, 0), "hello");
    EXPECT_EQ(matrix(0, 1), "hello");
    EXPECT_EQ(matrix(1, 0), "hello");
    EXPECT_EQ(matrix(1, 1), "hello");
}

TEST(MatrixFillTest, FillEmptyMatrixDoesNothing) {
    Matrix<int> matrix;

    EXPECT_NO_THROW(matrix.fill(10));

    EXPECT_TRUE(matrix.empty());
}

// ============================================================================
// transpose()
// ============================================================================

TEST(MatrixTransposeTest, RectangularMatrix) {
    Matrix<int> matrix{{1, 2, 3}, {4, 5, 6}};

    Matrix<int> transposed = matrix.transpose();

    EXPECT_EQ(transposed.rows(), 3);
    EXPECT_EQ(transposed.cols(), 2);

    EXPECT_EQ(transposed(0, 0), 1);
    EXPECT_EQ(transposed(0, 1), 4);

    EXPECT_EQ(transposed(1, 0), 2);
    EXPECT_EQ(transposed(1, 1), 5);

    EXPECT_EQ(transposed(2, 0), 3);
    EXPECT_EQ(transposed(2, 1), 6);
}

TEST(MatrixTransposeTest, SquareMatrix) {
    Matrix<int> matrix{{1, 2}, {3, 4}};

    Matrix<int> transposed = matrix.transpose();

    EXPECT_EQ(transposed(0, 0), 1);
    EXPECT_EQ(transposed(0, 1), 3);
    EXPECT_EQ(transposed(1, 0), 2);
    EXPECT_EQ(transposed(1, 1), 4);
}

TEST(MatrixTransposeTest, SingleRow) {
    Matrix<int> matrix{{1, 2, 3}};

    Matrix<int> transposed = matrix.transpose();

    EXPECT_EQ(transposed.rows(), 3);
    EXPECT_EQ(transposed.cols(), 1);

    EXPECT_EQ(transposed(0, 0), 1);
    EXPECT_EQ(transposed(1, 0), 2);
    EXPECT_EQ(transposed(2, 0), 3);
}

TEST(MatrixTransposeTest, SingleColumn) {
    Matrix<int> matrix{{1}, {2}, {3}};

    Matrix<int> transposed = matrix.transpose();

    EXPECT_EQ(transposed.rows(), 1);
    EXPECT_EQ(transposed.cols(), 3);

    EXPECT_EQ(transposed(0, 0), 1);
    EXPECT_EQ(transposed(0, 1), 2);
    EXPECT_EQ(transposed(0, 2), 3);
}

TEST(MatrixTransposeTest, SingleElement) {
    Matrix<int> matrix{{42}};

    Matrix<int> transposed = matrix.transpose();

    EXPECT_EQ(transposed.rows(), 1);
    EXPECT_EQ(transposed.cols(), 1);
    EXPECT_EQ(transposed(0, 0), 42);
}

TEST(MatrixTransposeTest, DoubleTransposeRestoresOriginal) {
    Matrix<int> matrix{{1, 2, 3}, {4, 5, 6}};

    Matrix<int> result = matrix.transpose().transpose();

    EXPECT_EQ(result.rows(), matrix.rows());
    EXPECT_EQ(result.cols(), matrix.cols());

    for (std::size_t i = 0; i < matrix.rows(); ++i) {
        for (std::size_t j = 0; j < matrix.cols(); ++j) {
            EXPECT_EQ(result(i, j), matrix(i, j));
        }
    }
}

TEST(MatrixTransposeTest, EmptyMatrix) {
    Matrix<int> matrix;

    Matrix<int> result = matrix.transpose();

    EXPECT_TRUE(result.empty());
    EXPECT_EQ(result.rows(), 0);
    EXPECT_EQ(result.cols(), 0);
}

TEST(MatrixTransposeTest, DoubleTransposeReturnsOriginal) {
    Matrix<int> a{{1, 8, -3}, {4, 0, 17}};

    Matrix<int> result = a.transpose().transpose();

    ASSERT_EQ(result.rows(), a.rows());
    ASSERT_EQ(result.cols(), a.cols());

    for (std::size_t i = 0; i < a.rows(); ++i) {
        for (std::size_t j = 0; j < a.cols(); ++j) {
            EXPECT_EQ(result(i, j), a(i, j));
        }
    }
}

// ============================================================================
// swap()
// ============================================================================

TEST(MatrixSwapTest, SwapMatrices) {
    Matrix<int> first{{1, 2}, {3, 4}};

    Matrix<int> second{{10, 20, 30}};

    first.swap(second);

    EXPECT_EQ(first.rows(), 1);
    EXPECT_EQ(first.cols(), 3);

    EXPECT_EQ(first(0, 0), 10);
    EXPECT_EQ(first(0, 1), 20);
    EXPECT_EQ(first(0, 2), 30);

    EXPECT_EQ(second.rows(), 2);
    EXPECT_EQ(second.cols(), 2);

    EXPECT_EQ(second(0, 0), 1);
    EXPECT_EQ(second(1, 1), 4);
}

TEST(MatrixSwapTest, SwapWithEmptyMatrix) {
    Matrix<int> empty;

    Matrix<int> matrix{{1, 2}, {3, 4}};

    empty.swap(matrix);

    EXPECT_EQ(empty.rows(), 2);
    EXPECT_EQ(empty.cols(), 2);

    EXPECT_TRUE(matrix.empty());
    EXPECT_EQ(matrix.rows(), 0);
    EXPECT_EQ(matrix.cols(), 0);
}

TEST(MatrixSwapTest, SelfSwap) {
    Matrix<int> matrix{{1, 2}, {3, 4}};

    auto* old_data = matrix.data();

    matrix.swap(matrix);

    EXPECT_EQ(matrix.data(), old_data);
    EXPECT_EQ(matrix.rows(), 2);
    EXPECT_EQ(matrix.cols(), 2);

    EXPECT_EQ(matrix(0, 0), 1);
    EXPECT_EQ(matrix(1, 1), 4);
}

// ============================================================================
// Non-trivial types
// ============================================================================

TEST(MatrixTypeTest, StringMatrix) {
    Matrix<std::string> matrix{{"one", "two"}, {"three", "four"}};

    EXPECT_EQ(matrix.rows(), 2);
    EXPECT_EQ(matrix.cols(), 2);

    EXPECT_EQ(matrix(0, 0), "one");
    EXPECT_EQ(matrix(0, 1), "two");
    EXPECT_EQ(matrix(1, 0), "three");
    EXPECT_EQ(matrix(1, 1), "four");
}

TEST(MatrixTypeTest, StringTranspose) {
    Matrix<std::string> matrix{{"a", "b", "c"}, {"d", "e", "f"}};

    Matrix<std::string> result = matrix.transpose();

    EXPECT_EQ(result.rows(), 3);
    EXPECT_EQ(result.cols(), 2);

    EXPECT_EQ(result(0, 0), "a");
    EXPECT_EQ(result(0, 1), "d");

    EXPECT_EQ(result(1, 0), "b");
    EXPECT_EQ(result(1, 1), "e");

    EXPECT_EQ(result(2, 0), "c");
    EXPECT_EQ(result(2, 1), "f");
}

TEST(MatrixOperatorTest, MatricesOfTheSameSizeSum) {
    Matrix<int> matrix_1{{1, 2}, {3, 4}};
    Matrix<int> matrix_2{{4, 3}, {2, 1}};

    Matrix<int> result = matrix_1 + matrix_2;

    EXPECT_EQ(result(0, 0), 5);
    EXPECT_EQ(result(0, 1), 5);
    EXPECT_EQ(result(1, 0), 5);
    EXPECT_EQ(result(1, 1), 5);
}

TEST(MatrixOperatorTest, MatricesOfTheSameSizeEqSum) {
    Matrix<int> matrix_1{{1, 2}, {3, 4}};
    Matrix<int> matrix_2{{4, 3}, {2, 1}};

    matrix_1 += matrix_2;

    EXPECT_EQ(matrix_1(0, 0), 5);
    EXPECT_EQ(matrix_1(0, 1), 5);
    EXPECT_EQ(matrix_1(1, 0), 5);
    EXPECT_EQ(matrix_1(1, 1), 5);
}

TEST(MatrixOperatorTest, MatricesOfTheSameSizeSub) {
    Matrix<int> matrix_1{{5, 5}, {5, 5}};
    Matrix<int> matrix_2{{1, 2}, {3, 4}};

    Matrix<int> result = matrix_1 - matrix_2;

    EXPECT_EQ(result(0, 0), 4);
    EXPECT_EQ(result(0, 1), 3);
    EXPECT_EQ(result(1, 0), 2);
    EXPECT_EQ(result(1, 1), 1);
}

TEST(MatrixOperatorTest, MatricesOfTheSameSizeEqSub) {
    Matrix<int> matrix_1{{5, 5}, {5, 5}};
    Matrix<int> matrix_2{{1, 2}, {3, 4}};

    matrix_1 -= matrix_2;

    EXPECT_EQ(matrix_1(0, 0), 4);
    EXPECT_EQ(matrix_1(0, 1), 3);
    EXPECT_EQ(matrix_1(1, 0), 2);
    EXPECT_EQ(matrix_1(1, 1), 1);
}

TEST(MatrixOperatorTest, MatricesOfDifferentSizesSum) {
    Matrix<int> matrix_1{{1, 2}, {3, 4}};
    Matrix<int> matrix_2{{4, 3}};

    EXPECT_THROW(Matrix<int> result = matrix_1 + matrix_2;, std::invalid_argument);
}

TEST(MatrixOperatorTest, MatricesOfDifferentSizesEqSum) {
    Matrix<int> matrix_1{{1, 2}, {3, 4}};
    Matrix<int> matrix_2{{4, 3}};

    EXPECT_THROW(matrix_1 += matrix_2;, std::invalid_argument);
}

TEST(MatrixOperatorTest, MatricesOfDifferentSizesSub) {
    Matrix<int> matrix_1{{5, 5}, {5, 5}};
    Matrix<int> matrix_2{{1, 2}};

    EXPECT_THROW(Matrix<int> result = matrix_1 - matrix_2;, std::invalid_argument);
}

TEST(MatrixOperatorTest, MatricesOfDifferentSizesEqSub) {
    Matrix<int> matrix_1{{5, 5}, {5, 5}};
    Matrix<int> matrix_2{{1, 2}};

    EXPECT_THROW(matrix_1 -= matrix_2;, std::invalid_argument);
}

TEST(MatrixOperatorTest, MatricesOfZeroSizesSum) {
    Matrix<int> matrix_1;
    Matrix<int> matrix_2;

    Matrix<int> result = matrix_1 + matrix_2;

    EXPECT_TRUE(result.empty());
    EXPECT_EQ(result.rows(), 0);
    EXPECT_EQ(result.cols(), 0);
}

TEST(MatrixOperatorTest, MatricesOfZeroSizesEqSum) {
    Matrix<int> matrix_1;
    Matrix<int> matrix_2;

    matrix_1 += matrix_2;

    EXPECT_TRUE(matrix_1.empty());
    EXPECT_EQ(matrix_1.rows(), 0);
    EXPECT_EQ(matrix_1.cols(), 0);
}

TEST(MatrixOperatorTest, MatricesOfZeroSizesSub) {
    Matrix<int> matrix_1;
    Matrix<int> matrix_2;

    Matrix<int> result = matrix_1 - matrix_2;

    EXPECT_TRUE(result.empty());
    EXPECT_EQ(result.rows(), 0);
    EXPECT_EQ(result.cols(), 0);
}

TEST(MatrixOperatorTest, MatricesOfZeroSizesEqSub) {
    Matrix<int> matrix_1;
    Matrix<int> matrix_2;

    matrix_1 -= matrix_2;

    EXPECT_TRUE(matrix_1.empty());
    EXPECT_EQ(matrix_1.rows(), 0);
    EXPECT_EQ(matrix_1.cols(), 0);
}

TEST(MatrixOperatorTest, MatricesOfOneSizesSum) {
    Matrix<int> matrix_1{{1}};
    Matrix<int> matrix_2{{2}};

    Matrix<int> result = matrix_1 + matrix_2;

    EXPECT_EQ(result(0, 0), 3);
}

TEST(MatrixOperatorTest, MatricesOfOneSizesEqSum) {
    Matrix<int> matrix_1{{1}};
    Matrix<int> matrix_2{{2}};

    matrix_1 += matrix_2;

    EXPECT_EQ(matrix_1(0, 0), 3);
}

TEST(MatrixOperatorTest, MatricesOfOneSizesSub) {
    Matrix<int> matrix_1{{2}};
    Matrix<int> matrix_2{{1}};

    Matrix<int> result = matrix_1 - matrix_2;

    EXPECT_EQ(result(0, 0), 1);
}

TEST(MatrixOperatorTest, MatricesOfOneSizesEqSub) {
    Matrix<int> matrix_1{{2}};
    Matrix<int> matrix_2{{1}};

    matrix_1 -= matrix_2;

    EXPECT_EQ(matrix_1(0, 0), 1);
}

TEST(MatrixOperatorTest, MatricesOfTheOneNSizeSum) {
    Matrix<int> matrix_1{{1, 2}};
    Matrix<int> matrix_2{{4, 3}};

    Matrix<int> result = matrix_1 + matrix_2;

    EXPECT_EQ(result(0, 0), 5);
    EXPECT_EQ(result(0, 1), 5);
}

TEST(MatrixOperatorTest, MatricesOfTheOneNSizeEqSum) {
    Matrix<int> matrix_1{{1, 2}};
    Matrix<int> matrix_2{{4, 3}};

    matrix_1 += matrix_2;

    EXPECT_EQ(matrix_1(0, 0), 5);
    EXPECT_EQ(matrix_1(0, 1), 5);
}

TEST(MatrixOperatorTest, MatricesOfTheOneNSizeSub) {
    Matrix<int> matrix_1{{5, 5}};
    Matrix<int> matrix_2{{1, 2}};

    Matrix<int> result = matrix_1 - matrix_2;

    EXPECT_EQ(result(0, 0), 4);
    EXPECT_EQ(result(0, 1), 3);
}

TEST(MatrixOperatorTest, MatricesOfTheOneNSizeEqSub) {
    Matrix<int> matrix_1{{5, 5}};
    Matrix<int> matrix_2{{1, 2}};

    matrix_1 -= matrix_2;

    EXPECT_EQ(matrix_1(0, 0), 4);
    EXPECT_EQ(matrix_1(0, 1), 3);
}

TEST(MatrixOperatorTest, MatricesOfTheSameSizeAZeroSum) {
    Matrix<int> matrix_1{{1, 2}, {3, 4}};
    Matrix<int> matrix_2{{0, 0}, {0, 0}};

    Matrix<int> result = matrix_1 + matrix_2;

    EXPECT_EQ(result(0, 0), matrix_1(0, 0));
    EXPECT_EQ(result(0, 1), matrix_1(0, 1));
    EXPECT_EQ(result(1, 0), matrix_1(1, 0));
    EXPECT_EQ(result(1, 1), matrix_1(1, 1));
}

TEST(MatrixOperatorTest, MatricesOfTheSameSizeAZeroSub) {
    Matrix<int> matrix_1{{5, 5}, {5, 5}};
    Matrix<int> matrix_2{{0, 0}, {0, 0}};

    Matrix<int> result = matrix_1 - matrix_2;

    EXPECT_EQ(result(0, 0), matrix_1(0, 0));
    EXPECT_EQ(result(0, 1), matrix_1(0, 1));
    EXPECT_EQ(result(1, 0), matrix_1(1, 0));
    EXPECT_EQ(result(1, 1), matrix_1(1, 1));
}

TEST(MatrixOperatorTest, MatricesOfTheSameSizeABEqBASum) {
    Matrix<int> matrix_1{{1, 2}, {3, 4}};
    Matrix<int> matrix_2{{4, 3}, {2, 1}};

    Matrix<int> result_1 = matrix_1 + matrix_2;
    Matrix<int> result_2 = matrix_2 + matrix_1;

    EXPECT_EQ(result_1(0, 0), result_2(0, 0));
    EXPECT_EQ(result_1(0, 1), result_2(0, 1));
    EXPECT_EQ(result_1(1, 0), result_2(1, 0));
    EXPECT_EQ(result_1(1, 1), result_2(1, 1));
}

TEST(MatrixOperatorTest, MatricesOfTheSameSizeABCEqBCASum) {
    Matrix<int> matrix_1{{1, 2}, {3, 4}};
    Matrix<int> matrix_2{{4, 3}, {2, 1}};
    Matrix<int> matrix_3{{10, 10}, {10, 10}};

    Matrix<int> result_1 = (matrix_1 + matrix_2) + matrix_3;
    Matrix<int> result_2 = (matrix_2 + matrix_3) + matrix_1;

    EXPECT_EQ(result_1(0, 0), result_2(0, 0));
    EXPECT_EQ(result_1(0, 1), result_2(0, 1));
    EXPECT_EQ(result_1(1, 0), result_2(1, 0));
    EXPECT_EQ(result_1(1, 1), result_2(1, 1));
}

TEST(MatrixOperatorTest, MatricesOfTheSameSizeABSumBSubEqA) {
    Matrix<int> matrix_1{{1, 2}, {3, 4}};
    Matrix<int> matrix_2{{4, 3}, {2, 1}};

    Matrix<int> result = (matrix_1 + matrix_2) - matrix_2;

    EXPECT_EQ(result(0, 0), matrix_1(0, 0));
    EXPECT_EQ(result(0, 1), matrix_1(0, 1));
    EXPECT_EQ(result(1, 0), matrix_1(1, 0));
    EXPECT_EQ(result(1, 1), matrix_1(1, 1));
}

