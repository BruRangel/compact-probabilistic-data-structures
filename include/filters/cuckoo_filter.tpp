#pragma once
#include <algorithm>
#include <cmath>
#include <utility>
#include "cuckoo_filter.hpp"

template<class Key, class Hash>
cuckoo_filter<Key, Hash>::cuckoo_filter(
    std::size_t n,
    double p,
    std::size_t b,
    std::size_t max_relocations
) : bucket_size(b),
    max_num_kicks(max_relocations),
    number_of_items(0),
    hash_function(),
    rng(std::random_device{}())
{
    // f = ceil(log_2(1/p) + log_2(2b))
    const std::size_t raw_f = std::ceil(std::log2(1.0 / p) + std::log2(2.0 * b));
    this->fingerprint_bits = std::clamp<std::size_t>(raw_f, 2, MAX_FINGERPRINT_BITS);
    this->fingerprint_mask = (this->fingerprint_bits == MAX_FINGERPRINT_BITS)
        ? ~fingerprint_type{0}
        : ((fingerprint_type{1} << this->fingerprint_bits) - 1);

    // Load factor alpha from Table 2 of Fan et al., 2014
    const double alpha = (b == 1) ? 0.45 : (b == 2) ? 0.84 : (b < 8) ? 0.95 : 0.98;
    const std::size_t min_buckets = std::ceil(n / (b * alpha));

    // m must be a power of 2 so that (i2 ^ hash(f)) % m == i1
    this->number_of_buckets = 1;
    while (this->number_of_buckets < min_buckets) {
        this->number_of_buckets <<= 1;
    }
    this->bucket_mask = this->number_of_buckets - 1;

    const std::size_t total_bits = this->number_of_buckets * this->bucket_size * this->fingerprint_bits;
    this->table.assign((total_bits + 63) / 64, 0ULL);
}

template<class Key, class Hash>
typename cuckoo_filter<Key, Hash>::fingerprint_type
cuckoo_filter<Key, Hash>::get_entry(std::size_t bucket, std::size_t slot) const
{
    const std::size_t bit_pos = (bucket * this->bucket_size + slot) * this->fingerprint_bits;
    const std::size_t word = bit_pos / 64;
    const std::size_t offset = bit_pos % 64;

    std::uint64_t val = this->table[word] >> offset;
    if (offset + this->fingerprint_bits > 64) {
        val |= this->table[word + 1] << (64 - offset);
    }
    return static_cast<fingerprint_type>(val & this->fingerprint_mask);
}

template<class Key, class Hash>
void cuckoo_filter<Key, Hash>::set_entry(std::size_t bucket, std::size_t slot, fingerprint_type fp)
{
    const std::size_t bit_pos = (bucket * this->bucket_size + slot) * this->fingerprint_bits;
    const std::size_t word = bit_pos / 64;
    const std::size_t offset = bit_pos % 64;
    const std::uint64_t mask = this->fingerprint_mask;
    const std::uint64_t val = fp & this->fingerprint_mask;

    this->table[word] = (this->table[word] & ~(mask << offset)) | (val << offset);
    if (offset + this->fingerprint_bits > 64) {
        const std::size_t shift = 64 - offset;
        this->table[word + 1] = (this->table[word + 1] & ~(mask >> shift)) | (val >> shift);
    }
}

template<class Key, class Hash>
void cuckoo_filter<Key, Hash>::swap_entry(std::size_t bucket, std::size_t slot, fingerprint_type &fp)
{
    const fingerprint_type old = this->get_entry(bucket, slot);
    this->set_entry(bucket, slot, fp);
    fp = old;
}

template<class Key, class Hash>
bool cuckoo_filter<Key, Hash>::insert_into_bucket(std::size_t bucket, fingerprint_type fp)
{
    for (std::size_t slot = 0; slot < this->bucket_size; ++slot) {
        if (this->get_entry(bucket, slot) == EMPTY) {
            this->set_entry(bucket, slot, fp);
            return true;
        }
    }
    return false;
}

template<class Key, class Hash>
typename cuckoo_filter<Key, Hash>::fingerprint_type
cuckoo_filter<Key, Hash>::compute_fingerprint(std::uint64_t hash_value) const
{
    // Avoid 0 (reserved for EMPTY): map uniformly to [1, 2^f - 1]
    return static_cast<fingerprint_type>((hash_value % this->fingerprint_mask) + 1);
}

template<class Key, class Hash>
std::size_t cuckoo_filter<Key, Hash>::alternate_index(std::size_t index, fingerprint_type fp) const
{
    std::size_t offset = this->hash_function(fp).first & this->bucket_mask;
    if (this->number_of_buckets > 1 && offset == 0) {
        offset = 1; // avoid i2 == i1 while preserving XOR involution
    }
    return index ^ offset;
}

template<class Key, class Hash>
bool cuckoo_filter<Key, Hash>::insert(const Key &key)
{
    const auto hashes = this->hash_function(key);
    const std::size_t i1 = hashes.first & this->bucket_mask;
    fingerprint_type fp = this->compute_fingerprint(hashes.second);
    const std::size_t i2 = this->alternate_index(i1, fp);

    // Algorithm 1 (Fan et al., 2014)
    if (this->insert_into_bucket(i1, fp) || this->insert_into_bucket(i2, fp)) {
        ++this->number_of_items;
        return true;
    }

    std::size_t i = (this->rng() & 1ULL) ? i2 : i1;
    std::vector<std::pair<std::size_t, std::size_t>> kicks;
    kicks.reserve(this->max_num_kicks);

    for (std::size_t n = 0; n < this->max_num_kicks; ++n) {
        const std::size_t slot = this->rng() % this->bucket_size;
        this->swap_entry(i, slot, fp);
        kicks.emplace_back(i, slot);

        i = this->alternate_index(i, fp);
        if (this->insert_into_bucket(i, fp)) {
            ++this->number_of_items;
            return true;
        }
    }

    // Relocation failed: undo swaps in reverse order so no previous item is lost
    for (auto it = kicks.rbegin(); it != kicks.rend(); ++it) {
        this->swap_entry(it->first, it->second, fp);
    }
    return false;
}

template<class Key, class Hash>
bool cuckoo_filter<Key, Hash>::probably_contains(const Key &key) const
{
    const auto hashes = this->hash_function(key);
    const std::size_t i1 = hashes.first & this->bucket_mask;
    const fingerprint_type fp = this->compute_fingerprint(hashes.second);
    const std::size_t i2 = this->alternate_index(i1, fp);

    // Algorithm 2 (Fan et al., 2014)
    for (std::size_t bucket : {i1, i2}) {
        for (std::size_t slot = 0; slot < this->bucket_size; ++slot) {
            if (this->get_entry(bucket, slot) == fp) {
                return true;
            }
        }
    }
    return false;
}

template<class Key, class Hash>
bool cuckoo_filter<Key, Hash>::erase(const Key &key)
{
    const auto hashes = this->hash_function(key);
    const std::size_t i1 = hashes.first & this->bucket_mask;
    const fingerprint_type fp = this->compute_fingerprint(hashes.second);
    const std::size_t i2 = this->alternate_index(i1, fp);

    // Algorithm 3 (Fan et al., 2014)
    for (std::size_t bucket : {i1, i2}) {
        for (std::size_t slot = 0; slot < this->bucket_size; ++slot) {
            if (this->get_entry(bucket, slot) == fp) {
                this->set_entry(bucket, slot, EMPTY);
                --this->number_of_items;
                return true;
            }
        }
    }
    return false;
}
