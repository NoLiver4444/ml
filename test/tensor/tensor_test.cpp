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

TEST(TensorConstructorTest, ConstructorNSizeAndValue) {
    Tensor<int> tensor({2, 3}, 5.0f);
}