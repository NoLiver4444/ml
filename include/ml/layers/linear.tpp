#pragma once

#include <cstddef>
#include <ml/layers/linear.hpp>
#include <stdexcept>
#include "ml/tensor/tensor.hpp"

namespace ml {

template <typename T>
Linear<T>::Linear(std::size_t in_features, std::size_t out_features)
    : weights_(makeWeights(in_features, out_features)), bias_({out_features}, T{}) {}

template <typename T>
Linear<T>::Linear(const Tensor<T>& weights, const Tensor<T>& bias)
    : weights_(weights), bias_(bias) {
    if (weights.ndim() != 2) {
        throw std::invalid_argument("Linear weights must be a rank-2 tensor");
    }

    if (bias.ndim() != 1) {
        throw std::invalid_argument("Linear bias must be a rank-1 tensor");
    }

    if (weights.shape()[0] == 0 || weights.shape()[1] == 0) {
        throw std::invalid_argument("Linear layer dimensions must be greater than zero");
    }

    if (bias.shape()[0] != weights.shape()[1]) {
        throw std::invalid_argument("Bias size must match number of output features");
    }
}

template <typename T>
Tensor<T> Linear<T>::forward(const Tensor<T>& input) const {
    if (input.ndim() != 2) {
        throw std::invalid_argument("Input tensor rank must be 2");
    }

    if (input.shape()[1] != weights_.shape()[0]) {
        throw std::invalid_argument("Input tensor size must match number of input features");
    }

    auto result = input.matmul(weights_);

    for (std::size_t i = 0; i < result.shape()[0]; ++i) {
        for (std::size_t j = 0; j < result.shape()[1]; ++j) {
            result(i, j) += bias_(j);
        }
    }

    return result;
}

template <typename T>
const Tensor<T>& Linear<T>::weights() const noexcept {
    return weights_;
}

template <typename T>
const Tensor<T>& Linear<T>::bias() const noexcept {
    return bias_;
}

template <typename T>
std::size_t Linear<T>::inFeatures() const noexcept {
    return weights_.shape()[0];
}

template <typename T>
std::size_t Linear<T>::outFeatures() const noexcept {
    return weights_.shape()[1];
}

template <typename T>
Tensor<T> Linear<T>::makeWeights(std::size_t in_features, std::size_t out_features) {
    if (in_features == 0 || out_features == 0) {
        throw std::invalid_argument("Linear layer dimensions must be greater than zero");
    }

    return Tensor<T>({in_features, out_features}, T{});
}

}  // namespace ml