#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
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

template <typename T>
Matrix<T>::Matrix(const Matrix& other)
    : data_(other.data_), rows_(other.rows_), cols_(other.cols_) {}

template <typename T>
Matrix<T>::Matrix(Matrix&& other) noexcept
    : data_(std::move(other.data_)), rows_(other.rows_), cols_(other.cols_) {
    other.rows_ = 0;
    other.cols_ = 0;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other) {
    if (this == &other) {
        return *this;
    }

    this->data_ = other.data_;
    this->rows_ = other.rows_;
    this->cols_ = other.cols_;

    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    this->data_ = std::move(other.data_);
    this->rows_ = other.rows_;
    this->cols_ = other.cols_;

    other.rows_ = 0;
    other.cols_ = 0;

    return *this;
}

template <typename T>
std::size_t Matrix<T>::rows() const noexcept {
    return rows_;
}

template <typename T>
std::size_t Matrix<T>::cols() const noexcept {
    return cols_;
}

template <typename T>
std::size_t Matrix<T>::size() const noexcept {
    return rows_ * cols_;
}

template <typename T>
bool Matrix<T>::empty() const noexcept {
    return size() == 0;
}

template <typename T>
T& Matrix<T>::operator()(std::size_t row, std::size_t col) {
    return data_[row * cols_ + col];
}

template <typename T>
const T& Matrix<T>::operator()(std::size_t row, std::size_t col) const {
    return data_[row * cols_ + col];
}

template <typename T>
T& Matrix<T>::at(std::size_t row, std::size_t col) {
    if (row >= rows_ || col >= cols_) {
        throw std::out_of_range("Matrix index out of range");
    }

    return (*this)(row, col);
}

template <typename T>
const T& Matrix<T>::at(std::size_t row, std::size_t col) const {
    if (row >= rows_ || col >= cols_) {
        throw std::out_of_range("Matrix index out of range");
    }

    return (*this)(row, col);
}

template <typename T>
T* Matrix<T>::data() noexcept {
    return data_.data();
}

template <typename T>
const T* Matrix<T>::data() const noexcept {
    return data_.data();
}

template <typename T>
void Matrix<T>::fill(const T& value) {
    for (auto& i : data_) {
        i = value;
    }
}

template <typename T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix<T> result(cols_, rows_);

    for (std::size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            result(j, i) = (*this)(i, j);
        }
    }

    return result;
}

template <typename T>
void Matrix<T>::swap(Matrix& other) noexcept {
    this->data_.swap(other.data_);
    std::swap(this->rows_, other.rows_);
    std::swap(this->cols_, other.cols_);
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix& other) const {
    Matrix<T> result(*this);
    result += other;

    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix& other) const {
    Matrix<T> result(*this);
    result -= other;

    return result;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix& other) {
    if (this->rows_ != other.rows_ || this->cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions do not match");
    }

    for (std::size_t i = 0; i < size(); ++i) {
        this->data_[i] += other.data_[i];
    }

    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix& other) {
    if (this->rows_ != other.rows_ || this->cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions do not match");
    }

    for (std::size_t i = 0; i < size(); ++i) {
        this->data_[i] -= other.data_[i];
    }

    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T& scalar) const {
    Matrix<T> result(*this);
    result *= scalar;

    return result;
}

template <typename T>
Matrix<T> operator*(const T& scalar, const Matrix<T>& matrix) {
    return matrix * scalar;
}

template <typename T>
Matrix<T> Matrix<T>::operator/(const T& scalar) const {
    Matrix<T> result(*this);
    result /= scalar;

    return result;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const T& scalar) {
    for (auto& i : data_) {
        i *= scalar;
    }

    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator/=(const T& scalar) {
    if (scalar == T{}) {
        throw std::domain_error("Division by zero");
    }

    for (auto& i : data_) {
        i /= scalar;
    }

    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::matmul(const Matrix& other) const {
    if (this->cols_ != other.rows_) {
        throw std::invalid_argument("Matrix dimensions do not match");
    }

    Matrix<T> result(this->rows_, other.cols_);

    for (std::size_t i = 0; i < this->rows_; ++i) {
        for (std::size_t j = 0; j < other.cols_; ++j) {
            T sum{};

            for (std::size_t k = 0; k < cols_; ++k) {
                sum += (*this)(i, k) * other(k, j);
            }

            result(i, j) = sum;
        }
    }

    return result;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix& other) const {
    if (this->rows_ != other.rows_ || this->cols_ != other.cols_) {
        return false;
    }

    for (std::size_t i = 0; i < size(); ++i) {
        if (this->data_[i] != other.data_[i]) {
            return false;
        }
    }

    return true;
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix& other) const {
    return !(*this == other);
}