#pragma once

#include <ml/tensor/tensor.hpp>

namespace ml {

template <typename T>
class Layer {
public:
    virtual Tensor<T> forward(const Tensor<T>& input) const = 0;

    virtual ~Layer() = default;
};

}  // namespace ml