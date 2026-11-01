#include <gtest/gtest.h>

#include <ml/layers/relu.hpp>
#include <ml/layers/linear.hpp>

using namespace ml;

TEST(ReLUTest, CorrectForwardRank2) {
    Tensor<int> input({3, 2});

    input(0, 0) = 1;
    input(0, 1) = 2;
    input(1, 0) = -3;
    input(1, 1) = -4;
    input(2, 0) = 5;
    input(2, 1) = 6;

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

    input(0) = 1;
    input(1) = -2;
    input(2) = 3;

    ReLU<int> relu;

    auto result = relu.forward(input);

    EXPECT_EQ(result(0), 1);
    EXPECT_EQ(result(1), 0);
    EXPECT_EQ(result(2), 3);

    EXPECT_EQ(input(1), -2);

    EXPECT_EQ(result.shape(), input.shape());
}

TEST(ReLUTest, ZeroRemainsZero) {
    Tensor<int> input({1});
    input(0) = 0;

    ReLU<int> relu;

    auto result = relu.forward(input);

    EXPECT_EQ(result(0), 0);
    EXPECT_EQ(result.shape(), input.shape());
}