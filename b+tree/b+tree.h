#ifndef BPTREE_H
#define BPTREE_H

#include "../file_organization.h"
#include <string>
#include <fstream>
#include <vector>

using namespace std;

const int R = 3, BUCKET_SIZE = 20; // I used random values

template<typename TypeKey>
struct NodeB
{
    TypeKey keys[R];
    long childrens[R + 1];
    int size;
    bool isLeaf;
};

template<typename RecordType>
struct Bucket
{
    // Number of records of a bucket
    RecordType records[BUCKET_SIZE];
    // Current size of a bucket
    int size;
    // Address of next bucket
    long nextBucket;
};

template<typename TypeKey, typename RecordType>
class BPTree: public FileOrganization
{
private:
    NodeB<TypeKey> *root;
    int m;

    RecordType searchRec(NodeB<TypeKey> *node, char *key);

public:
    BPTree(std::string file, std::string index)
      : fileName{file}, indexName{index} {}
    BPTree(std::string file);
    ~BPTree();

    // Operations
    void insert(RecordType record) override;                                        // TODO
    bool remove(char *key) override;                                                // TODO
    RecordType search(char *key) override;
    vector<RecordType> rangesearch(TypeKey initialKey, TypeKey endKey) override; 

    // Utils
    Bucket<RecordType> readBucket(string file, long address); 
    NodeB<TypeKey> readNode(string file, long address);         // TODO
    
};




#endif