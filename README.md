# ML from Scratch in C++

A machine learning framework written completely from scratch in modern C++.

The goal of this project is not to create another production-ready ML library, but to deeply understand how machine learning frameworks work internally by implementing their core components manually.

No STL containers for the core data structures.  
No Eigen.  
No PyTorch.  
No TensorFlow.  
No third-party math libraries.

Core containers, tensor operations, neural network layers, model composition and memory management are implemented manually.

---

# Project Goals

- Learn low-level memory management
- Build STL-like containers from scratch
- Implement linear algebra
- Build multidimensional tensors
- Understand tensor shapes and strides
- Implement neural network layers
- Build composable neural networks
- Implement model inference
- Build an automatic differentiation engine
- Implement optimizers
- Implement loss functions
- Train neural networks without external ML libraries
- Understand every layer of a modern ML framework

---

# Current Progress

## Containers

- [x] Vector
- [x] Matrix
- [x] Tensor

## Tensor Operations

- [x] Shape
- [x] Strides
- [x] Rank / ndim
- [x] Element access
- [x] Checked element access
- [x] Reshape
- [x] Flatten
- [x] Transpose
- [x] Permute
- [x] Fill
- [x] Tensor addition
- [x] Matrix multiplication
- [ ] Broadcasting
- [ ] Reduction operations
- [ ] Slicing

## Linear Algebra

- [x] Matrix Multiplication
- [x] Transpose
- [ ] Broadcasting
- [ ] Dot Product
- [ ] Matrix Decomposition

## Layers

- [x] Layer abstraction
- [x] Linear
- [x] ReLU
- [x] Sigmoid
- [ ] Softmax
- [ ] LayerNorm
- [ ] Dropout

## Models

- [x] Sequential
- [x] Model
- [x] Forward inference
- [x] Model identifier
- [ ] Model serialization
- [ ] Weight loading from file
- [ ] Model metadata

## Autograd

- [ ] Computational Graph
- [ ] Gradient Storage
- [ ] Backward Propagation
- [ ] Automatic Differentiation

## Loss Functions

- [ ] MSE
- [ ] Binary Cross Entropy
- [ ] Cross Entropy

## Optimizers

- [ ] SGD
- [ ] Momentum
- [ ] Adam

## Neural Networks

- [x] Basic feed-forward network composition
- [ ] Trainable MLP
- [ ] CNN
- [ ] Transformer

---

# Architecture

The current inference architecture of the library is:

```text
Tensor
  ↓
Layer<T>
  ↓
Linear / ReLU / Sigmoid
  ↓
Sequential<T>
  ↓
Model
```

A simple network can be composed as:

```text
Input
  ↓
Linear
  ↓
ReLU
  ↓
Linear
  ↓
Sigmoid
  ↓
Prediction
```

Example:

```cpp
ml::Sequential<float> network;

network.add(
    std::make_unique<ml::Linear<float>>(
        weights_1,
        bias_1
    )
);

network.add(
    std::make_unique<ml::ReLU<float>>()
);

network.add(
    std::make_unique<ml::Linear<float>>(
        weights_2,
        bias_2
    )
);

network.add(
    std::make_unique<ml::Sigmoid<float>>()
);

ml::Model model(
    "binary_classifier",
    std::move(network)
);

auto output = model.forward(input);
```

---

# Repository Structure

```text
ml/
├── CMakeLists.txt
├── CMakePresets.json
├── .clang-format
├── .gitignore
├── README.md
│
├── include/
│   └── ml/
│       ├── core/
│       │   ├── vector.hpp
│       │   ├── vector.tpp
│       │   ├── matrix.hpp
│       │   └── matrix.tpp
│       │
│       ├── tensor/
│       │   ├── tensor.hpp
│       │   └── tensor.tpp
│       │
│       ├── layers/
│       │   ├── layer.hpp
│       │   ├── linear.hpp
│       │   ├── linear.tpp
│       │   ├── relu.hpp
│       │   ├── relu.tpp
│       │   ├── sigmoid.hpp
│       │   └── sigmoid.tpp
│       │
│       └── models/
│           ├── sequential.hpp
│           ├── sequential.tpp
│           └── model.hpp
│
├── src/
│   └── models/
│       └── model.cpp
│
├── test/
│   ├── vector/
│   ├── matrix/
│   ├── tensor/
│   ├── layers/
│   └── models/
│
├── examples/
│
└── docs/
```

---

# Vector

The project contains a custom dynamically allocated container implemented without `std::vector`.

Implemented features include:

- Rule of Five
- Raw memory allocation
- Placement new
- Manual object lifetime management
- Copy semantics
- Move semantics
- Exception safety
- `reserve()`
- `resize()`
- `shrink_to_fit()`
- `push_back()`
- lvalue and rvalue insertion
- `emplace_back()`
- `pop_back()`
- `swap()`
- `clear()`
- Iterators
- Capacity management

The container is used internally by other parts of the framework.

---

# Matrix

A custom matrix implementation is used as part of the low-level mathematical foundation of the framework.

The Matrix component is intended to provide explicit control over storage and linear algebra operations without relying on external numerical libraries.

---

# Tensor

`Tensor<T>` represents multidimensional data.

It stores:

```text
data
shape
strides
```

Implemented functionality includes:

- Dynamic tensor storage
- Arbitrary rank
- Shape information
- Stride calculation
- Tensor size
- Rank through `ndim()`
- Empty-state checks
- Raw data access
- Unchecked element access
- Checked access through `at()`
- Reshape
- Flatten
- Transpose
- Axis permutation
- Fill
- Tensor addition
- Rank-2 matrix multiplication

Example:

```cpp
ml::Tensor<float> input({2, 3});

input(0, 0) = 1.0f;
input(0, 1) = 2.0f;
input(0, 2) = 3.0f;

input(1, 0) = 4.0f;
input(1, 1) = 5.0f;
input(1, 2) = 6.0f;
```

For matrix multiplication:

```text
A: [M, K]
B: [K, N]

A × B = [M, N]
```

---

# Layer Abstraction

All neural network layers implement a common interface:

```cpp
template <typename T>
class Layer {
public:
    virtual Tensor<T> forward(
        const Tensor<T>& input
    ) const = 0;

    virtual ~Layer() = default;
};
```

This allows different layer types to be stored and executed through the same interface.

---

# Linear Layer

The `Linear<T>` layer implements a fully connected transformation.

Tensor convention:

```text
input   [batch, in_features]
weights [in_features, out_features]
bias    [out_features]

output  [batch, out_features]
```

Operation:

```text
Y = XW + b
```

The layer validates:

- input rank
- weight rank
- bias rank
- feature dimensions
- bias size

Weights and bias can be supplied explicitly.

---

# ReLU

ReLU is implemented as a stateless element-wise activation.

```text
ReLU(x) = max(0, x)
```

Properties:

- preserves tensor shape
- supports arbitrary tensor rank
- does not modify the input tensor
- creates a new output tensor

---

# Sigmoid

Sigmoid is implemented for floating-point tensors.

```text
sigmoid(x) = 1 / (1 + exp(-x))
```

Properties:

- element-wise operation
- preserves tensor shape
- supports arbitrary rank
- does not modify the input
- restricted to floating-point types

---

# Sequential

`Sequential<T>` allows multiple layers to be composed into one network.

Internally it stores:

```cpp
Vector<std::unique_ptr<Layer<T>>>
```

Each layer owns its parameters, while `Sequential` owns the layers.

Forward execution follows:

```text
input
  ↓
layer 1
  ↓
layer 2
  ↓
...
  ↓
output
```

An empty `Sequential` behaves as an identity operation:

```text
f(x) = x
```

`Sequential` is:

- non-copyable
- movable
- responsible for layer ownership

---

# Model

`Model` represents a complete machine learning model.

It contains:

```text
model ID
+
Sequential<float>
```

Example:

```cpp
ml::Model model(
    "classifier_v1",
    std::move(network)
);
```

Inference:

```cpp
auto prediction = model.forward(input);
```

The model validates:

- model ID must not be empty
- network must contain at least one layer

`Model` provides a higher-level abstraction over the underlying neural network architecture.

---

# Ownership Model

The library intentionally uses explicit ownership semantics.

For example:

```text
Sequential
   ↓ owns
unique_ptr<Layer>
```

This guarantees that each layer has a single owner.

A complete model owns its `Sequential` network:

```text
Model
  ↓
Sequential
  ↓
unique_ptr<Layer>
```

This design avoids unnecessary copying and makes object lifetime explicit.

---

# Testing

GoogleTest is used for unit testing.

The project contains tests for:

- Vector
- Matrix
- Tensor
- Linear
- ReLU
- Sigmoid
- Sequential
- Model

Test coverage includes:

- Constructors
- Copy semantics
- Move semantics
- Exception safety
- Lifetime management
- Tensor shape
- Tensor strides
- Element access
- Reshape
- Transpose
- Matrix multiplication
- Layer validation
- Layer forward passes
- Activation functions
- Sequential execution
- Empty Sequential behavior
- Null layer rejection
- Complete network inference
- Model ID validation
- Model network validation
- Model forward execution

An end-to-end test currently verifies a network of the form:

```text
Input
  ↓
Linear(2, 2)
  ↓
ReLU
  ↓
Linear(2, 1)
  ↓
Sigmoid
  ↓
Output
```

---

# Build

Configure:

```bash
cmake -S . -B build
```

Build:

```bash
cmake --build build
```

Run tests:

```bash
ctest --test-dir build --output-on-failure
```

Run GoogleTest executable directly:

```bash
./build/ml_tests
```

---

# CMake Presets

The project provides several build configurations.

## Debug

```bash
cmake --preset debug
cmake --build --preset debug
ctest --test-dir build --output-on-failure
```

## AddressSanitizer + UBSan

```bash
cmake --preset asan
cmake --build --preset asan
ctest --test-dir build-asan --output-on-failure
```

## clang-tidy

```bash
cmake --preset tidy
cmake --build --preset tidy
```

## Coverage

```bash
cmake --preset coverage
cmake --build --preset coverage --target coverage
```

---

# Formatting

Format all project source files:

```bash
cmake --build --preset debug --target format
```

Check formatting without modifying files:

```bash
cmake --build --preset debug --target format-check
```

The project uses `clang-format`.

---

# Development Tools

The project supports:

- GoogleTest
- clang-format
- clang-tidy
- AddressSanitizer
- UndefinedBehaviorSanitizer
- gcovr
- CMake presets
- `compile_commands.json`

---

# Requirements

- C++17
- CMake 3.16+
- GCC or Clang
- GoogleTest
- clang-format

Optional:

- clang-tidy
- gcovr
- AddressSanitizer
- UndefinedBehaviorSanitizer

---

# Roadmap

1. [x] Vector
2. [x] Matrix
3. [x] Tensor
4. [x] Basic Linear Algebra
5. [x] Layer abstraction
6. [x] Linear layer
7. [x] ReLU
8. [x] Sigmoid
9. [x] Sequential
10. [x] Model
11. [ ] Broadcasting
12. [ ] Loss Functions
13. [ ] Computational Graph
14. [ ] Automatic Differentiation
15. [ ] Backward Propagation
16. [ ] SGD
17. [ ] Adam
18. [ ] Trainable MLP
19. [ ] CNN
20. [ ] Transformer

---

# Related Project

This library is also used as the ML backend for a separate C++ inference server project.

The inference server uses the library to:

```text
receive input
    ↓
select model
    ↓
run inference
    ↓
return prediction
```

The ML framework itself remains independent from networking, HTTP, databases and server logic.

---

# Why?

The purpose of this project is educational.

Instead of hiding implementation details behind existing machine learning frameworks, the project exposes and manually implements the underlying mechanisms:

```text
memory management
      ↓
containers
      ↓
linear algebra
      ↓
tensors
      ↓
layers
      ↓
models
      ↓
autograd
      ↓
training
```

The long-term goal is to understand how a machine learning framework works from memory allocation all the way to neural network training and inference.

---

# License

This project is licensed under the MIT License.