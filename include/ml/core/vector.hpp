#pragma once

#include <cstddef>
#include <initializer_list>
#include <iostream>

template <typename T>
class Vector {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    T* allocate(std::size_t n);
    void deallocate(T* ptr) noexcept;

    void destroy_range(std::size_t first, std::size_t last) noexcept;
    void destroy_range(T* data, std::size_t first, std::size_t last) noexcept;

public:
    Vector();
    explicit Vector(std::size_t n);
    Vector(std::size_t n, const T& value);
    Vector(std::initializer_list<T> list);

    Vector(const Vector& other);
    Vector& operator=(const Vector& other);
    Vector& operator=(std::initializer_list<T> list);

    Vector(Vector&& other) noexcept;
    Vector& operator=(Vector&& other) noexcept;

    ~Vector();

    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    bool empty() const noexcept;

    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;

    T& at(std::size_t index);
    const T& at(std::size_t index) const;

    T& front();
    const T& front() const;

    T& back();
    const T& back() const;

    T* data() noexcept;
    const T* data() const noexcept;

    void reserve(std::size_t new_capacity);

    void resize(std::size_t new_size);
    void resize(std::size_t new_size, const T& value);

    void shrink_to_fit();

    void clear() noexcept;

    void swap(Vector& other) noexcept;

    void push_back(const T& value);
    void push_back(T&& value);
    template <typename... Args>
    T& emplace_back(Args&&... args);

    void pop_back();

    void print() const;
};

#include "vector.tpp"