#include "../../include/ml/models/model.hpp"

#include <utility>
#include <stdexcept>

namespace ml {

Model::Model(std::string id, Sequential<float> network)
    : id_(std::move(id)), network_(std::move(network)) {
    if (id_.empty()) {
        throw std::invalid_argument("Model id cannot be empty");
    }

    if (network_.empty()) {
        throw std::invalid_argument("Model network cannot be empty");
    }
}

const std::string& Model::id() const noexcept {
    return id_;
}

Tensor<float> Model::forward(const Tensor<float>& input) const {
    return network_.forward(input);
}

}  // namespace ml