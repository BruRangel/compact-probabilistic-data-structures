#include <cstdint>
#include <iostream>
#include "filters/cuckoo_filter.hpp"
#include "hashing/murmurhash3_adapter.hpp"

int main()
{
    const std::uint64_t n = 100000;
    const double target_false_positive_rate = 0.01;
    cuckoo_filter<std::uint64_t, murmurhash3> filter(n, target_false_positive_rate);
    const std::uint64_t limit = 1000000;

    std::uint64_t failed_insertions = 0;

    for (std::uint64_t key = 0; key < n; ++key) {
        if (!filter.insert(key)) {
            ++failed_insertions;
        }
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

    std::cout << "Inserted keys: " << filter.item_count() << '\n';
    std::cout << "Failed insertions: " << failed_insertions << '\n';
    std::cout << "Number of buckets: " << filter.bucket_count() << '\n';
    std::cout << "Bucket size: " << filter.get_bucket_size() << '\n';
    std::cout << "Fingerprint size (bits): " << filter.get_fingerprint_size() << '\n';
    std::cout << "Total bits: " << filter.bit_count() << '\n';
    std::cout << "Load factor: " << 100.0 * filter.load_factor() << "%" << '\n';
    std::cout << "Number of false negatives: " << false_negatives << '\n';
    std::cout << "Absent keys tested: " << limit << '\n';
    std::cout << "Number of false positives: " << n_false_positives << '\n';
    std::cout << "Target false positive rate: " << 100.0 * target_false_positive_rate << "%" << '\n';
    std::cout << "True false positive rate: " << 100.0 * true_false_positive_rate << "%" << '\n';

    const std::uint64_t half = n / 2;
    std::uint64_t failed_erasures = 0;

    for (std::uint64_t key = 0; key < half; ++key) {
        if (!filter.erase(key)) {
            ++failed_erasures;
        }
    }

    for (std::uint64_t key = half; key < n; ++key) {
        if (!filter.probably_contains(key)) {
            false_negatives = true;
            std::cout << "False negative found after erase." << "\n";
        }
    }

    std::cout << "Erased keys: " << half - failed_erasures << '\n';
    std::cout << "Remaining keys: " << filter.item_count() << '\n';

    if (failed_insertions > 0 || failed_erasures > 0 || false_negatives) {
        return 1;
    }

    return 0;
}
