#include <gtest/gtest.h>

#include <cstddef>
#include <ml/layers/linear.hpp>
#include <stdexcept>
#include "ml/tensor/tensor.hpp"

using namespace ml;

TEST(LinearConstructorTest, SizeConstructor) {
    Linear<int> linear(2, 4);

    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
            EXPECT_EQ(linear.weights()(i, j), 0);
        }
    }

    for (std::size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(linear.bias()(i), 0);
    }

    EXPECT_EQ(linear.inFeatures(), 2);
    EXPECT_EQ(linear.outFeatures(), 4);
}

TEST(LinearConstructorTest, TensorsConstructor) {
    Tensor<int> weights({3, 8}, 10);
    Tensor<int> bias({8}, 2);

    Linear<int> linear(weights, bias);

    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 8; ++j) {
            EXPECT_EQ(linear.weights()(i, j), 10);
        }
    }

    for (std::size_t i = 0; i < 8; ++i) {
        EXPECT_EQ(linear.bias()(i), 2);
    }

    EXPECT_EQ(linear.inFeatures(), 3);
    EXPECT_EQ(linear.outFeatures(), 8);
}

TEST(LinearConstructorTest, ZeroInputFeaturesThrows) {
    EXPECT_THROW(Linear<int> linear(0, 8);, std::invalid_argument);
}

TEST(LinearConstructorTest, ZeroOutputFeaturesThrows) {
    EXPECT_THROW(Linear<int> linear(3, 0);, std::invalid_argument);
}

TEST(LinearConstructorTest, WrongWeightsRankThrows) {
    Tensor<int> weights({3}, 10);
    Tensor<int> bias({8}, 2);

    EXPECT_THROW(Linear<int> linear(weights, bias);, std::invalid_argument);
}

TEST(LinearConstructorTest, WrongBiasRankThrows) {
    Tensor<int> weights({3, 8}, 10);
    Tensor<int> bias({8, 2}, 2);

    EXPECT_THROW(Linear<int> linear(weights, bias);, std::invalid_argument);
}

TEST(LinearConstructorTest, BiasSizeMismatchThrows) {
    Tensor<int> weights({3, 8}, 10);
    Tensor<int> bias({7}, 2);

    EXPECT_THROW(Linear<int> linear(weights, bias);, std::invalid_argument);
}

TEST(LinearForwardTest, CorrectForward) {
    Tensor<int> input({2, 3});
    Tensor<int> weights({3, 2});
    Tensor<int> bias({2});

    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            input(i, j) = static_cast<int>(i * 3 + j) + 1;
        }
    }

    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 2; ++j) {
            weights(i, j) = static_cast<int>(i * 2 + j) + 1;
        }
    }

    bias(0) = 10;
    bias(1) = 20;

    Linear<int> linear(weights, bias);

    auto result = linear.forward(input);

    EXPECT_EQ(result(0, 0), 32);
    EXPECT_EQ(result(0, 1), 48);
    EXPECT_EQ(result(1, 0), 59);
    EXPECT_EQ(result(1, 1), 84);

    EXPECT_EQ(result.shape()[0], 2);
    EXPECT_EQ(result.shape()[1], 2);
}

TEST(LinearForwardTest, WrongInputRankThrows) {
    Tensor<int> input({2});
    Tensor<int> weights({3, 2});
    Tensor<int> bias({2});

    Linear<int> linear(weights, bias);

    EXPECT_THROW(linear.forward(input), std::invalid_argument);
}

TEST(LinearForwardTest, InputSizeMismatchThrows) {
    Tensor<int> input({2, 2});
    Tensor<int> weights({3, 2});
    Tensor<int> bias({2});

    Linear<int> linear(weights, bias);

    EXPECT_THROW(linear.forward(input), std::invalid_argument);
}