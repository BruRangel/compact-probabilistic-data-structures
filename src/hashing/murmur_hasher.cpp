#include "hashing/murmur_hasher.hpp"
#include "hashing/murmurhash3.hpp"

hash_pair murmur_hasher::operator()(std::uint64_t key) const {
    std::uint64_t output[2];

    MurmurHash3_x64_128(
        &key,
        sizeof(key),
        seed_,
        output
    );

    return {output[0], output[1]};
}
