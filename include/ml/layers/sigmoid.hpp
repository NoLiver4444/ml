#pragma once

#include <type_traits>
#include <ml/layers/layer.hpp>

namespace ml {

template <typename T>
class Sigmoid : public Layer<T> {
    static_assert(std::is_floating_point_v<T>, "Sigmoid requires floating-point type");

public:
    Tensor<T> forward(const Tensor<T>& input) const override;
};

}  // namespace ml

#include "sigmoid.tpp"