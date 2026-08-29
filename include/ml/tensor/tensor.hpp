#pragma once

#include <cstddef>

#include <ml/core/vector.hpp>

template <typename T>
class Tensor {
private:
    Vector<T> data_;
    Vector<std::size_t> shape_;
    Vector<std::size_t> strides_;

    void compute_strides();

    std::size_t compute_offset(
        const Vector<std::size_t>& indices
    ) const;

public:
    Tensor();

    explicit Tensor(const Vector<std::size_t>& shape);

    Tensor(
        const Vector<std::size_t>& shape,
        const T& value
    );

    Tensor(const Tensor&) = default;
    Tensor(Tensor&&) noexcept = default;

    Tensor& operator=(const Tensor&) = default;
    Tensor& operator=(Tensor&&) noexcept = default;

    ~Tensor() = default;

    const Vector<std::size_t>& shape() const noexcept;
    const Vector<std::size_t>& strides() const noexcept;

    std::size_t size() const noexcept;
    std::size_t ndim() const noexcept;

    bool empty() const noexcept;

    T* data() noexcept;
    const T* data() const noexcept;

    template <typename... Indices>
    T& operator()(Indices... indices);

    template <typename... Indices>
    const T& operator()(Indices... indices) const;

    template <typename... Indices>
    T& at(Indices... indices);

    template <typename... Indices>
    const T& at(Indices... indices) const;

    void reshape(const Vector<std::size_t>& new_shape);

    template <typename... Dims>
    void reshape(Dims... dims);

    void flatten();

    Tensor transpose(
        std::size_t dim0,
        std::size_t dim1
    ) const;

    Tensor permute(
        const Vector<std::size_t>& dims
    ) const;

    void fill(const T& value);

    Tensor operator+(const Tensor& other) const;
    Tensor operator-(const Tensor& other) const;
    Tensor operator*(const Tensor& other) const;
    Tensor operator/(const Tensor& other) const;

    Tensor& operator+=(const Tensor& other);
    Tensor& operator-=(const Tensor& other);
    Tensor& operator*=(const Tensor& other);
    Tensor& operator/=(const Tensor& other);

    Tensor matmul(const Tensor& other) const;

    Tensor sum(std::size_t axis, bool keepdim = false) const;
    Tensor mean(std::size_t axis, bool keepdim = false) const;
    Tensor max(std::size_t axis, bool keepdim = false) const;
    Tensor min(std::size_t axis, bool keepdim = false) const;
};

#include "tensor.tpp"