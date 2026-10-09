#pragma once
#include <cstddef>
#include <vector>
#include <optional>

// Boyer-Moore majority vote algorithm
// Finds the majority element (more than n/2 copies, n being the size of the vector) in a vector
// in linear time (with 2 passes through the array)

template<class T>
T probable_majority_element(const std::vector<T> &A) {
    T current{};
    std::size_t counter = 0;

    for (const T &x : A) {
        if (counter == 0) {
            current = x;
            counter = 1;
        }
        else if (x == current) {
            ++counter;
        }
        else {
            --counter;
        }
    }

    return current;
};

template<class T>
std::optional<T> majority_element(const std::vector<T> &A) {
    if (A.empty()) return std::nullopt;
    T candidate = probable_majority_element(A);

    std::size_t count = 0;
    for (const T &x : A) {
        if (x == candidate) {
            ++count;
        }
    }

    if (count > A.size() / 2) {
        return candidate;
    }

    return std::nullopt;
};
