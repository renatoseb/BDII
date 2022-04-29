#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>
#include <vector>

constexpr std::size_t BUFFER_SIZE = 4096; // min for x86

template <typename TypeKey, typename RecordType> struct IndexPage {
  int size = 0;
  TypeKey key;
  std::vector<std::pair<TypeKey, long>> entries;
  int getMaxSize() {
    return (BUFFER_SIZE - 8 - sizeof(TypeKey)) / sizeof(RecordType);
  }
  void insert(TypeKey key, long value) {
    entries.push_back(std::make_pair<key, value>);
    size++;
  }

  void readPage(std::string fileName) {
    std::ifstream index(fileName, std::ios::binary);
    TypeKey key;
    long value;
    int bufferSpace = BUFFER_SIZE;
    auto indexRecordSize = sizeof(std::make_pair<TypeKey, long>);

    while (index.read((char *)&key, sizeof(key)) &&
           bufferSpace >= indexRecordSize) {
      index.read((char *)&value, sizeof(value));
      insert(key, value);
      bufferSpace = bufferSpace - indexRecordSize;
    }
  }

  long findPage(TypeKey key) {
    int l = 0;
    int r = size;
    return entries[findPage(l, r, key)].second;
  }

  int findPage(int l, int r, TypeKey key) {
    int mid = l + (r - l) / 2;
    if (r >= l) {
      if (entries[mid].first == key)
        // shouldn't get the same key
        return mid;
      if (entries[mid].first > key)
        return findPage(l, mid - 1, key);
      if (entries[mid].first < key)
        return findPage(mid + 1, r, key);
    }
    return entries[mid].first;
  }
};
