#include <gtest/gtest.h>

#include <cstddef>
#include <ml/tensor/tensor.hpp>

#include <stdexcept>
#include <string>
#include <utility>
#include "ml/core/vector.hpp"

TEST(TensorConstructorTest, DefaultConstructor) {
    Tensor<int> tensor;

    EXPECT_EQ(tensor.shape().size(), 0);
    EXPECT_EQ(tensor.strides().size(), 0);
    EXPECT_EQ(tensor.size(), 0);
    EXPECT_EQ(tensor.ndim(), 0);
    EXPECT_TRUE(tensor.empty());
}

TEST(TensorConstructorTest, ConstructorNSize) {
    Tensor<int> tensor({2, 3, 4});

    EXPECT_EQ(tensor.shape()[0], 2);
    EXPECT_EQ(tensor.shape()[1], 3);
    EXPECT_EQ(tensor.shape()[2], 4);

    EXPECT_EQ(tensor.strides()[0], 12);
    EXPECT_EQ(tensor.strides()[1], 4);
    EXPECT_EQ(tensor.strides()[2], 1);

    EXPECT_EQ(tensor.size(), 24);
    EXPECT_EQ(tensor.ndim(), 3);

    EXPECT_FALSE(tensor.empty());
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