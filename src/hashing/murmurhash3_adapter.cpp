#include "hashing/murmurhash3_adapter.hpp"
#include "hashing/murmurhash3.hpp"

std::pair<std::uint64_t, std::uint64_t> murmurhash3::operator()(std::uint64_t key) const {
    std::uint64_t output[2];

    MurmurHash3_x64_128(
        &key,
        sizeof(key),
        seed,
        output
    );

    // Returns to values, h1(key) and h2(key)
    // We will use this positions to calculate another hashes,
    // hi(key) = (h1(key) + i * h2(key)) % m
    return {output[0], output[1]};
}
