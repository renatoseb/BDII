#ifndef NODE_H
#define NODE_H

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

#endif