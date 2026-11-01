#pragma once

#include "ml/tensor/tensor.hpp"
#include "sequential.hpp"
#include <utility>
#include <stdexcept>

namespace ml {

template <typename T>
void Sequential<T>::add(std::unique_ptr<Layer<T>> layer) {
    if (!layer) {
        throw std::invalid_argument("Sequential cannot contain null layer");
    }

    layers_.push_back(std::move(layer));
}

template <typename T>
Tensor<T> Sequential<T>::forward(const Tensor<T>& input) const {
    Tensor<T> result = input;

    for (const auto& layer : layers_) {
        result = layer->forward(result);
    }

    return result;
}

template <typename T>
std::size_t Sequential<T>::size() const noexcept {
    return layers_.size();
}

template <typename T>
bool Sequential<T>::empty() const noexcept {
    return layers_.empty();
}

}  // namespace ml