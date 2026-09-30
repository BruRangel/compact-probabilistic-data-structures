#include <vector>
#include <cstddef>

// size of the bit vector == m = -\frac{n \ln(p)}{(\ln(2))^2}
// number of hash functions == k = \frac{m}{n}\ln(2)

template<class Key>
class bloom_filter {
private:
    std::vector<bool> bit_vector;
    std::size_t number_of_bits;
    std::size_t number_of_expected_elements;
    std::size_t number_of_hash_functions;
    double target_false_positive_rate;

public:
    bloom_filter(
        std::size_t number_of_expected_elements,
        double target_false_positive_rate       
    );
    ~bloom_filter() = default;
    void insert(const Key &key);
    bool possibly_contains(const Key &key) const;
};
