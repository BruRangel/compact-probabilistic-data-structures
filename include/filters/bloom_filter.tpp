#pragma once
#include <algorithm>
#include <cmath>
#include "bloom_filter.hpp"

template<class Key, class Hash>
bloom_filter<Key, Hash>::bloom_filter(
    std::size_t n,
    double p
) : number_of_expected_elements(n), target_false_positive_rate(p)
{
    // size of the bit vector == m = -\frac{n \ln(p)}{(\ln(2))^2}
    this->number_of_bits = std::ceil(
    -(n * std::log(p))
    /(std::pow(std::log(2.0), 2))
    );

    this->bit_vector.assign(this->number_of_bits, false);

    const auto m = this->number_of_bits;

    // number of hash functions == k = \frac{m}{n}\ln(2)
    this->number_of_hash_functions = std::max(
    std::round((double((m))/double((n))) * std::log(2.0)), 
    1.0
    );
}

template<class Key, class Hash>
void bloom_filter<Key, Hash>::insert(const Key &key)
{
    const auto hashes = this->hash_function(key);

    // Use double-hashing to emulate k-hashes
    // for each hash, marks the corresponding position as 1/true
    for (std::size_t i = 0; i < this->number_of_hash_functions; ++i) {
        const auto pos = (hashes.first + i * hashes.second) 
        % this->number_of_bits;

        this->bit_vector[pos] = true;
    }
}

template<class Key, class Hash>
bool bloom_filter<Key, Hash>::probably_contains(const Key &key) const
{
    const auto hashes = this->hash_function(key);

    // Use double-hashing to emulate k-hashes
    // Verifies if every bit is was setted true
    for (std::size_t i = 0; i < this->number_of_hash_functions; ++i) {
        const auto pos = (hashes.first + i * hashes.second) 
        % this->number_of_bits;

        if (!this->bit_vector[pos]) {
            return false;
        }
    }

    return true;
}

template<class Key, class Hash>
std::size_t bloom_filter<Key, Hash>::bit_count() const {
    return this->number_of_bits;
}

template<class Key, class Hash>
std::size_t bloom_filter<Key, Hash>::hash_count() const {
    return this->number_of_hash_functions;
}
