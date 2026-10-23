#include <gtest/gtest.h>

#include <cstddef>
#include <ml/layers/relu.hpp>
#include <ml/layers/linear.hpp>
#include <stdexcept>

using namespace ml;

TEST(ReLUTest, CorrectForwardRank2) {
    Tensor<int> input({3, 2});

    int num = 1;

    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 2; ++j) {
            input(i, j) = (static_cast<int>(i * 2 + j) + 1) * num;
        }

        num = num * -1;
    }

    ReLU<int> relu;

    auto result = relu.forward(input);

    EXPECT_EQ(result(0, 0), 1);
    EXPECT_EQ(result(0, 1), 2);
    EXPECT_EQ(result(1, 0), 0);
    EXPECT_EQ(result(1, 1), 0);
    EXPECT_EQ(result(2, 0), 5);
    EXPECT_EQ(result(2, 1), 6);

    EXPECT_EQ(input(1, 0), -3);
    EXPECT_EQ(input(1, 1), -4);

    EXPECT_EQ(result.shape(), input.shape());
}

TEST(ReLUTest, CorrectForwardRank1) {
    Tensor<int> input({3});

    int num = 1;

    for (std::size_t i = 0; i < 3; ++i) {
        input(i) = (static_cast<int>(i) + 1) * num;

        num = num * -1;
    }

    ReLU<int> relu;

    auto result = relu.forward(input);

    EXPECT_EQ(result(0), 1);
    EXPECT_EQ(result(1), 0);
    EXPECT_EQ(result(2), 3);

    EXPECT_EQ(input(1), -2);

    EXPECT_EQ(result.shape(), input.shape());
}