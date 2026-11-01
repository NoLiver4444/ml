#include <gtest/gtest.h>

#include <memory>

#include <ml/layers/linear.hpp>
#include <ml/layers/relu.hpp>
#include <ml/layers/sigmoid.hpp>
#include <ml/models/sequential.hpp>
#include <ml/tensor/tensor.hpp>

using namespace ml;

TEST(SequentialTest, ForwardThroughCompleteNetwork) {
    Tensor<float> input({1, 2});

    input(0, 0) = 2.0f;
    input(0, 1) = -1.0f;

    Tensor<float> weights_1({2, 2});
    Tensor<float> bias_1({2});

    weights_1(0, 0) = 1.0f;
    weights_1(0, 1) = -1.0f;
    weights_1(1, 0) = 1.0f;
    weights_1(1, 1) = 1.0f;

    bias_1(0) = 0.0f;
    bias_1(1) = 0.0f;

    Tensor<float> weights_2({2, 1});
    Tensor<float> bias_2({1});

    weights_2(0, 0) = 2.0f;
    weights_2(1, 0) = 3.0f;

    bias_2(0) = -1.0f;

    Sequential<float> model;

    model.add(std::make_unique<Linear<float>>(weights_1, bias_1));

    model.add(std::make_unique<ReLU<float>>());

    model.add(std::make_unique<Linear<float>>(weights_2, bias_2));

    model.add(std::make_unique<Sigmoid<float>>());

    auto result = model.forward(input);

    EXPECT_EQ(result.shape()[0], 1);
    EXPECT_EQ(result.shape()[1], 1);

    EXPECT_NEAR(result(0, 0), 0.7310586f, 1e-6f);
}

TEST(SequentialTest, EmptySequentialReturnsInput) {
    Sequential<float> model;

    Tensor<float> input({2, 2});

    input(0, 0) = 1.0f;
    input(0, 1) = -2.0f;
    input(1, 0) = 3.0f;
    input(1, 1) = 4.0f;

    auto result = model.forward(input);

    EXPECT_EQ(result.shape(), input.shape());

    EXPECT_FLOAT_EQ(result(0, 0), 1.0f);
    EXPECT_FLOAT_EQ(result(0, 1), -2.0f);
    EXPECT_FLOAT_EQ(result(1, 0), 3.0f);
    EXPECT_FLOAT_EQ(result(1, 1), 4.0f);

    EXPECT_FLOAT_EQ(input(0, 1), -2.0f);
}

TEST(SequentialTest, AddNullLayerThrows) {
    Sequential<float> model;

    EXPECT_THROW(model.add(nullptr), std::invalid_argument);
}