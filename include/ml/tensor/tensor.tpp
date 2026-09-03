#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
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
std::size_t Tensor<T>::compute_offset(const Vector<std::size_t>& indices) const {
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
Vector<std::size_t> Tensor<T>::find_place(std::size_t linear) const {
    Vector<std::size_t> result(ndim());

    for (std::size_t i = 0; i < ndim(); ++i) {
        result[i] = linear / strides_[i];
        linear %= strides_[i];
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
    return data_.data();
}

template <typename T>
const T* Tensor<T>::data() const noexcept {
    return data_.data();
}

template <typename T>
template <typename... Indices>
T& Tensor<T>::operator()(Indices... indices) {
    static_assert((std::is_integral_v<Indices> && ...), "Tensor indices must be integral");

    Vector<std::size_t> v{static_cast<std::size_t>(indices)...};

    return data_[compute_offset(v)];
}

template <typename T>
template <typename... Indices>
const T& Tensor<T>::operator()(Indices... indices) const {
    static_assert((std::is_integral_v<Indices> && ...), "Tensor indices must be integral");

    Vector<std::size_t> v{static_cast<std::size_t>(indices)...};

    return data_[compute_offset(v)];
}

template <typename T>
template <typename... Indices>
T& Tensor<T>::at(Indices... indices) {
    static_assert((std::is_integral_v<Indices> && ...), "Tensor indices must be integral");

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
    static_assert((std::is_integral_v<Indices> && ...), "Tensor indices must be integral");

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
void Tensor<T>::reshape(const Vector<std::size_t>& new_shape) {
    if (size() != compute_size(new_shape)) {
        throw std::invalid_argument("The size of the new shape does not match the original size");
    }

    shape_ = new_shape;

    compute_strides();
}

template <typename T>
template <typename... Dims>
void Tensor<T>::reshape(Dims... dims) {
    static_assert((std::is_integral_v<Dims> && ...), "Tensor dimensions must be integral");

    Vector<std::size_t> new_shape{static_cast<std::size_t>(dims)...};

    reshape(new_shape);
}

template <typename T>
void Tensor<T>::flatten() {
    reshape(size());
}

template <typename T>
Tensor<T> Tensor<T>::transpose(std::size_t dim0, std::size_t dim1) const {
    if (dim0 >= shape_.size() || dim1 >= shape_.size()) {
        throw std::out_of_range("Wrong index");
    }

    Vector<std::size_t> new_shape(shape_);

    std::swap(new_shape[dim0], new_shape[dim1]);

    Tensor<T> new_tensor(new_shape);

    for (std::size_t linear = 0; linear < size(); ++linear) {
        Vector<std::size_t> old_place = find_place(linear);
        Vector<std::size_t> new_place = old_place;

        std::swap(new_place[dim0], new_place[dim1]);

        auto new_linear = new_tensor.compute_offset(new_place);

        new_tensor.data_[new_linear] = data_[linear];
    }

    return new_tensor;
}

template <typename T>
Tensor<T> Tensor<T>::permute(const Vector<std::size_t>& dims) const {
    if (dims.size() != ndim()) {
        throw std::invalid_argument("Permutation rank does not match tensor rank");
    }
    
    Vector<std::size_t> new_shape(shape_.size());
    Vector<bool> used(ndim(), false);

    for (std::size_t i = 0; i < dims.size(); ++i) {
        if (dims[i] >= shape_.size()) {
            throw std::out_of_range("Permutation dimension out of range");
        }

        if (used[dims[i]]) {
            throw std::invalid_argument("Permutation contains duplicate dimensions");
        }

        used[dims[i]] = true;
        new_shape[i] = shape_[dims[i]];
    }

    Tensor<T> new_tensor(new_shape);

    for (std::size_t linear = 0; linear < size(); ++linear) {
        Vector<std::size_t> old_place = find_place(linear);
        Vector<std::size_t> new_place(ndim());

        for (std::size_t i = 0; i < ndim(); ++i) {
            new_place[i] = old_place[dims[i]];
        }

        const auto new_linear = new_tensor.compute_offset(new_place);

        new_tensor.data_[new_linear] = data_[linear];
    }

    return new_tensor;
}

template <typename T>
void Tensor<T>::fill(const T& value) {
    for (auto &i : data_) {
        i = value;
    }
}

template <typename T>
Tensor<T> Tensor<T>::operator+(const Tensor& other) const {
    if (this->shape_ != other.shape_) {
        throw std::invalid_argument("Tensor shapes do not match");
    }

    Tensor<T> new_tensor(shape_);

    for (std::size_t i = 0; i < size(); ++i) {
        new_tensor.data_[i] = this->data_[i] + other.data_[i];
    }

    return new_tensor;
}