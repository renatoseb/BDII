#ifndef FILEORGANIZATION_H
#define FILEORGANIZATION_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>
#include <vector>

constexpr std::size_t BUFFER_SIZE = 4096; // min for x86

template <typename TypeKey, typename RecordType> struct DataPage {
  long next = -1; // 8
  int size = 0;   // 4
  TypeKey key;
  std::vector<RecordType> records;

  DataPage(int s, TypeKey k) : size{s}, key{k} {}

  int getMaxSize() {
    return (4096 - 12 - sizeof(TypeKey)) / sizeof(RecordType);
  }
  void insert(RecordType record) {
    records.push_back(record);
    // ordenar records
  }
  void readPage(std::string fileName, long pos) {
    std::ifstream data(fileName, std::ios::binary);
    RecordType record;
    int bufferSpace = BUFFER_SIZE;
    auto recordSize = sizeof(record);

    while (data.read((char *)&record, recordSize) &&
           bufferSpace >= recordSize) {
      insert(record);
      bufferSpace = bufferSpace - recordSize;
    }
  }
  std::pair<DataPage, DataPage> breakPage() {
    auto mid = size / 2;
    auto page1 = DataPage(mid, records[0].getKey());
    auto page2 = DataPage(size - mid, records[mid].getKey());

    for (int i = 0; i < mid; ++i) {
      page1.insert(records[i]);
    }

    for (int i = mid; i < size; ++i) {
      page2.insert(records[i]);
    }

    return std::make_pair<page1, page2>;
  }
};

template <typename TypeKey, typename RecordType> class FileOrganization {
private:
  string dataFile;
  string indexFile;
public:
  virtual void readIndex() = 0;
  virtual void writeIndex() = 0;
  virtual void insert(RecordType record) = 0;
  virtual RecordType search(TypeKey key) = 0;
  virtual std::vector<RecordType> searchInRange(TypeKey initialKey,
                                                TypeKey endKey) = 0;
  virtual bool remove(TypeKey key) = 0;
};

#endif
