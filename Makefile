CXX = g++
CPPFLAGS = -Iinclude
CXXFLAGS = -std=c++17 -Wall -Wextra -g

TARGET = build/test_bloom_filter

SOURCES = tests/test_bloom_filter.cpp \
          src/hashing/murmurhash3_adapter.cpp \
          src/hashing/murmurhash3.cpp

HEADERS = include/filters/bloom_filter.hpp \
          include/filters/bloom_filter.tpp \
          include/hashing/murmurhash3_adapter.hpp \
          include/hashing/murmurhash3.hpp

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS) Makefile
	mkdir -p build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)