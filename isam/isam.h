#ifndef ISAM_H
#define ISAM_H

#include "../file_organization.h"
#include "index_node.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

template <typename TypeKey, typename RecordType>
class IsamSparse : public FileOrganization<TypeKey, RecordType> {
public:
  IsamSparse(std::string file, std::string index)
      : dataFile{file}, indexFile{index} {}
  IsamSparse(std::string file);
  ~IsamSparse();

  void insert(RecordType record) override;
  bool remove(TypeKey key) override;
  RecordType search(TypeKey key) override;
  std::vector<RecordType> searchInRange(TypeKey initialKey,
                                        TypeKey endKey) override;
  void writeIndex() override;
  void readIndex() override;

  RecordType search(long pagePos, TypeKey key);
  void writeData(DataPage<TypeKey, RecordType> page);
  void writeData(DataPage<TypeKey, RecordType> page, long physicalPos);
  std::vector<RecordType> scanIndexEntry(long pagePos);
  bool remove(long pagePos, TypeKey key);

private:
  std::string dataFile;
  std::string indexFile;
  IndexPage<TypeKey, RecordType> indexPage;
};

template <typename TypeKey, typename RecordType>
std::vector<RecordType>
IsamSparse<TypeKey, RecordType>::scanIndexEntry(long pagePos) {
  std::vector<RecordType> scanResult;
  auto dataPage = DataPage<TypeKey, RecordType>(dataFile, pagePos);

  for (auto &record : dataPage.records) {
    scanResult.push_back(record);
  }
  if (dataPage.next != -1) {
    auto tempResults = scanIndexEntry(dataPage.next);
    for (auto &record : tempResults)
      scanResult.push_back(record);
  }

  return scanResult;
}

template <typename TypeKey, typename RecordType>
IsamSparse<TypeKey, RecordType>::IsamSparse(std::string file) : dataFile{file} {
  dataFile = "indices/isam/" + dataFile;
  std::fstream(indexFile, std::ios::binary);
}

template <typename TypeKey, typename RecordType>
IsamSparse<TypeKey, RecordType>::~IsamSparse() {
  writeIndex();
}

template <typename TypeKey, typename RecordType>
void IsamSparse<TypeKey, RecordType>::writeIndex() {}

template <typename TypeKey, typename RecordType>
void IsamSparse<TypeKey, RecordType>::writeData(
    DataPage<TypeKey, RecordType> page) {
  std::ofstream file(dataFile, std::ios::app | std::ios::binary);
  file.write((char *)page.next, sizeof(page.next));
  file.write((char *)page.size, sizeof(page.size));
  file.write((char *)page.key, sizeof(page.key));
  for (auto &record : page.records) {
    file.write((char *)record, sizeof(record));
  }
}

template <typename TypeKey, typename RecordType>
void IsamSparse<TypeKey, RecordType>::writeData(
    DataPage<TypeKey, RecordType> page, long pos) {
  std::ofstream file(dataFile, std::ios::binary);
  file.seekp(pos);
  for (auto &record : page.records) {
    file.write((char *)record, sizeof(record));
  }
}

template <typename TypeKey, typename RecordType>
void IsamSparse<TypeKey, RecordType>::insert(RecordType record) {
  auto pagePos = indexPage.findPage(record.getKey());
  auto dataPage = DataPage<TypeKey, RecordType>(dataFile, pagePos);

  if (dataPage.size == dataPage.getMaxSize()) {
    if (indexPage.size == indexPage.getMaxSize()) {
      // overflow
      DataPage<TypeKey, RecordType> ovfPage;
      ovfPage.next = pagePos;
      ovfPage.key = dataPage.key;
      ovfPage.insert(record);

      std::ofstream file(dataFile, std::ios::app | std::ios::binary);
      long pagePos2 = file.tellp();
      writeData(ovfPage, pagePos2);

      auto indexEntry = indexPage.findEntry(record.getKey());
      indexPage[indexEntry].second = pagePos2;
    } else {
      // break page
      auto dividedPages = dataPage.breakPage();
      writeData(dividedPages.first, pagePos);

      std::ofstream file(dataFile, std::ios::app | std::ios::binary);
      long pagePos2 = file.tellp();

      writeData(dividedPages.second, pagePos2);
      indexPage.insert(dividedPages.second.key, pagePos2);
    }
  } else {
    // just insert
    dataPage.insert(record);
    writeData(dataPage, pagePos);
  }
}

template <typename TypeKey, typename RecordType>
RecordType IsamSparse<TypeKey, RecordType>::search(TypeKey key) {
  auto pagePos = indexPage.findPage(key);
  auto dataPage = DataPage<TypeKey, RecordType>(dataFile, pagePos);

  for (auto &record : dataPage.records) {
    if (record.getKey() == key)
      return record;
  }
  if (dataPage.next != -1)
    return search(dataPage.next, key);
  throw std::invalid_argument("Key value is not found");
}

template <typename TypeKey, typename RecordType>
RecordType IsamSparse<TypeKey, RecordType>::search(long pagePos, TypeKey key) {
  auto dataPage = DataPage<TypeKey, RecordType>(dataFile, pagePos);

  for (auto &record : dataPage.records) {
    if (record.getKey() == key)
      return record;
  }
  if (dataPage.next != -1)
    return search(pagePos, key);
  throw std::invalid_argument("Key value is not found");
}

template <typename TypeKey, typename RecordType>
std::vector<RecordType>
IsamSparse<TypeKey, RecordType>::searchInRange(TypeKey initialKey,
                                               TypeKey endKey) {
  std::vector<RecordType> searchResult;
  auto initialEntryPos = indexPage.findEntry(initialKey);
  auto endEntryPos = indexPage.findEntry(endKey);

  for (int i = initialEntryPos; i < endEntryPos; ++i) {
    auto pagePos = indexPage.entries[i].second;
    auto tempResults = scanIndexEntry(pagePos);

    for (auto &record : tempResults)
      searchResult.push_back(record);
  }

  return searchResult;
}

template <typename TypeKey, typename RecordType>
bool IsamSparse<TypeKey, RecordType>::remove(TypeKey key) {
  auto pagePos = indexPage.findPage(key);
  return remove(pagePos, key);
}

template <typename TypeKey, typename RecordType>
bool IsamSparse<TypeKey, RecordType>::remove(long pagePos, TypeKey key) {
  auto dataPage = DataPage<TypeKey, RecordType>(dataFile, pagePos);

  for (int i = 0; i < dataPage.size; ++i) {
    if (dataPage.records[i].getKey() == key) {
      dataPage.records.erase(i);
      writeData(dataPage, pagePos);
      return true;
    }
  }

  if (dataPage.next != -1) {
    return remove(dataPage.next, key);
  } else {
    throw std::invalid_argument("Key value is not found");
  }
}

#endif
