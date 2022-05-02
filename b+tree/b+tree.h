#ifndef BPTREE_H
#define BPTREE_H

#include "../file_organization.h"
#include "node.h"
#include <string>
#include <fstream>
#include <vector>
#include "page.h"
using namespace std;

const int R = 3, BUCKET_SIZE = 20, BUFFER_SIZE = 4096; // I used random values

template<typename TypeKey, typename RecordType>
class BPTree: public FileOrganization
{
private:
    Page<RecordType> page;  

    RecordType searchRec(NodeB<TypeKey> *node, TypeKey key);
    bool removeRec(NodeB<TypeKey>& node, TypeKey key);

public:
    BPTree(Page<RecordType> page)
    {
        this->page = page;
    }
    ~BPTree();

    // Operations
    void insert(RecordType record) override;                                        // TODO
    bool remove(TypeKey key) override;                                                // TODO
    RecordType search(TypeKey key) override;
    vector<RecordType> searchInRange(TypeKey initialKey, TypeKey endKey) override; 

    // Utils
    // Bucket<RecordType> readBucket(string file, long address); 
    NodeB<TypeKey> readNode(string file, long address);         // TODO
    void writeNode(long pos, nodeB<TypeKey> node);
    TypeKey succesor(NodeB<TypeKey>& node); 
    bool borrowFromSibling(Bucket<RecordType> );
    void decreaseHeight(NodeB<TypeKey>& node, NodeB<TypeKey>& nodeUnderflow, int pos);
    void mergeLeaf(NodeB<TypeKey>& node, NodeB<TypeKey> nodeUnderflow, int pos);
    bool mergeWithParent(NodeB<TypeKey>& node, NodeB<TypeKey>& nodeUnderflow, int pos);
    bool borrowFromSibling(NodeB<TypeKey>& nodeUnderflow, NodeB<TypeKey>& node, int pos); 
};




#endif