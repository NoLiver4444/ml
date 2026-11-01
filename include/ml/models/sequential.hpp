#pragma once

#include <memory>
#include <ml/layers/layer.hpp>
#include <ml/core/vector.hpp>

namespace ml {

template <typename T>
class Sequential {
private:
    Vector<std::unique_ptr<Layer<T>>> layers_;

public:
    Sequential() = default;

    Sequential(const Sequential&) = delete;
    Sequential& operator=(const Sequential&) = delete;

    Sequential(Sequential&&) noexcept = default;
    Sequential& operator=(Sequential&&) noexcept = default;

    void add(std::unique_ptr<Layer<T>> layer);

    Tensor<T> forward(const Tensor<T>& input) const;

    std::size_t size() const noexcept;
    bool empty() const noexcept;
};

}  // namespace ml

#include "sequential.tpp"