#pragma once

#include <cstddef>
#include <initializer_list>

#include <ml/core/vector.hpp>

template <typename T>
class Matrix {
private:
    Vector<T> data_;
    std::size_t rows_;
    std::size_t cols_;

public:
    Matrix();

    Matrix(std::size_t rows, std::size_t cols);

    Matrix(std::size_t rows, std::size_t cols, const T& value);

    Matrix(std::initializer_list<std::initializer_list<T>> values);

    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;

    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;

    ~Matrix() = default;

    std::size_t rows() const noexcept;
    std::size_t cols() const noexcept;
    std::size_t size() const noexcept;

    bool empty() const noexcept;

    T& operator()(std::size_t row, std::size_t col);

    const T& operator()(std::size_t row, std::size_t col) const;

    T& at(std::size_t row, std::size_t col);

    const T& at(std::size_t row, std::size_t col) const;

    T* data() noexcept;
    const T* data() const noexcept;

    void fill(const T& value);

    Matrix transpose() const;

    void swap(Matrix& other) noexcept;

    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;

    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);

    Matrix operator*(const T& scalar) const;
    Matrix operator/(const T& scalar) const;

    Matrix& operator*=(const T& scalar);
    Matrix& operator/=(const T& scalar);

    Matrix matmul(const Matrix& other) const;
};

#include "matrix.tpp"