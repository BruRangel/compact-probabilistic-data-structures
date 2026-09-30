#include <algorithm>
#include <cmath>
#include "bloom_filter.hpp"

template<class Key, class Hasher>
bloom_filter<Key, Hasher>::bloom_filter(
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

template<class Key, class Hasher>
void bloom_filter<Key, Hasher>::insert(const Key &key)
{
    const auto hashes = this->hasher(key);

    // Use double-hashing to emulate k-hashes
    // for each hash, marks the corresponding position as 1/true
    for (std::size_t i = 0; i < this->number_of_hash_functions; ++i) {
        const auto pos = (hashes.first + i * hashes.second) 
        % this->number_of_bits;

        this->bit_vector[pos] = true;
    }
}

template<class Key, class Hasher>
void bloom_filter<Key, Hasher>::probably_contains(const Key &key) const
{
    const auto hashes = this->hasher(key);

    // Use double-hashing to emulate k-hashes
    // for each hash, marks the corresponding position as 1/true
    for (std::size_t i = 0; i < this->number_of_hash_functions; ++i) {
        const auto pos = (hashes.first + i * hashes.second) 
        % this->number_of_bits;

        this->bit_vector[pos] = true;
    }
}