#include "b+tree.h"


template<typename TypeKey, typename RecordType>
Bucket<RecordType> BPTree<TypeKey, RecordType>::readBucket(string file, long address) 
{
        Bucket bucket;
        ifstream file_(file.c_str(), ios::in | ios::binary);
        file_.seekg(address, ios::beg);
        file_.read((char *)&bucket, sizeof(Bucket));
        file_.close();
        return bucket;
}

// TODO: Comentar función para que se entienda mejor
template<typename TypeKey, typename RecordType>
RecordType BPTree<TypeKey, RecordType>::searchRec(NodeB<TypeKey> *node, char *key)
{
        try
        {
            if (!node->isLeaf)
            {
                int pos = 0;
                while (pos < node->size && key > node->keys[pos])
                    pos++;
                delete node;
                node = readNode(indexFile, node->children[pos]);
                return searchRec(node, key);
            }
            else
            {
                long addressRecord = -1;
                for (int i = 0; i < node->size; i++)
                    if (node->keys[i] == key)
                        addressRecord = node->children[i];
                if (addressRecord)
                    throw "Error searching " + toString(key) + " in key B+Tree\n";

                return readRecord(this->fileName, addressRecord);
            }
        }
        catch (string err)
        {
            cout << err << endl;
            Record nullRecord;
            return nullRecord;
        }
}

template<typename TypeKey, typename RecordType>
RecordType BPTree<TypeKey, RecordType>::search(TypeKey key)
{
    return searchRec(root, key);
}


template<typename TypeKey, typename RecordType>
vector<RecordType> BPTree<TypeKey, RecordType>::searchInRange(TypeKey initialKey, TypeKey endKey)
{
    try
        {
            vector<RecordType> records;
            Bucket<RecordType> bucket = searchRec(this->root, initialKey);
            TypeKey currKey = initialKey;
            // while to itarate over the buckets until the key2 is found
            while (true)
            {
                Bucket<RecordType> currBucket = bucket;
                bool foundEndKey = false; // Flag to know if key2 is found

                // Iterate over records in current bucket
                for (int i = 0; i < currBucket.size; ++i)
                {
                    records.push_back(currBucket.record[i]);
                    // if key2 is found
                    if (currBucket.record[i].key == endKey)
                    {
                        break;
                        foundEndKey = true;
                    }
                }

                // finish the iteration
                if (foundEndKey)
                    break;

                // Check if it is at the end or can still iterate
                if (currBucket.nextBucket != -1)
                    currBucket = readBucket(dataFile, currBucket.nextBucket);
                else
                    throw "End key " + toString(endKey) + " not found\n";            }
        }
        catch (string err)
        {
            cout << err << endl;
            vector<RecordType> empty;
            return empty;
        }
}