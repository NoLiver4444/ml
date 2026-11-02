#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <utility>

#include <ml/layers/linear.hpp>
#include <ml/models/model.hpp>
#include <ml/models/sequential.hpp>
#include <ml/tensor/tensor.hpp>

using namespace ml;

TEST(ModelTest, CorrectId) {
    Sequential<float> network;

    network.add(
        std::make_unique<Linear<float>>(2, 1)
    );

    Model model("test_model", std::move(network));

    EXPECT_EQ(model.id(), "test_model");
}

TEST(ModelTest, CorrectForward) {
    Tensor<float> weights({2, 1});
    Tensor<float> bias({1});

    weights(0, 0) = 2.0f;
    weights(1, 0) = 3.0f;
    bias(0) = -1.0f;

    Sequential<float> network;

    network.add(
        std::make_unique<Linear<float>>(weights, bias)
    );

    Model model("linear_model", std::move(network));

    Tensor<float> input({1, 2});

    input(0, 0) = 2.0f;
    input(0, 1) = 1.0f;

    auto result = model.forward(input);

    // 2*2 + 1*3 - 1 = 6
    EXPECT_FLOAT_EQ(result(0, 0), 6.0f);

    EXPECT_EQ(result.shape()[0], 1);
    EXPECT_EQ(result.shape()[1], 1);
}

TEST(ModelTest, EmptyIdThrows) {
    Sequential<float> network;

    network.add(
        std::make_unique<Linear<float>>(2, 1)
    );

    EXPECT_THROW(
        Model("", std::move(network)),
        std::invalid_argument
    );
}

TEST(ModelTest, EmptyNetworkThrows) {
    Sequential<float> network;

    EXPECT_THROW(
        Model("test_model", std::move(network)),
        std::invalid_argument
    );
}