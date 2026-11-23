#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "filters/bloom_filter.hpp"
#include "filters/cuckoo_filter.hpp"
#include "hashing/murmurhash3_adapter.hpp"

using clock_type = std::chrono::steady_clock;

template<class Filter>
void run_benchmark(const std::string &name, std::uint64_t n, double target_fpr, std::uint64_t queries)
{
    Filter filter(n, target_fpr);

    auto t0 = clock_type::now();
    for (std::uint64_t key = 0; key < n; ++key) {
        filter.insert(key);
    }
    auto t1 = clock_type::now();

    const std::uint64_t hit_queries = (n < queries) ? n : queries;
    std::uint64_t hits = 0;
    for (std::uint64_t key = 0; key < hit_queries; ++key) {
        if (filter.probably_contains(key)) {
            ++hits;
        }
    }
    auto t2 = clock_type::now();

    std::uint64_t false_positives = 0;
    for (std::uint64_t key = n; key < n + queries; ++key) {
        if (filter.probably_contains(key)) {
            ++false_positives;
        }
    }
    auto t3 = clock_type::now();

    if (hits != hit_queries) {
        std::cerr << "Warning: " << name << " had " << (hit_queries - hits) << " false negatives at p=" << target_fpr << "\n";
    }

    const double insert_ns = std::chrono::duration<double, std::nano>(t1 - t0).count() / n;
    const double hit_ns = std::chrono::duration<double, std::nano>(t2 - t1).count() / hit_queries;
    const double miss_ns = std::chrono::duration<double, std::nano>(t3 - t2).count() / queries;
    const double empirical_fpr = static_cast<double>(false_positives) / queries;
    const double bits_per_item = static_cast<double>(filter.bit_count()) / n;

    std::cout << name << ','
              << n << ','
              << target_fpr << ','
              << empirical_fpr << ','
              << bits_per_item << ','
              << insert_ns << ','
              << hit_ns << ','
              << miss_ns << '\n';
}

int main(int argc, char *argv[])
{
    // Default n = 120000 fills 32768 buckets (b = 4) to ~91.6% load factor
    const std::uint64_t n = (argc > 1) ? std::stoull(argv[1]) : 120000;
    const std::uint64_t queries = (argc > 2) ? std::stoull(argv[2]) : 500000;

    const std::vector<double> fprs = {0.1, 0.05, 0.01, 0.005, 0.001, 0.0005, 0.0001};

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "filter,n,target_fpr,empirical_fpr,bits_per_item,insert_ns,lookup_hit_ns,lookup_miss_ns\n";

    for (double p : fprs) {
        run_benchmark<bloom_filter<std::uint64_t, murmurhash3>>("bloom", n, p, queries);
        run_benchmark<cuckoo_filter<std::uint64_t, murmurhash3>>("cuckoo", n, p, queries);
    }

    return 0;
}
