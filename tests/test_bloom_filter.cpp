#include <cstdint>
#include <iostream>
#include "filters/bloom_filter.hpp"
#include "hashing/murmurhash3_adapter.hpp"

int main() 
{
    const std::uint64_t n = 100000;
    const double target_false_positive_rate = 0.01;
    bloom_filter<std::uint64_t, murmurhash3> filter(n, target_false_positive_rate);
    const std::uint64_t limit = 1000000;

    for (std::uint64_t key = 0; key < n; ++key) {
        filter.insert(key);
    }

    bool false_negatives = false;

    for (std::uint64_t key = 0; key < n; ++key) {
        if (!filter.probably_contains(key)) {
            false_negatives = true;
            std::cout << "False negative found." << "\n";
        }
    }

    std::uint64_t n_false_positives = 0;

    for (std::uint64_t key = n; key < n + limit; ++key) {
        if (filter.probably_contains(key)) {
            ++n_false_positives;
        }
    }

    const double true_false_positive_rate = static_cast<double>(n_false_positives) / limit;

    std::cout << "Inserted keys: " << n << '\n';
    std::cout << "Number of bits on the bit vector: " << filter.bit_count() << "\n";
    std::cout << "Number of hash functions: " << filter.hash_count() << "\n";
    std::cout << "Number of false negatives: " << false_negatives << '\n';
    std::cout << "Absent keys tested: " << limit << '\n';
    std::cout << "Number of false positives: " << n_false_positives << '\n';
    std::cout << "Target false positive rate: " << 100.0 * target_false_positive_rate << "%" << "\n";
    std::cout << "True false positive rate: " << 100.0 * true_false_positive_rate << "%" << "\n";

    if (false_negatives) {
        return 1;
    }

    return 0;
}
