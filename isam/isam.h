#ifndef ISAM_H
#define ISAM_H

#include "../file_organization.h"

#include <filesystem>
#include <fstream>
#include <map>
#include <string>

template <typename TypeKey, typename RecordType>
class IsamSparse : public FileOrganization<RecordType> {
public:
  IsamSparse(std::string file, std::string index)
      : fileName{file}, indexName{index} {}
  IsamSparse(std::string file);
  ~IsamSparse();

  void insert(RecordType record) override;
  bool remove(TypeKey key) override;
  RecordType search(TypeKey key) override;
  void readIndex() override;
  void writeIndex() override;

  void compressFile();

private:
  std::string fileName;
  std::string indexName;
  std::map<int, int> index;
};

template<typename RecordType>
IsamSparse<RecordType>::~IsamSparse() {
  writeIndex();
}

template<typename RecordType>
IsamSparse<RecordType>IsamSparse(std::string file) : fileName{file} {
  fileName = "indices/isam/" + fileName;
  std::fstream(indexName, std::ios::binary);
}



#endif
