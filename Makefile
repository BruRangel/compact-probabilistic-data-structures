CXX = g++
CPPFLAGS = -Iinclude
CXXFLAGS = -std=c++17 -Wall -Wextra -g
BENCHFLAGS = -std=c++17 -O2 -Wall -Wextra

HASH_SOURCES = src/hashing/murmurhash3_adapter.cpp \
               src/hashing/murmurhash3.cpp

HASH_HEADERS = include/hashing/murmurhash3_adapter.hpp \
               include/hashing/murmurhash3.hpp

BLOOM_TARGET = build/test_bloom_filter
CUCKOO_TARGET = build/test_cuckoo_filter
BENCH_TARGET = build/bench_filters
TARGETS = $(BLOOM_TARGET) $(CUCKOO_TARGET) $(BENCH_TARGET)

.PHONY: all test test_bloom test_cuckoo bench clean

all: $(TARGETS)

$(BLOOM_TARGET): tests/test_bloom_filter.cpp $(HASH_SOURCES) $(HASH_HEADERS) \
                 include/filters/bloom_filter.hpp include/filters/bloom_filter.tpp Makefile
	mkdir -p build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) tests/test_bloom_filter.cpp $(HASH_SOURCES) -o $(BLOOM_TARGET)

$(CUCKOO_TARGET): tests/test_cuckoo_filter.cpp $(HASH_SOURCES) $(HASH_HEADERS) \
                  include/filters/cuckoo_filter.hpp include/filters/cuckoo_filter.tpp Makefile
	mkdir -p build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) tests/test_cuckoo_filter.cpp $(HASH_SOURCES) -o $(CUCKOO_TARGET)

$(BENCH_TARGET): benchmarks/bench_filters.cpp $(HASH_SOURCES) $(HASH_HEADERS) \
                 include/filters/bloom_filter.hpp include/filters/bloom_filter.tpp \
                 include/filters/cuckoo_filter.hpp include/filters/cuckoo_filter.tpp Makefile
	mkdir -p build
	$(CXX) $(CPPFLAGS) $(BENCHFLAGS) benchmarks/bench_filters.cpp $(HASH_SOURCES) -o $(BENCH_TARGET)

test_bloom: $(BLOOM_TARGET)
	./$(BLOOM_TARGET)

test_cuckoo: $(CUCKOO_TARGET)
	./$(CUCKOO_TARGET)

test: test_bloom test_cuckoo

bench: $(BENCH_TARGET)
	python3 benchmarks/compare_filters.py

clean:
	rm -f $(TARGETS)
