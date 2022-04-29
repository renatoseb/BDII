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
  void writeIndex() override;
  void readIndex() override;

  void compressFile();

private:
  std::string dataFile;
  std::string indexFile;
  IndexPage<TypeKey, RecordType> indexPage;
};

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
void IsamSparse<TypeKey, RecordType>::insert(RecordType record) {
  DataPage<TypeKey, RecordType> dataPage;
  auto dataPos = indexPage.findPage(record.getKey());
  dataPage.readPage(dataFile, dataPos);

  if (dataPage.size == dataPage.getMaxSize()) {
    if (indexPage.size == indexPage.getMaxSize()) {
      dataPage.insert(record);
    } else {
      auto dividedPages = dataPage.breakPage();
      writePage(dividedPages.first, dataPos);

      std::ofstream file(dataFile, std::ios::app | std::ios::binary);
      long dataPos2 = file.tellp();

      writePage(dividedPages.second);
      indexPage.insert(dividedPages.second.key, dataPos2);
    }
  }
}

#endif
