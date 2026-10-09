#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>

// Using the same notation of the paper of Fan et al., 2014.
// The user needs to inform:
// n == the number of elements expected to be inserted on the filter
// p == the target false positive rate
// b == the size of each bucket
// max_realocations == the maximum number of realocations allowed

// we will calculate

// the fingerprint size f = ceil(log_2(1/p) + log_2(2b))
// the number of buckets m = ceil(m / (b * a))
// a == load factor

// This template lets the user choose the key type and 
// the hash function to be used on the filter.
template<class Key, class Hash>
class cuckoo_filter {
private:
    using fingerprint_type = std::uint16_t;
    static constexpr fingerprint_type EMPTY = 0;
    static constexpr std::size_t MAX_FINGERPRINT_BITS = 16;

    std::vector<fingerprint_type> table;
    std::size_t number_of_buckets;
    std::size_t bucket_size;
    std::size_t fingerprint_bits;
    fingerprint_type fingerprint_mask;
    std::size_t max_num_kicks;
    std::size_t number_of_items;

    Hash hash_function;

    // table operations
    fingerprint_type get_entry(std::size_t bucket, std::size_t slot) const;
    void set_entry(std::size_t bucket, std::size_t slot, fingerprint_type fp);
    void swap_entry(std::size_t bucket, std::size_t slot, fingerprint_type &fp);

    // partial-key cuckoo hashing
    

public:
    cuckoo_filter (
        std::size_t number_of_expected_elements,
        double target_false_positive_rate,
        std::size_t bucket_size = 4,
        std::size_t max_realocations = 500
    );

    // main methods
    bool insert(const Key &key);
    bool probably_contains(const Key &key) const;
    bool erase(const Key &key);

    std::size_t bucket_count() const;
    std::size_t get_bucket_size() const;
    std::size_t get_fingerprint_size() const;
    std::size_t item_count() const;
    double load_factor() const;
};
