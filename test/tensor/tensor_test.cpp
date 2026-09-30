#include <gtest/gtest.h>

#include <cstddef>
#include <ml/tensor/tensor.hpp>

#include <stdexcept>
#include <string>
#include <utility>
#include "ml/core/vector.hpp"

using namespace ml;

// ============================================================================
// Constructors
// ============================================================================

TEST(TensorConstructorTest, DefaultConstructor) {
    Tensor<int> tensor;

    EXPECT_EQ(tensor.ndim(), 0);
    EXPECT_EQ(tensor.size(), 0);
    EXPECT_TRUE(tensor.empty());
    EXPECT_EQ(tensor.data(), nullptr);
}

TEST(TensorConstructorTest, ShapeConstructor) {
    Tensor<int> tensor({2, 3, 4});

    EXPECT_EQ(tensor.ndim(), 3);
    EXPECT_EQ(tensor.size(), 24);
    EXPECT_FALSE(tensor.empty());

    EXPECT_EQ(tensor.shape()[0], 2);
    EXPECT_EQ(tensor.shape()[1], 3);
    EXPECT_EQ(tensor.shape()[2], 4);

    EXPECT_EQ(tensor.strides()[0], 12);
    EXPECT_EQ(tensor.strides()[1], 4);
    EXPECT_EQ(tensor.strides()[2], 1);
}

TEST(TensorConstructorTest, ShapeAndValueConstructor) {
    Tensor<int> tensor({2, 3}, 7);

    EXPECT_EQ(tensor.ndim(), 2);
    EXPECT_EQ(tensor.size(), 6);

    EXPECT_EQ(tensor.shape()[0], 2);
    EXPECT_EQ(tensor.shape()[1], 3);

    EXPECT_EQ(tensor.strides()[0], 3);
    EXPECT_EQ(tensor.strides()[1], 1);

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        EXPECT_EQ(tensor.data()[i], 7);
    }
}

TEST(TensorConstructorTest, ScalarTensor) {
    Tensor<int> tensor({}, 42);

    EXPECT_EQ(tensor.ndim(), 0);
    EXPECT_EQ(tensor.size(), 1);
    EXPECT_FALSE(tensor.empty());

    EXPECT_EQ(tensor.data()[0], 42);
}

TEST(TensorConstructorTest, EmptyDimensionTensor) {
    Tensor<int> tensor({2, 0, 3});

    EXPECT_EQ(tensor.ndim(), 3);
    EXPECT_EQ(tensor.size(), 0);
    EXPECT_TRUE(tensor.empty());

    EXPECT_EQ(tensor.shape()[0], 2);
    EXPECT_EQ(tensor.shape()[1], 0);
    EXPECT_EQ(tensor.shape()[2], 3);
}

// ============================================================================
// at()
// ============================================================================

TEST(TensorAtTest, Read1D) {
    Tensor<int> tensor({4});

    tensor(0) = 10;
    tensor(1) = 20;
    tensor(2) = 30;
    tensor(3) = 40;

    EXPECT_EQ(tensor.at(0), 10);
    EXPECT_EQ(tensor.at(1), 20);
    EXPECT_EQ(tensor.at(2), 30);
    EXPECT_EQ(tensor.at(3), 40);
}

TEST(TensorAtTest, Read3D) {
    Tensor<int> tensor({2, 3, 4});

    tensor(1, 2, 3) = 99;

    EXPECT_EQ(tensor.at(1, 2, 3), 99);
}

TEST(TensorAtTest, WriteAccess) {
    Tensor<int> tensor({2, 2});

    tensor.at(1, 1) = 42;

    EXPECT_EQ(tensor(1, 1), 42);
}

TEST(TensorAtTest, ConstAccess) {
    Tensor<int> tensor({2, 2});

    tensor(0, 1) = 55;

    const Tensor<int>& const_tensor = tensor;

    EXPECT_EQ(const_tensor.at(0, 1), 55);
}

TEST(TensorAtTest, WrongNumberOfIndicesThrows) {
    Tensor<int> tensor({2, 3, 4});

    EXPECT_THROW(tensor.at(1, 2), std::invalid_argument);

    EXPECT_THROW(tensor.at(1, 2, 3, 4), std::invalid_argument);
}

TEST(TensorAtTest, FirstDimensionOutOfRangeThrows) {
    Tensor<int> tensor({2, 3, 4});

    EXPECT_THROW(tensor.at(2, 0, 0), std::out_of_range);
}

TEST(TensorAtTest, MiddleDimensionOutOfRangeThrows) {
    Tensor<int> tensor({2, 3, 4});

    EXPECT_THROW(tensor.at(0, 3, 0), std::out_of_range);
}

TEST(TensorAtTest, LastDimensionOutOfRangeThrows) {
    Tensor<int> tensor({2, 3, 4});

    EXPECT_THROW(tensor.at(0, 0, 4), std::out_of_range);
}

TEST(TensorAtTest, LastValidElement) {
    Tensor<int> tensor({2, 3, 4});

    tensor.at(1, 2, 3) = 123;

    EXPECT_EQ(tensor.at(1, 2, 3), 123);
}

TEST(TensorTransposeTest, Transpose2D) {
    Tensor<int> tensor({2, 2});

    tensor(0, 0) = 1;
    tensor(0, 1) = 2;
    tensor(1, 0) = 3;
    tensor(1, 1) = 4;

    tensor = tensor.transpose(0, 1);

    EXPECT_EQ(tensor(0, 0), 1);
    EXPECT_EQ(tensor(0, 1), 3);
    EXPECT_EQ(tensor(1, 0), 2);
    EXPECT_EQ(tensor(1, 1), 4);
}

TEST(TensorTransposeTest, Transpose3D) {
    Tensor<int> tensor({2, 2, 3});

    int value = 1;

    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 2; ++j) {
            for (std::size_t k = 0; k < 3; ++k) {
                tensor(i, j, k) = value++;
            }
        }
    }

    /*
        Original shape: {2, 2, 3}

        tensor[0]:

        1  2  3
        4  5  6

        tensor[1]:

        7   8   9
        10 11  12
    */

    Tensor<int> result = tensor.transpose(0, 2);

    // {2, 2, 3} -> {3, 2, 2}

    EXPECT_EQ(result.ndim(), 3);
    EXPECT_EQ(result.shape()[0], 3);
    EXPECT_EQ(result.shape()[1], 2);
    EXPECT_EQ(result.shape()[2], 2);

    /*
        Expected:

        result[0]:

        1   7
        4  10

        result[1]:

        2   8
        5  11

        result[2]:

        3   9
        6  12
    */

    EXPECT_EQ(result(0, 0, 0), 1);
    EXPECT_EQ(result(0, 0, 1), 7);
    EXPECT_EQ(result(0, 1, 0), 4);
    EXPECT_EQ(result(0, 1, 1), 10);

    EXPECT_EQ(result(1, 0, 0), 2);
    EXPECT_EQ(result(1, 0, 1), 8);
    EXPECT_EQ(result(1, 1, 0), 5);
    EXPECT_EQ(result(1, 1, 1), 11);

    EXPECT_EQ(result(2, 0, 0), 3);
    EXPECT_EQ(result(2, 0, 1), 9);
    EXPECT_EQ(result(2, 1, 0), 6);
    EXPECT_EQ(result(2, 1, 1), 12);
}

TEST(TensorTransposeTest, Transpose4D) {
    Tensor<int> tensor({2, 3, 2, 2});

    int value = 1;

    for (std::size_t a = 0; a < 2; ++a) {
        for (std::size_t b = 0; b < 3; ++b) {
            for (std::size_t c = 0; c < 2; ++c) {
                for (std::size_t d = 0; d < 2; ++d) {
                    tensor(a, b, c, d) = value++;
                }
            }
        }
    }

    /*
        Original shape: {2, 3, 2, 2}

        axis0 = 0:

        axis1 = 0       axis1 = 1       axis1 = 2

        1  2            5  6             9 10
        3  4            7  8            11 12


        axis0 = 1:

        axis1 = 0       axis1 = 1       axis1 = 2

        13 14           17 18           21 22
        15 16           19 20           23 24
    */

    Tensor<int> result = tensor.transpose(0, 1);

    // {2, 3, 2, 2} -> {3, 2, 2, 2}

    EXPECT_EQ(result.ndim(), 4);
    EXPECT_EQ(result.shape()[0], 3);
    EXPECT_EQ(result.shape()[1], 2);
    EXPECT_EQ(result.shape()[2], 2);
    EXPECT_EQ(result.shape()[3], 2);

    /*
        Expected data:

        axis0 = 0:
            axis1 = 0       axis1 = 1

            1  2            13 14
            3  4            15 16

        axis0 = 1:
            axis1 = 0       axis1 = 1

            5  6            17 18
            7  8            19 20

        axis0 = 2:
            axis1 = 0       axis1 = 1

             9 10           21 22
            11 12           23 24
    */

    EXPECT_EQ(result(0, 0, 0, 0), 1);
    EXPECT_EQ(result(0, 0, 0, 1), 2);
    EXPECT_EQ(result(0, 0, 1, 0), 3);
    EXPECT_EQ(result(0, 0, 1, 1), 4);

    EXPECT_EQ(result(0, 1, 0, 0), 13);
    EXPECT_EQ(result(0, 1, 0, 1), 14);
    EXPECT_EQ(result(0, 1, 1, 0), 15);
    EXPECT_EQ(result(0, 1, 1, 1), 16);

    EXPECT_EQ(result(1, 0, 0, 0), 5);
    EXPECT_EQ(result(1, 0, 0, 1), 6);
    EXPECT_EQ(result(1, 0, 1, 0), 7);
    EXPECT_EQ(result(1, 0, 1, 1), 8);

    EXPECT_EQ(result(1, 1, 0, 0), 17);
    EXPECT_EQ(result(1, 1, 0, 1), 18);
    EXPECT_EQ(result(1, 1, 1, 0), 19);
    EXPECT_EQ(result(1, 1, 1, 1), 20);

    EXPECT_EQ(result(2, 0, 0, 0), 9);
    EXPECT_EQ(result(2, 0, 0, 1), 10);
    EXPECT_EQ(result(2, 0, 1, 0), 11);
    EXPECT_EQ(result(2, 0, 1, 1), 12);

    EXPECT_EQ(result(2, 1, 0, 0), 21);
    EXPECT_EQ(result(2, 1, 0, 1), 22);
    EXPECT_EQ(result(2, 1, 1, 0), 23);
    EXPECT_EQ(result(2, 1, 1, 1), 24);
}

TEST(TensorTransposeTest, DoubleTransposeReturnsOriginalTensor) {
    Tensor<int> tensor({2, 3, 4});

    int value = 1;

    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            for (std::size_t k = 0; k < 4; ++k) {
                tensor(i, j, k) = value++;
            }
        }
    }

    Tensor<int> result = tensor.transpose(0, 2).transpose(0, 2);

    EXPECT_EQ(result.shape(), tensor.shape());

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        EXPECT_EQ(result.data()[i], tensor.data()[i]);
    }
}

// ============================================================================
// Reshape
// ============================================================================

TEST(TensorReshapeTest, BasicReshape) {
    Tensor<int> tensor({2, 3});

    int value = 1;

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        tensor.data()[i] = value++;
    }

    tensor.reshape(3, 2);

    EXPECT_EQ(tensor.shape()[0], 3);
    EXPECT_EQ(tensor.shape()[1], 2);

    EXPECT_EQ(tensor.size(), 6);

    EXPECT_EQ(tensor(0, 0), 1);
    EXPECT_EQ(tensor(0, 1), 2);
    EXPECT_EQ(tensor(1, 0), 3);
    EXPECT_EQ(tensor(1, 1), 4);
    EXPECT_EQ(tensor(2, 0), 5);
    EXPECT_EQ(tensor(2, 1), 6);
}

TEST(TensorReshapeTest, ReshapeDoesNotChangeData) {
    Tensor<int> tensor({2, 3});

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        tensor.data()[i] = static_cast<int>(i + 1);
    }

    int* old_data = tensor.data();

    tensor.reshape(6);

    EXPECT_EQ(tensor.data(), old_data);

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        EXPECT_EQ(tensor.data()[i], static_cast<int>(i + 1));
    }
}

TEST(TensorReshapeTest, WrongSizeThrows) {
    Tensor<int> tensor({2, 3});

    EXPECT_THROW(tensor.reshape(2, 4), std::invalid_argument);
}

// ============================================================================
// Flatten
// ============================================================================

TEST(TensorFlattenTest, BasicFlatten) {
    Tensor<int> tensor({2, 3, 4});

    tensor.flatten();

    EXPECT_EQ(tensor.ndim(), 1);
    EXPECT_EQ(tensor.shape()[0], 24);
    EXPECT_EQ(tensor.size(), 24);
}

TEST(TensorFlattenTest, FlattenDoesNotChangeData) {
    Tensor<int> tensor({2, 2, 2});

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        tensor.data()[i] = static_cast<int>(i + 1);
    }

    int* old_data = tensor.data();

    tensor.flatten();

    EXPECT_EQ(tensor.data(), old_data);

    for (std::size_t i = 0; i < 8; ++i) {
        EXPECT_EQ(tensor.data()[i], static_cast<int>(i + 1));
    }
}

// ============================================================================
// Transpose
// ============================================================================

TEST(TensorTransposeTest, Basic2DTranspose) {
    Tensor<int> tensor({2, 3});

    /*
        1 2 3
        4 5 6
    */

    tensor(0, 0) = 1;
    tensor(0, 1) = 2;
    tensor(0, 2) = 3;
    tensor(1, 0) = 4;
    tensor(1, 1) = 5;
    tensor(1, 2) = 6;

    Tensor<int> result = tensor.transpose(0, 1);

    EXPECT_EQ(result.shape()[0], 3);
    EXPECT_EQ(result.shape()[1], 2);

    /*
        1 4
        2 5
        3 6
    */

    EXPECT_EQ(result(0, 0), 1);
    EXPECT_EQ(result(0, 1), 4);

    EXPECT_EQ(result(1, 0), 2);
    EXPECT_EQ(result(1, 1), 5);

    EXPECT_EQ(result(2, 0), 3);
    EXPECT_EQ(result(2, 1), 6);
}

TEST(TensorTransposeTest, Basic3DTranspose) {
    Tensor<int> tensor({2, 2, 3});

    int value = 1;

    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 2; ++j) {
            for (std::size_t k = 0; k < 3; ++k) {
                tensor(i, j, k) = value++;
            }
        }
    }

    Tensor<int> result = tensor.transpose(0, 2);

    EXPECT_EQ(result.shape()[0], 3);
    EXPECT_EQ(result.shape()[1], 2);
    EXPECT_EQ(result.shape()[2], 2);

    EXPECT_EQ(result(0, 0, 0), 1);
    EXPECT_EQ(result(0, 0, 1), 7);

    EXPECT_EQ(result(0, 1, 0), 4);
    EXPECT_EQ(result(0, 1, 1), 10);

    EXPECT_EQ(result(1, 0, 0), 2);
    EXPECT_EQ(result(1, 0, 1), 8);

    EXPECT_EQ(result(2, 1, 0), 6);
    EXPECT_EQ(result(2, 1, 1), 12);
}

TEST(TensorTransposeTest, WrongDimensionThrows) {
    Tensor<int> tensor({2, 3, 4});

    EXPECT_THROW(tensor.transpose(0, 3), std::out_of_range);
}

// ============================================================================
// Permute
// ============================================================================

TEST(TensorPermuteTest, Basic3DPermute) {
    Tensor<int> tensor({2, 3, 4});

    int value = 1;

    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            for (std::size_t k = 0; k < 4; ++k) {
                tensor(i, j, k) = value++;
            }
        }
    }

    Tensor<int> result = tensor.permute({2, 0, 1});

    // {2,3,4} -> {4,2,3}

    EXPECT_EQ(result.shape()[0], 4);
    EXPECT_EQ(result.shape()[1], 2);
    EXPECT_EQ(result.shape()[2], 3);

    // old(i,j,k) -> new(k,i,j)

    EXPECT_EQ(result(0, 0, 0), tensor(0, 0, 0));
    EXPECT_EQ(result(3, 0, 2), tensor(0, 2, 3));
    EXPECT_EQ(result(0, 1, 0), tensor(1, 0, 0));
    EXPECT_EQ(result(3, 1, 2), tensor(1, 2, 3));
}

TEST(TensorPermuteTest, IdentityPermutation) {
    Tensor<int> tensor({2, 3, 4});

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        tensor.data()[i] = static_cast<int>(i + 1);
    }

    Tensor<int> result = tensor.permute({0, 1, 2});

    EXPECT_EQ(result.shape(), tensor.shape());

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        EXPECT_EQ(result.data()[i], tensor.data()[i]);
    }
}

TEST(TensorPermuteTest, WrongNumberOfDimensionsThrows) {
    Tensor<int> tensor({2, 3, 4});

    EXPECT_THROW(tensor.permute({0, 1}), std::invalid_argument);
}

TEST(TensorPermuteTest, DimensionOutOfRangeThrows) {
    Tensor<int> tensor({2, 3, 4});

    EXPECT_THROW(tensor.permute({0, 1, 3}), std::out_of_range);
}

TEST(TensorPermuteTest, DuplicateDimensionThrows) {
    Tensor<int> tensor({2, 3, 4});

    EXPECT_THROW(tensor.permute({0, 0, 2}), std::invalid_argument);
}

TEST(TensorPermuteTest, Double3DPermute) {
    Tensor<int> tensor({2, 3, 4});

    int value = 1;

    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            for (std::size_t k = 0; k < 4; ++k) {
                tensor(i, j, k) = value++;
            }
        }
    }

    Tensor<int> result = tensor.permute({2, 0, 1});
    result = result.permute({1, 2, 0});

    EXPECT_EQ(result.shape(), tensor.shape());

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        EXPECT_EQ(result.data()[i], tensor.data()[i]);
    }
}

TEST(FlattenReshapeTest, ReshapeFlattenReshape) {
    Tensor<int> tensor({2, 3, 4});

    int value = 1;

    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            for (std::size_t k = 0; k < 4; ++k) {
                tensor(i, j, k) = value++;
            }
        }
    }

    Tensor<int> result = tensor;

    result.reshape(4, 6);
    result.flatten();
    result.reshape(2, 3, 4);

    EXPECT_EQ(result.shape(), tensor.shape());

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        EXPECT_EQ(result.data()[i], tensor.data()[i]);
    }
}

// ============================================================================
// Shape invariants / edge cases
// ============================================================================

TEST(TensorPermuteInvariantTest, InversePermutationRestoresTensor) {
    Tensor<int> tensor({2, 3, 4});

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        tensor.data()[i] = static_cast<int>(i + 1);
    }

    Tensor<int> result = tensor.permute({2, 0, 1});
    result = result.permute({1, 2, 0});

    EXPECT_EQ(result.shape(), tensor.shape());

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        EXPECT_EQ(result.data()[i], tensor.data()[i]);
    }
}

TEST(TensorReshapeInvariantTest, ReshapeFlattenReshapeRestoresTensor) {
    Tensor<int> tensor({2, 3, 4});

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        tensor.data()[i] = static_cast<int>(i + 1);
    }

    Tensor<int> original = tensor;

    tensor.reshape(4, 6);
    tensor.flatten();
    tensor.reshape(2, 3, 4);

    EXPECT_EQ(tensor.shape(), original.shape());

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        EXPECT_EQ(tensor.data()[i], original.data()[i]);
    }
}

TEST(TensorTransposeEdgeTest, EqualSizedAxesStillMoveData) {
    Tensor<int> tensor({2, 2, 3});

    int value = 1;

    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 2; ++j) {
            for (std::size_t k = 0; k < 3; ++k) {
                tensor(i, j, k) = value++;
            }
        }
    }

    Tensor<int> result = tensor.transpose(0, 1);

    EXPECT_EQ(result.shape(), tensor.shape());

    // shape тот же, но координаты 0 и 1 поменялись
    EXPECT_EQ(result(1, 0, 2), tensor(0, 1, 2));
    EXPECT_EQ(result(0, 1, 1), tensor(1, 0, 1));
}

TEST(TensorPermuteEdgeTest, EqualSizedAxesStillMoveData) {
    Tensor<int> tensor({2, 2, 2});

    int value = 1;

    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 2; ++j) {
            for (std::size_t k = 0; k < 2; ++k) {
                tensor(i, j, k) = value++;
            }
        }
    }

    Tensor<int> result = tensor.permute({2, 0, 1});

    EXPECT_EQ(result.shape(), tensor.shape());

    EXPECT_EQ(result(1, 0, 1), tensor(0, 1, 1));
    EXPECT_EQ(result(0, 1, 0), tensor(1, 0, 0));
}

TEST(TensorOneDimensionalTest, IdentityOperations) {
    Tensor<int> tensor({5});

    for (std::size_t i = 0; i < 5; ++i) {
        tensor(i) = static_cast<int>(i + 1);
    }

    Tensor<int> transposed = tensor.transpose(0, 0);
    Tensor<int> permuted = tensor.permute({0});

    EXPECT_EQ(transposed.shape(), tensor.shape());
    EXPECT_EQ(permuted.shape(), tensor.shape());

    for (std::size_t i = 0; i < tensor.size(); ++i) {
        EXPECT_EQ(transposed.data()[i], tensor.data()[i]);
        EXPECT_EQ(permuted.data()[i], tensor.data()[i]);
    }
}

TEST(TensorOneDimensionalTest, InvalidTransposeThrows) {
    Tensor<int> tensor({5});

    EXPECT_THROW(tensor.transpose(0, 1), std::out_of_range);
}

TEST(TensorScalarTest, PermuteEmptyDimensions) {
    Tensor<int> tensor({}, 42);

    Tensor<int> result = tensor.permute({});

    EXPECT_EQ(result.ndim(), 0);
    EXPECT_EQ(result.size(), 1);
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.data()[0], 42);
}

TEST(TensorScalarTest, TransposeThrows) {
    Tensor<int> tensor({}, 42);

    EXPECT_THROW(tensor.transpose(0, 0), std::out_of_range);
}

TEST(TensorEmptyDimensionTest, Transpose) {
    Tensor<int> tensor({2, 0, 3});

    Tensor<int> result = tensor.transpose(0, 2);

    EXPECT_EQ(result.ndim(), 3);
    EXPECT_EQ(result.size(), 0);
    EXPECT_TRUE(result.empty());

    EXPECT_EQ(result.shape()[0], 3);
    EXPECT_EQ(result.shape()[1], 0);
    EXPECT_EQ(result.shape()[2], 2);
}

TEST(TensorEmptyDimensionTest, Permute) {
    Tensor<int> tensor({2, 0, 3});

    Tensor<int> result = tensor.permute({1, 2, 0});

    EXPECT_EQ(result.ndim(), 3);
    EXPECT_EQ(result.size(), 0);
    EXPECT_TRUE(result.empty());

    EXPECT_EQ(result.shape()[0], 0);
    EXPECT_EQ(result.shape()[1], 3);
    EXPECT_EQ(result.shape()[2], 2);
}

TEST(TensorStrideTest, ThreeDimensionalStrides) {
    Tensor<int> tensor({2, 3, 4});

    ASSERT_EQ(tensor.strides().size(), 3);

    EXPECT_EQ(tensor.strides()[0], 12);
    EXPECT_EQ(tensor.strides()[1], 4);
    EXPECT_EQ(tensor.strides()[2], 1);
}

TEST(TensorStrideTest, FourDimensionalStrides) {
    Tensor<int> tensor({2, 3, 4, 5});

    ASSERT_EQ(tensor.strides().size(), 4);

    EXPECT_EQ(tensor.strides()[0], 60);
    EXPECT_EQ(tensor.strides()[1], 20);
    EXPECT_EQ(tensor.strides()[2], 5);
    EXPECT_EQ(tensor.strides()[3], 1);
}

TEST(TensorMatmulTest, Tensors32On23) {
    Tensor<int> tensor_1({3, 2});
    Tensor<int> tensor_2({2, 3});

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            tensor_1(i, j) = 2 * i + j + 1;
        }
    }

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            tensor_2(i, j) = 3 * i + j + 1;
        }
    }

    Tensor<int> result = tensor_1.matmul(tensor_2);

    EXPECT_EQ(result(0, 0), 9);
    EXPECT_EQ(result(0, 1), 12);
    EXPECT_EQ(result(0, 2), 15);

    EXPECT_EQ(result(1, 0), 19);
    EXPECT_EQ(result(1, 1), 26);
    EXPECT_EQ(result(1, 2), 33);

    EXPECT_EQ(result(2, 0), 29);
    EXPECT_EQ(result(2, 1), 40);
    EXPECT_EQ(result(2, 2), 51);

    Vector<std::size_t> shape = {3, 3};

    EXPECT_EQ(result.shape(), shape);
}

TEST(TensorMatmulTest, Tensors23On32) {
    Tensor<int> tensor_1({2, 3});
    Tensor<int> tensor_2({3, 2});

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            tensor_1(i, j) = 3 * i + j + 1;
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            tensor_2(i, j) = 6 + 2 * i + j + 1;
        }
    }

    Tensor<int> result = tensor_1.matmul(tensor_2);

    EXPECT_EQ(result(0, 0), 58);
    EXPECT_EQ(result(0, 1), 64);

    EXPECT_EQ(result(1, 0), 139);
    EXPECT_EQ(result(1, 1), 154);

    Vector<std::size_t> shape = {2, 2};

    EXPECT_EQ(result.shape(), shape);
}

TEST(TensorMatmulTest, Tensors23On34) {
    Tensor<int> tensor_1({2, 3});
    Tensor<int> tensor_2({3, 4});

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            tensor_1(i, j) = 3 * i + j + 1;
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            tensor_2(i, j) = 4 * i + j + 1;
        }
    }

    Tensor<int> result = tensor_1.matmul(tensor_2);

    EXPECT_EQ(result(0, 0), 38);
    EXPECT_EQ(result(0, 1), 44);
    EXPECT_EQ(result(0, 2), 50);
    EXPECT_EQ(result(0, 3), 56);

    EXPECT_EQ(result(1, 0), 83);
    EXPECT_EQ(result(1, 1), 98);
    EXPECT_EQ(result(1, 2), 113);
    EXPECT_EQ(result(1, 3), 128);

    Vector<std::size_t> shape = {2, 4};

    EXPECT_EQ(result.shape(), shape);
}

TEST(TensorMatmulTest, TensorsRankOver2) {
    Tensor<int> tensor_1({2, 3, 4});
    Tensor<int> tensor_2({3, 4});

    EXPECT_THROW(Tensor<int> result = tensor_1.matmul(tensor_2);, std::invalid_argument);
}

TEST(TensorMatmulTest, TensorShapesDoNotMatch) {
    Tensor<int> tensor_1({2, 3});
    Tensor<int> tensor_2({4, 4});

    EXPECT_THROW(Tensor<int> result = tensor_1.matmul(tensor_2);, std::invalid_argument);
}

TEST(TensorMatmulTest, TensorsFloat) {
    Tensor<float> tensor_1({2, 1});
    Tensor<float> tensor_2({1, 2});

    tensor_1(0, 0) = 0.5;
    tensor_1(1, 0) = 1.5;

    tensor_2(0, 0) = 0.5;
    tensor_2(0, 1) = 0.25;

    Tensor<float> result = tensor_1.matmul(tensor_2);

    EXPECT_EQ(result(0, 0), 0.25);
    EXPECT_EQ(result(0, 1), 0.125);
    EXPECT_EQ(result(1, 0), 0.75);
    EXPECT_EQ(result(1, 1), 0.375);

    Vector<std::size_t> shape = {2, 2};

    EXPECT_EQ(result.shape(), shape);
}

TEST(TensorMatmulTest, TensorsFloat2) {
    Tensor<float> tensor_1({1, 2});
    Tensor<float> tensor_2({2, 1});

    tensor_1(0, 0) = 0.5;
    tensor_1(0, 1) = 1.5;

    tensor_2(0, 0) = 0.5;
    tensor_2(1, 0) = 0.25;

    Tensor<float> result = tensor_1.matmul(tensor_2);

    EXPECT_EQ(result(0, 0), 0.625);

    Vector<std::size_t> shape = {1, 1};

    EXPECT_EQ(result.shape(), shape);
}