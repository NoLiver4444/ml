#include <gtest/gtest.h>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

#include "../../include/ml/math/vector.hpp"

class ThrowOnCopy {
public:
    static inline int alive = 0;
    static inline int copies = 0;
    static inline int throw_on_copy = -1;

    int value = 0;

    ThrowOnCopy() {
        ++alive;
    }

    explicit ThrowOnCopy(int value) : value(value) {
        ++alive;
    }

    ThrowOnCopy(const ThrowOnCopy& other) : value(other.value) {
        ++copies;

        if (throw_on_copy > 0 && copies == throw_on_copy) {
            throw std::runtime_error("Copy constructor failed");
        }

        ++alive;
    }

    // Специально НЕ noexcept.
    // Тогда move_if_noexcept предпочтет copy constructor.
    ThrowOnCopy(ThrowOnCopy&& other) noexcept(false) : value(other.value) {
        ++alive;
    }

    ThrowOnCopy& operator=(const ThrowOnCopy&) = default;
    ThrowOnCopy& operator=(ThrowOnCopy&&) = default;

    ~ThrowOnCopy() {
        --alive;
    }

    static void disable_throwing() {
        copies = 0;
        throw_on_copy = -1;
    }

    static void throw_on(int copy_number) {
        copies = 0;
        throw_on_copy = copy_number;
    }
};