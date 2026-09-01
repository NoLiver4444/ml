#pragma once

#include <cstddef>
#include <stdexcept>
#include "ml/core/vector.hpp"
#include "tensor.hpp"

template <typename T>
Tensor<T>::Tensor() : data_(), shape_(), strides_() {}

template <typename T>
Tensor<T>::Tensor(const Vector<std::size_t>& shape)
    : data_(compute_size(shape)), shape_(shape), strides_(shape.size()) {
    compute_strides();
}

template <typename T>
Tensor<T>::Tensor(const Vector<std::size_t>& shape, const T& value)
    : data_(compute_size(shape), value), shape_(shape), strides_(shape.size()) {
    compute_strides();
}

template <typename T>
void Tensor<T>::compute_strides() {
    std::size_t stride = 1;

    for (std::size_t i = shape_.size(); i > 0; --i) {
        strides_[i - 1] = stride;
        stride *= shape_[i - 1];
    }
}

template <typename T>
std::size_t Tensor<T>::compute_offset(
    const Vector<std::size_t>& indices
) const
{
    std::size_t offset = 0;

    for (std::size_t i = 0; i < indices.size(); ++i) {
        offset += indices[i] * strides_[i];
    }

    return offset;
}

template <typename T>
std::size_t Tensor<T>::compute_size(const Vector<std::size_t>& shape) {
    std::size_t result = 1;

    for (const auto dimension : shape) {
        result *= dimension;
    }

    return result;
}

template <typename T>
const Vector<std::size_t>& Tensor<T>::shape() const noexcept {
    return shape_;
}

template <typename T>
const Vector<std::size_t>& Tensor<T>::strides() const noexcept {
    return strides_;
}

template <typename T>
std::size_t Tensor<T>::size() const noexcept {
    return data_.size();
}

template <typename T>
std::size_t Tensor<T>::ndim() const noexcept {
    return shape_.size();
}

template <typename T>
bool Tensor<T>::empty() const noexcept {
    return data_.empty();
}

template <typename T>
T* Tensor<T>::data() noexcept {
    return data_;
}

template <typename T>
const T* Tensor<T>::data() const noexcept {
    return data_;
}

template <typename T>
template <typename... Indices>
T& Tensor<T>::operator()(Indices... indices) {
    static_assert(
        (std::is_integral_v<Indices> && ...),
        "Tensor indices must be integral"
    );

    Vector<std::size_t> v{static_cast<std::size_t>(indices)...};

    return data_[compute_offset(v)];
}

template <typename T>
template <typename... Indices>
const T& Tensor<T>::operator()(Indices... indices) const {
    static_assert(
        (std::is_integral_v<Indices> && ...),
        "Tensor indices must be integral"
    );
    
    Vector<std::size_t> v{static_cast<std::size_t>(indices)...};

    return data_[compute_offset(v)];
}

template <typename T>
template <typename... Indices>
T& Tensor<T>::at(Indices... indices) {
    static_assert(
        (std::is_integral_v<Indices> && ...),
        "Tensor indices must be integral"
    );

    Vector<std::size_t> v{static_cast<std::size_t>(indices)...};

    if (v.size() != ndim()) {
        throw std::invalid_argument("Number of indices does not match tensor dimensions");
    }

    for (std::size_t i = 0; i < shape_.size(); ++i) {
        if (v[i] >= shape_[i]) {
            throw std::out_of_range("Tensor index out of range");
        }
    }

    return data_[compute_offset(v)];
}

template <typename T>
template <typename... Indices>
const T& Tensor<T>::at(Indices... indices) const {
    static_assert(
        (std::is_integral_v<Indices> && ...),
        "Tensor indices must be integral"
    );

    Vector<std::size_t> v{static_cast<std::size_t>(indices)...};

    if (v.size() != ndim()) {
        throw std::invalid_argument("Number of indices does not match tensor dimensions");
    }

    for (std::size_t i = 0; i < shape_.size(); ++i) {
        if (v[i] >= shape_[i]) {
            throw std::out_of_range("Tensor index out of range");
        }
    }

    return data_[compute_offset(v)];
}