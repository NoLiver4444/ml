# ML from Scratch in C++

A machine learning framework written completely from scratch in modern C++.

The goal of this project is not to create another ML library, but to deeply understand how machine learning frameworks work internally by implementing every major component manually.

No STL containers for the core data structures.
No Eigen.
No PyTorch.
No TensorFlow.
No third-party math libraries.

Everything is implemented manually, including memory management, linear algebra, automatic differentiation, neural network layers and optimizers.

---

# Project Goals

- Learn low-level memory management
- Build STL-like containers
- Implement linear algebra
- Build an automatic differentiation engine
- Implement neural network layers
- Implement optimizers
- Train neural networks without external libraries
- Understand every layer of a modern ML framework

---

# Current Progress

## Containers

- [x] Vector
- [ ] Matrix
- [ ] Tensor

## Linear Algebra

- [ ] Matrix Multiplication
- [ ] Transpose
- [ ] Broadcasting

## Autograd

- [ ] Computational Graph
- [ ] Backward Propagation

## Layers

- [ ] Linear
- [ ] ReLU
- [ ] Sigmoid
- [ ] Softmax
- [ ] LayerNorm

## Loss Functions

- [ ] MSE
- [ ] CrossEntropy

## Optimizers

- [ ] SGD
- [ ] Momentum
- [ ] Adam

## Neural Networks

- [ ] MLP
- [ ] CNN
- [ ] Transformer

---

# Repository Structure

```text
include/
    ml/
        core/
        math/
            vector.hpp
            vector.tpp
            matrix.hpp
            matrix.tpp
        tensor/
        autograd/
        layers/
        optimizers/

src/

test/
    vector/
    matrix/

examples/

docs/
```

---

# Vector

The first completed component of the project is a custom implementation of a dynamic array.

Implemented features:

- Rule of Five
- Raw memory allocation
- Placement new
- Manual object lifetime management
- Copy semantics
- Move semantics
- Exception safety
- reserve()
- resize()
- shrink_to_fit()
- push_back()
- emplace_back()
- pop_back()
- swap()
- clear()

---

# Testing

GoogleTest is used for unit testing.

Current test coverage includes:

- Constructors
- Copy / Move semantics
- Exception safety
- Lifetime management
- Edge cases
- Self-reference tests
- Memory reallocation
- Capacity management

---

# Build

```bash
cmake -S . -B build
cmake --build build
```

Run tests

```bash
ctest --test-dir build --output-on-failure
```

---

# Requirements

- C++17
- CMake 3.16+
- GCC / Clang
- GoogleTest

---

# Roadmap

1. Vector
2. Matrix
3. Tensor
4. Linear Algebra
5. Autograd
6. Neural Network Layers
7. Optimizers
8. Loss Functions
9. MLP
10. CNN
11. Transformer

---

# Why?

This project is intended as a learning project focused on understanding how machine learning frameworks are implemented internally rather than providing production-ready performance.

---

# License

This project is licensed under the MIT License.