#pragma once
#include <vector>
#include <cstddef>

// size of the bit vector == m = -\frac{n \ln(p)}{(\ln(2))^2}
// number of hash functions == k = \frac{m}{n}\ln(2)

// This template lets the user choose the key type and 
// the hash function to be used on the filter.
template<class Key, class Hash>
class bloom_filter {
private:
    std::vector<bool> bit_vector;
    std::size_t number_of_bits;
    std::size_t number_of_expected_elements;
    std::size_t number_of_hash_functions;
    double target_false_positive_rate;
    Hash hash_function;

public:
    // To create a bloom filter is needed the number of expected elements
    // and the target false positive rate.
    // The optimal size of the bit vector and number of hash functions to be used
    // can be calculated from those values.
    bloom_filter(
        std::size_t number_of_expected_elements,
        double target_false_positive_rate       
    );
    void insert(const Key &key);
    bool probably_contains(const Key &key) const;
};

#include "bloom_filter.tpp"
