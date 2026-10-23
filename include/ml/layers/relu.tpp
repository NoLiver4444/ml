#pragma once

#include <algorithm>
#include <cstddef>
#include <ml/layers/relu.hpp>

namespace ml {

template <typename T>
Tensor<T> ReLU<T>::forward(const Tensor<T>& input) const {
    Tensor<T> result = input;

    for (std::size_t i = 0; i < result.size(); ++i) {
        auto& el = result.data()[i];

        el = std::max(el, T{});
    }

    return result;
}

}  // namespace ml