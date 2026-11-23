#pragma once
#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

// Cuckoo Filter based on Fan et al., 2014 ("Cuckoo Filter: Practically Better Than Bloom").
// User parameters:
//   n == expected number of elements
//   p == target false positive rate
//   b == bucket size (default 4)
//   max_relocations == MaxNumKicks before declaring the filter full (default 500)
//
// Derived parameters:
//   f = ceil(log_2(1/p) + log_2(2b))
//   m = next_power_of_two(ceil(n / (b * a)))  (power of 2 for XOR partial-key hashing)

template<class Key, class Hash>
class cuckoo_filter {
private:
    using fingerprint_type = std::uint32_t;
    static constexpr fingerprint_type EMPTY = 0;
    static constexpr std::size_t MAX_FINGERPRINT_BITS = 32;

    std::vector<std::uint64_t> table; // bit-packed storage (f bits per slot)
    std::size_t number_of_buckets;
    std::size_t bucket_mask;
    std::size_t bucket_size;
    std::size_t fingerprint_bits;
    fingerprint_type fingerprint_mask;
    std::size_t max_num_kicks;
    std::size_t number_of_items;

    Hash hash_function;
    std::mt19937_64 rng;

    // Bit-packed table operations
    fingerprint_type get_entry(std::size_t bucket, std::size_t slot) const;
    void set_entry(std::size_t bucket, std::size_t slot, fingerprint_type fp);
    void swap_entry(std::size_t bucket, std::size_t slot, fingerprint_type &fp);
    bool insert_into_bucket(std::size_t bucket, fingerprint_type fp);

    // Partial-key cuckoo hashing
    fingerprint_type compute_fingerprint(std::uint64_t hash_value) const;
    std::size_t alternate_index(std::size_t index, fingerprint_type fp) const;

public:
    cuckoo_filter(
        std::size_t number_of_expected_elements,
        double target_false_positive_rate,
        std::size_t bucket_size = 4,
        std::size_t max_relocations = 500
    );

    bool insert(const Key &key);
    bool probably_contains(const Key &key) const;
    bool erase(const Key &key);

    std::size_t bucket_count() const { return number_of_buckets; }
    std::size_t get_bucket_size() const { return bucket_size; }
    std::size_t get_fingerprint_size() const { return fingerprint_bits; }
    std::size_t item_count() const { return number_of_items; }
    std::size_t capacity() const { return number_of_buckets * bucket_size; }
    std::size_t bit_count() const { return capacity() * fingerprint_bits; }
    double load_factor() const { return static_cast<double>(number_of_items) / capacity(); }
};

#include "cuckoo_filter.tpp"
