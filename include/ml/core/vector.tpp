#pragma once

#include <cstddef>
#include <stdexcept>
#include "vector.hpp"

template <typename T>
Vector<T>::Vector() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>
Vector<T>::Vector(std::size_t n) : data_(allocate(n)), size_(0), capacity_(n) {
    try {
        for (; size_ < n; ++size_) {
            new (data_ + size_) T();
        }
    } catch (...) {
        destroy_range(0, size_);
        deallocate(data_);
        throw;
    }
}

template <typename T>
Vector<T>::Vector(std::size_t n, const T& value) : data_(allocate(n)), size_(0), capacity_(n) {
    try {
        for (; size_ < n; ++size_) {
            new (data_ + size_) T(value);
        }
    } catch (...) {
        destroy_range(0, size_);
        deallocate(data_);
        throw;
    }
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> list)
    : data_(allocate(list.size())), size_(0), capacity_(list.size()) {
    try {
        for (const auto& value : list) {
            new (data_ + size_) T(value);
            ++size_;
        }
    } catch (...) {
        destroy_range(0, size_);
        deallocate(data_);
        throw;
    }
}

template <typename T>
Vector<T>::Vector(const Vector& other)
    : data_(allocate(other.size_)), size_(0), capacity_(other.size_) {
    try {
        for (; size_ < other.size_; ++size_) {
            new (data_ + size_) T(other.data_[size_]);
        }
    } catch (...) {
        destroy_range(0, size_);
        deallocate(data_);
        throw;
    }
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this == &other) {
        return *this;
    }

    Vector temp(other);
    swap(temp);

    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(std::initializer_list<T> list) {
    Vector temp(list);
    swap(temp);

    return *this;
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    destroy_range(0, size_);
    deallocate(data_);

    this->data_ = other.data_;
    this->size_ = other.size_;
    this->capacity_ = other.capacity_;

    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;

    return *this;
}

template <typename T>
Vector<T>::~Vector() {
    destroy_range(0, size_);
    deallocate(data_);
}

template <typename T>
std::size_t Vector<T>::size() const noexcept {
    return size_;
}

template <typename T>
std::size_t Vector<T>::capacity() const noexcept {
    return capacity_;
}

template <typename T>
bool Vector<T>::empty() const noexcept {
    return size_ == 0;
}

template <typename T>
T& Vector<T>::operator[](std::size_t index) {
    return data_[index];
}

template <typename T>
const T& Vector<T>::operator[](std::size_t index) const {
    return data_[index];
}

template <typename T>
T& Vector<T>::at(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }

    return data_[index];
}

template <typename T>
const T& Vector<T>::at(std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }

    return data_[index];
}

template <typename T>
T& Vector<T>::front() {
    if (empty()) {
        throw std::out_of_range("Empty vector");
    }

    return data_[0];
}

template <typename T>
const T& Vector<T>::front() const {
    if (empty()) {
        throw std::out_of_range("Empty vector");
    }

    return data_[0];
}

template <typename T>
T& Vector<T>::back() {
    if (empty()) {
        throw std::out_of_range("Empty vector");
    }

    return data_[size_ - 1];
}

template <typename T>
const T& Vector<T>::back() const {
    if (empty()) {
        throw std::out_of_range("Empty vector");
    }

    return data_[size_ - 1];
}

template <typename T>
T* Vector<T>::data() noexcept {
    return data_;
}

template <typename T>
const T* Vector<T>::data() const noexcept {
    return data_;
}

template <typename T>
void Vector<T>::reserve(std::size_t new_capacity) {
    if (new_capacity <= capacity_) {
        return;
    }

    auto new_data = allocate(new_capacity);

    std::size_t constructed = 0;

    try {
        for (; constructed < size_; ++constructed) {
            new (new_data + constructed) T(std::move_if_noexcept(data_[constructed]));
        }
    } catch (...) {
        destroy_range(new_data, 0, constructed);
        deallocate(new_data);
        throw;
    }

    destroy_range(0, size_);

    deallocate(data_);

    data_ = new_data;
    capacity_ = new_capacity;
}

template <typename T>
void Vector<T>::resize(std::size_t new_size) {
    if (new_size == size_) {
        return;
    }

    if (new_size < size_) {
        destroy_range(new_size, size_);
        size_ = new_size;
        return;
    }

    if (new_size > capacity_) {
        reserve(new_size);
    }

    while (size_ < new_size) {
        new (data_ + size_) T();
        ++size_;
    }
}

template <typename T>
void Vector<T>::resize(std::size_t new_size, const T& value) {
    if (new_size == size_) {
        return;
    }

    if (new_size < size_) {
        destroy_range(new_size, size_);
        size_ = new_size;
        return;
    }

    if (new_size > capacity_) {
        reserve(new_size);
    }

    while (size_ < new_size) {
        new (data_ + size_) T(value);
        ++size_;
    }
}

template <typename T>
void Vector<T>::shrink_to_fit() {
    if (size_ == capacity_) {
        return;
    }

    if (size_ == 0) {
        deallocate(data_);

        data_ = nullptr;
        capacity_ = 0;

        return;
    }

    auto new_data = allocate(size_);

    std::size_t constructed = 0;

    try {
        for (; constructed < size_; ++constructed) {
            new (new_data + constructed) T(std::move_if_noexcept(data_[constructed]));
        }
    } catch (...) {
        destroy_range(new_data, 0, constructed);

        deallocate(new_data);
        throw;
    }

    destroy_range(0, size_);

    deallocate(data_);

    data_ = new_data;
    capacity_ = size_;
}

template <typename T>
void Vector<T>::clear() noexcept {
    destroy_range(0, size_);

    size_ = 0;
}

template <typename T>
void Vector<T>::swap(Vector& other) noexcept {
    std::swap(this->data_, other.data_);
    std::swap(this->size_, other.size_);
    std::swap(this->capacity_, other.capacity_);
}

template <typename T>
void Vector<T>::push_back(const T& value) {
    if (size_ == capacity_) {
        T temp(value);

        const std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;

        reserve(new_capacity);

        new (data_ + size_) T(std::move(temp));
    } else {
        new (data_ + size_) T(value);
    }

    ++size_;
}

template <typename T>
void Vector<T>::push_back(T&& value) {
    if (size_ == capacity_) {
        T temp(std::move(value));

        const std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;

        reserve(new_capacity);

        new (data_ + size_) T(std::move(temp));
    } else {
        new (data_ + size_) T(std::move(value));
    }

    ++size_;
}

template <typename T>
template <typename... Args>
T& Vector<T>::emplace_back(Args&&... args) {
    if (size_ == capacity_) {
        std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
        reserve(new_capacity);
    }

    new (data_ + size_) T(std::forward<Args>(args)...);
    ++size_;

    return data_[size_ - 1];
}

template <typename T>
void Vector<T>::pop_back() {
    if (empty()) {
        throw std::out_of_range("Empty vector");
    }

    --size_;
    data_[size_].~T();
}

template <typename T>
void Vector<T>::print() const {
    for (std::size_t i = 0; i < size_; ++i) {
        std::cout << data_[i] << ' ';
    }

    std::cout << '\n';
}

template <typename T>
T* Vector<T>::allocate(std::size_t n) {
    if (n == 0) {
        return nullptr;
    }

    if (n > static_cast<std::size_t>(-1) / sizeof(T)) {
        throw std::bad_array_new_length();
    }

    return static_cast<T*>(::operator new(sizeof(T) * n));
}

template <typename T>
void Vector<T>::deallocate(T* ptr) noexcept {
    ::operator delete(ptr);
}

template <typename T>
void Vector<T>::destroy_range(std::size_t first, std::size_t last) noexcept {
    destroy_range(data_, first, last);
}

template <typename T>
void Vector<T>::destroy_range(T* data, std::size_t first, std::size_t last) noexcept {
    for (; first < last; ++first) {
        data[first].~T();
    }
}

template <typename T>
T* Vector<T>::begin() noexcept {
    return data_;
}

template <typename T>
const T* Vector<T>::begin() const noexcept {
    return data_;
}

template <typename T>
T* Vector<T>::end() noexcept {
    return data_ + size_;
}

template <typename T>
const T* Vector<T>::end() const noexcept {
    return data_ + size_;
}

template <typename T>
bool Vector<T>::operator==(const Vector& other) const {
    if (size_ != other.size_) {
        return false;
    }

    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] != other[i]) {
            return false;
        }
    }

    return true;
}

template <typename T>
bool Vector<T>::operator!=(const Vector& other) const {
    return !(*this == other);
}