#pragma once
#include <cstdint>
#include <utility>
#include <random>

// Adapts murmurhash3 to be usable on the bloom filter implementation
class murmurhash3 {
private:
    std::uint32_t seed;

public:
    // If not explicited, the constructor will choose a random seed
    explicit murmurhash3(std::uint32_t seed = std::random_device{}())
        : seed(seed) {}

    // Two values to make double hashing
    std::pair<std::uint64_t, std::uint64_t> operator()(std::uint64_t key) const;

    std::uint32_t get_seed() const {
        return seed;
    }
};
