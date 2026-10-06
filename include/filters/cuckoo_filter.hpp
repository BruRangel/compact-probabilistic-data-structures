#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>

// The user needs to inform:
// m == the number of elements expected to be inserted on the filter
// p == the target false positive rate
// b == the size of each bucket
// max_realocations == the maximum number of realocations allowed

// the fingerprint size f = ceil(log_2(1/p) + log_2(2b))
// the number of buckets n = ceil(m / (b * a))

// This template lets the user choose the key type and 
// the hash function to be used on the filter.
template<class Key, class Hash>
class cuckoo_filter {
private:
    using fingerprint_type = std::uint16_t;
    static constexpr fingerprint_type EMPTY = 0;

    std::vector<fingerprint_type> table; // m * b entries
    std::size_t number_of_buckets;
    std::size_t bucket_size;
    std::size_t fingerprint_mask;
    std::size_t max_realocations;
    Hash hash_function;

public:
    cuckoo_filter (
        std::size_t number_of_expected_elements,
        double target_false_positive_rate,
        std::size_t bucket_size = 4,
        std::size_t max_realocations = 500
    );

    bool insert(const Key &key);
    bool probably_contains(const Key &key) const;
    bool erase(const Key &key);

    std::size_t bucket_count() const;
    std::size_t bucket_size() const;
    std::size_t fingerprint_size() const;
};
