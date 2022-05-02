#include "b+tree.h"
#include<cmath>


template<typename TypeKey, typename RecordType>
NodeB<TypeKey> readNode(string file, long address)
{
    NodeB<TypeKey> node;
    ifstram file_(file.c_str(), ios::in | ios::binary);
    file_.read(reinterpret_cast<char *>(&node), sizeof(node));
    file_.close();
    return node;
}

template<typename TypeKey, typename RecordType>
RecordType BPTree<TypeKey, RecordType>::searchRec(NodeB<TypeKey> *node, TypeKey key)
{
        try
        {
            if (!node->isLeaf)
            {
                int pos = 0;
                while (pos < node->size && key > node->keys[pos])
                    pos++;
                delete node;
                node = readNode(this->indexFile, node->children[pos]);
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
                    throw "End key " + toString(endKey) + " not found\n";            
            }
        }
        catch (string err)
        {
            cout << err << endl;
            vector<RecordType> empty;
            return empty;
        }
}



template<typename TypeKey, typename RecordType>
bool BPTree<TypeKey, RecordType>::removeRec(NodeB<TypeKey>& node, TypeKey key)
{
    // Search the position of the key to remove
    int pos = 0;
    while(pos < node.size && node.keys[pos] < key)
        ++pos;    

    // If isn't leaf 
    if(!node.imLeaf())
    {
        // Searching 
        if(node.keys[pos] == key && pos != node.size)
        {
            NodeB<TypeKey> nextNode = readNode(node.id, node.childrens[pos + 1]);
            node.keys[pos] = succesor(nextNode);
            writeNode(node.id, node);    
            ++pos;
        }
        NodeB<TypeKey> children = readNode(node.childrens[pos]);

        // Tendré que retornar el state?
        remove(children, key);
        if(node.size < floor(R/2.0))
        {
            // do operations to modify structure of the tree
            bool canSteal = borrowFromSibling(children, node, pos);
            if(!canSteal)
            {
                // if its leaf merge with another leaf
                if(children.imLeaf())
                    mergeLeaf(node, children, pos);
                // else merge with the parent
                else
                {
                    bool canMerge = mergeWithParent(node, children, pos);
                    if(!canMerge)
                        decreaseHeight(node, children, pos);
                }
            }
        }

    }
    else if(node.keys[pos] == key)
    {
        node.removeKeyInNode(pos);
        writeNode(node.id, node);
    }
    
    return true;
} 

template<typename TypeKey, typename RecordType>
bool BPTree<TypeKey, RecordType>::remove(TypeKey key)
{
    NodeB<TypeKey> root = readNode(this->dataFile);
    removeRec(root, key);
}


template<typename TypeKey, typename RecordType>
TypeKey BPTree<TypeKey, RecordType>::succesor(NodeB<TypeKey>& node)
{
    // Siempre q
    while(!node.imLeaf())
        node = readNode(node.childrens[0])
    
    if(node.size == 1)
    {
        if(node.nexNode == -1)
            return NULL;
        node = readNode(node.nextNode);
        return node.keys[0];
    }
    return node.keys[1];
}


template<typename TypeKey, typename RecordType>
void BPTree<TypeKey, RecordType>::decreaseHeight(NodeB<TypeKey>& node, NodeB<TypeKey>& nodeUnderflow, int pos)
{
    if(pos != node.size)
    {
        NodeB<TypeKey> children = readNode(node.childrens[pos]);
        if(children.size < floor(R / 2.0))
        {
            NodeB<TypeKey> sibling = readNode(node.getChildren(pos + 1));
            sibling.insertInNode(0, node.keys[pos]);
            int last = nodeUnderflow.size;
            sibling.childrens[0] = nodeUnderflow.getChildren(last);

            for(int i = last - 1; i >= 0; --i)
            {
                sibling.insertInNode(0, nodeUnderflow.data[i]);
                sibling.childrens[0] = nodeUnderflow.getChildren(i);
            }

            node.deleteInNode(pos);
            node.childrens[pos] = sibling.id;
            writeNode(sibling.id, sibling);
            writeNode(node.id, node);
            return;
        }
    }

    NodeB<TypeKey> sibling = readNode(node.getChildren(pos - 1));
    int last = sibling.size;
    sibling.insertInNode(last, node.data[pos - 1]);
    sibling.childrens[last + 1] = nodeUnderflow.getChildren(0);

    for(int i = 0; i < nodeUnderflow.size; ++i)
    {
        last = sibling.size;
        sibling.insertInNode(last, nodeUnderflow.data[i]);
        sibling.childrens[last + 1] = nodeUnderflow.getChildren(i + 1);
    }

    node.deleteInNode(pos - 1);
    node.childrens[pos - 1] = sibling.id;

    writeNode(sibling.id, sibling);
    writeNode(node.id, node);
}


template<typename TypeKey, typename RecordType>
void BPTree<TypeKey, RecordType>::mergeLeaf(NodeB<TypeKey>& node, NodeB<TypeKey> nodeUnderflow, int pos)
{
    if (pos - 1 >= 0) 
    {
		NodeB<TypeKey> sibling = readNode(node.getChildren(i));
		for (int i = 0; i < nodeUnderflow.size; ++i) 
        {
		  int pos_ = sibling.size;
		  sibling.insertInNode(pos_, nodeUnderflow.keys[i]);
		}

		sibling.nextNode = nodeUnderflow.nextNode;
		node.deleteInNode(pos - 1);
		writeNode(sibling.id, sibling);
		writeNode(node.id, node);
    } 
    else 
    {
		NodeB<TypeKey> sibling = readNode(node.getChildren(1));
		for (int i = 0; i < sibling.size; ++i) 
        {
		  int pos_ = nodeUnderflow.size;
		  nodeUnderflow.insertInNode(pos, sibling.keys[i]);
		}
		nodeUnderflow.nextNode = sibling.nextNode;
		node.deleteInNode(0);
		writeNode(nodeUnderflow.id, nodeUnderflow);
		writeNode(node.id, node);
	}
}


template<typename TypeKey, typename RecordType>
bool BPTree<TypeKey, RecordType>::mergeWithParent(NodeB<TypeKey>& node, NodeB<TypeKey>& nodeUnderflow, int pos)
{
    if(pos != 0)
    {
        NodeB<TypeKey> sibling = readNode(node.getChildren(pos - 1));
        if(sibling.size - 1 >= floor(R / 2.0))
        {
            TypeKey temp = node.keys[pos];
            nodeUnderflow.insertInNode(0, temp);
            node.keys[pos] = sibling.keys[0];
            nodeUnderflow.childrens[1] = sibling.getChildren(0);
            sibling.childrens[0] = sibling.getChildren(1);
            sibling.deleteInNode(0);

            writeNode(sibling.id, sibling);
            writeNode(node.id, node);
            writeNode(nodeUnderflow.id, nodeUnderflow);
            return true;
        }
        else if(pos != R)
        {
            NodeB<TypeKey> sibling = readNode(node.getChildren(pos + 1));
            if(sibling.size - 1 >= floor(R / 2.0))
            {
                NodeB<TypeKey> temp = node.keys[pos];
                nodeUnderflow.insertInNode(0, temp);
                node.keys[pos] = sibling.keys[0];
                nodeUnderflow.childrens[1] = sibling.getchildren(0);
                sibling.children[0] = sibling.getChildren(1);
                sibling.deleteInNode(0);

                writeNode(sibling.id, sibling);
                writeNode(node.id, node);
                writeNode(nodeUnderflow.id, nodeUnderflow);
                return true;
            }
        }
    }
}


template<typename TypeKey, typename RecordType>
bool BPTree<TypeKey, RecordType>::borrowFromSibling(NodeB<TypeKey>& nodeUnderflow, NodeB<TypeKey>& node, int pos)
{
    if (nodeUnderflow.imLeaf()) 
    {
		if (pos != node.size) 
        {
            NodeB<TypeKey> sibling = readNode(node.getChildren(pos + 1));
            if (sibling.size - 1 >= floor(R / 2.0)) 
            {
                NodeB<TypeKey> temp = sibling.keys[0];
                NodeB<TypeKey> temp2 = sibling.keys[1];
                sibling.deleteInNode(0);
                nodeUnderflow.insertInNode(sibling.size - 1, temp);
                node.keys[pos] = temp2;
                writeNode(sibling.id, sibling);
                writeNode(nodeUnderflow.id, nodeUnderflow);
                writeNode(node.id, node);
                return true;
            }
		}
		if (pos > 0) 
        {
		    NodeB<TypeKey> sibling = readNode(node.getChildren(pos - 1));
		    if (sibling.count - 1 >= std::floor(TREE_ORDER / 2.0)) 
            {
                NodeB<TypeKey> temp = sibling.keys[sibling.size - 1];
                sibling.deleteInNode(sibling.size - 1);
                nodeUnderflow.insertInNode(0, temp);
                node.keys[pos - 1] = temp;
                writeNode(sibling.id, sibling);
                writeNode(nodeUnderflow.id, nodeUnderflow);
                writeNode(node.id, node);
                return true;
		    }
		}
	}
	  return false;
}