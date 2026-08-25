#pragma once

#include <cstddef>
#include "matrix.hpp"

template <typename T>
Matrix<T>::Matrix() : data_(), rows_(0), cols_(0) {}

template <typename T>
Matrix<T>::Matrix(std::size_t rows, std::size_t cols)
    : data_(rows * cols), rows_(rows), cols_(cols) {}

template <typename T>
Matrix<T>::Matrix(std::size_t rows, std::size_t cols, const T& value) : Matrix(rows, cols) {
    fill(value);
}

template <typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> values)
    : data_(), rows_(values.size()), cols_(0) {
    if (values.size() == 0) {
        return;
    }

    cols_ = values.begin()->size();

    for (const auto& row : values) {
        if (row.size() != cols_) {
            throw std::invalid_argument("Matrix rows must have equal sizes");
        }
    }

    data_.resize(rows_ * cols_);

    std::size_t i = 0;

    for (const auto& rows : values) {
        std::size_t j = 0;

        for (const auto& value : rows) {
            data_[i * cols_ + j] = value;
            ++j;
        }
        ++i;
    }
}