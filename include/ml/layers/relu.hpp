#pragma once

#include <ml/layers/layer.hpp>

namespace ml {

template <typename T>
class ReLU : public Layer<T> {
public:
    Tensor<T> forward(const Tensor<T>& input) const override;
};

}  // namespace ml

#include "relu.tpp"