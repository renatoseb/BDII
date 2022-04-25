#ifndef FILEORGANIZATION_H
#define FILEORGANIZATION_H

#include <iostream>
#include <map>
#include <string>
#include <unistd.h>
#include <vector>

constexpr std::size_t BUFFER_SIZE = 4096; // min for x86

struct Page {
  long next = -1; // 8
  int size = 0;   // 4
  char *key;
  std::map<int, long> record;
};

template <typename RecordType> class FileOrganization {
public:
  virtual void readIndex() = 0;
  virtual void writeIndex() = 0;
  virtual void insert(RecordType record) = 0;
  virtual RecordType search(char *key) = 0;
  virtual std::vector<RecordType> searchInRange(char *initialKey,
                                                char *endKey) = 0;
  virtual bool remove(char *key) = 0;
};

#endif
