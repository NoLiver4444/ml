#pragma once

#include <cstddef>
#include <ml/layers/layer.hpp>

namespace ml {

template <typename T>
class Linear : public Layer<T> {
private:
    Tensor<T> weights_;
    Tensor<T> bias_;

    static Tensor<T> makeWeights(std::size_t in_features, std::size_t out_features);

public:
    Linear(std::size_t in_features, std::size_t out_features);
    Linear(const Tensor<T>& weights, const Tensor<T>& bias);

    Tensor<T> forward(const Tensor<T>& input) const override;

    const Tensor<T>& weights() const noexcept;
    const Tensor<T>& bias() const noexcept;

    std::size_t inFeatures() const noexcept;
    std::size_t outFeatures() const noexcept;
};

}  // namespace ml

#include "linear.tpp"