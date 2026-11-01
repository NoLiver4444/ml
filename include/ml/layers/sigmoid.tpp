#pragma once

#include <cmath>
#include <cstddef>
#include <type_traits>
#include <ml/layers/sigmoid.hpp>

namespace ml {

template <typename T>
Tensor<T> Sigmoid<T>::forward(const Tensor<T>& input) const {
    Tensor<T> result = input;

    for (std::size_t i = 0; i < result.size(); ++i) {
        auto& el = result.data()[i];

        el = T{1} / (T{1} + std::exp(-el));
    }

    return result;
}

}  // namespace ml