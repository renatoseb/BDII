#ifndef NODE_H
#define NODE_H

template<typename TypeKey>
struct NodeB
{
public:
    TypeKey keys[R];
    long childrens[R + 1];

    // Current size of keys
    int size;
    long nextNode = 0;
    // Save the position of node in indexfile 
    long id; 

    NodeB()
    {
        for(int i = 0; i < R+1; ++i)
            this->childrens[i] = 0; 
    }
    bool imLeaf()
    {
        return this->childrens[0] == 0;
    }

    long getChildren(int i)
    {
        return this->childrens[i];
    }

    void insertInNode(int pos, KeyType key) {
        int i = this->size;

        while (i > pos) 
        {
            keys[i] = keys[i - 1];
            childrens[i + 1] = childrens[i];
            i--;
        }
        keys[i] = key;
        childrens[i + 1] = childrens[i];

        this->size++;
    }
};


#endif