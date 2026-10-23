#pragma once

#include <ml/tensor/tensor.hpp>

namespace ml {

template <typename T>
class ReLU {
public:
    Tensor<T> forward(const Tensor<T>& input) const;
};

}  // namespace ml

#include "relu.tpp"