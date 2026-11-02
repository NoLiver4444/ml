#pragma once

#include <string>


#include <ml/tensor/tensor.hpp>
#include "ml/models/sequential.hpp"

namespace ml {

class Model {
private:
    std::string id_;
    Sequential<float> network_;

public:
    Model(std::string id, Sequential<float> network);

    const std::string& id() const noexcept;

    Tensor<float> forward(const Tensor<float>& input) const;
};

}  // namespace ml