#pragma once

#include <cstdint>

#include "hashing/hash_pair.hpp"

class murmur_hasher {
private:
    std::uint32_t seed_;

public:
    explicit murmur_hasher(std::uint32_t seed = 0)
        : seed_(seed) {}

    hash_pair operator()(std::uint64_t key) const;
};
