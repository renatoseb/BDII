#ifndef BPTREE_H
#define BPTREE_H

#include "../file_organization.h"
#include "node.h"
#include <string>
#include <fstream>
#include <vector>

using namespace std;

const int R = 3, BUCKET_SIZE = 20, BUFFER_SIZE = 4096; // I used random values

template<typename TypeKey, typename RecordType>
class BPTree: public FileOrganization
{
private:
    NodeB<TypeKey> *root;
    int m;

    RecordType searchRec(NodeB<TypeKey> *node, char *key);

public:
    BPTree(std::string file, std::string index)
      : fileName{file}, indexName{index} 
      {
          // code
          this->root = readNode(0, this->dataFile);
      }
    BPTree(std::string file);
    ~BPTree();

    // Operations
    void insert(RecordType record) override;                                        // TODO
    bool remove(char *key) override;                                                // TODO
    RecordType search(TypeKey key) override;
    vector<RecordType> searchInRange(TypeKey initialKey, TypeKey endKey) override; 

    // Utils
    Bucket<RecordType> readBucket(string file, long address); 
    NodeB<TypeKey> readNode(string file, long address);         // TODO
    
};




#endif