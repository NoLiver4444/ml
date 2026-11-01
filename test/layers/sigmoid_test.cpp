#include <gtest/gtest.h>

#include <ml/layers/sigmoid.hpp>

using namespace ml;

TEST(Sigmoid, CorrectForwardZero) {
    Tensor<float> tensor({1});
    tensor(0) = 0.0f;

    Sigmoid<float> sigmoid;

    auto result = sigmoid.forward(tensor);

    EXPECT_NEAR(result(0), 0.5f, 1e-6f);

    EXPECT_EQ(tensor.shape(), result.shape());
    EXPECT_FLOAT_EQ(tensor(0), 0.0f);
}

TEST(Sigmoid, CorrectForwardOne) {
    Tensor<float> tensor({1});
    tensor(0) = 1.0f;

    Sigmoid<float> sigmoid;

    auto result = sigmoid.forward(tensor);

    EXPECT_NEAR(result(0), 0.7310586f, 1e-6f);

    EXPECT_EQ(tensor.shape(), result.shape());
    EXPECT_FLOAT_EQ(tensor(0), 1.0f);
}

TEST(Sigmoid, CorrectForwardNegativeOne) {
    Tensor<float> tensor({1});
    tensor(0) = -1.0f;

    Sigmoid<float> sigmoid;

    auto result = sigmoid.forward(tensor);

    EXPECT_NEAR(result(0), 0.2689414f, 1e-6f);

    EXPECT_EQ(tensor.shape(), result.shape());
    EXPECT_FLOAT_EQ(tensor(0), -1.0f);
}

TEST(Sigmoid, CorrectForwardRank2) {
    Tensor<float> input({3, 2});

    input(0, 0) = 1.0f;
    input(0, 1) = 2.0f;
    input(1, 0) = -1.0f;
    input(1, 1) = 0.0f;
    input(2, 0) = 0.0f;
    input(2, 1) = 1.0f;

    Sigmoid<float> sigmoid;

    auto result = sigmoid.forward(input);

    EXPECT_NEAR(result(0, 0), 0.7310586f, 1e-6f);
    EXPECT_NEAR(result(0, 1), 0.8807970f, 1e-6f);
    EXPECT_NEAR(result(1, 0), 0.2689414f, 1e-6f);
    EXPECT_NEAR(result(1, 1), 0.5f, 1e-6f);
    EXPECT_NEAR(result(2, 0), 0.5f, 1e-6f);
    EXPECT_NEAR(result(2, 1), 0.7310586f, 1e-6f);

    EXPECT_FLOAT_EQ(input(1, 0), -1.0f);
    EXPECT_FLOAT_EQ(input(1, 1), 0.0f);

    EXPECT_EQ(result.shape(), input.shape());
}