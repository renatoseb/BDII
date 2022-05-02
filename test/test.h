#ifndef TEST
#define TEST

#include "../b+tree/b+tree.h"
#include "record.h"
#include "page.h"
#include<chrono>

using namespace std;

void testBTree(string dataFile)
{
    cout << "Running test of B+Tree\n";
    BPTree<int, GameData> *dataset = insertBPTreeTest("data.dat");
    searchBTreeTest(dataset);
    rangeSearchTest(dataset); 
       
}

BPTree<int, GameData>* insertBPTreeTest(string filename)
{

    Page<NodeB<int>> btreeIndex("indexbtree.bin"); 
    BPTree<int, NodeB<int>> bptree(btreeIndex);
    Page<GameData> records("data.bin");
    vector<GameData *> dataset = readCSV("../datasets/game_data.csv");

    auto begin = chrono::high_resolution_clock::now();
    BPTree<int, GameData> *btree = new BPTree<int, GameData>("data.dat", "index.dat");   
    for(auto i: dataset)
    {
        btree->insert(*i);
    }

    auto final = chrono::high_resolution_clock::now();
    auto insertsTime = chrono::duration_cast<chrono::seconds>(final - begin).count();

    cout << "Time of Insertion: " << insertsTime << " seconds\n";
    cout << "Reads on disk: " << "----" << " times\n";
    cout << "Writes on disk: " << "----" << " times\n";

    return btree;
   
}

void searchBTreeTest(BPTree<int, GameData> *btree)
{
    auto begin =  chrono::high_resolution_clock::now();
    
    GameData search1 = btree->search(400);
    GameData search2 = btree->search(369);

    auto final = chrono::high_resolution_clock::now();
    auto searchTime = chrono::duration_cast<chrono::seconds>(final - begin).count();
    
    cout << "Time of writing: " << searchTime << " seconds\n";
    cout << "Reads on disk: " << "----" << " times\n";
    cout << "Writes on disk: " << "----" << " times\n";
}

void rangeSearchTest(BPTree<int, GameData> *btree)
{
    auto begin =  chrono::high_resolution_clock::now();
    
    vector<GameData> search3 = btree->searchInRange(369, 400);

    auto final = chrono::high_resolution_clock::now();
    auto searchRangeTime = chrono::duration_cast<chrono::seconds>(final - begin).count();
    
    cout << "Time of search range: " << searchRangeTime << " seconds\n";
    cout << "Reads on disk: " << "----" << " times\n";
    cout << "Writes on disk: " << "----" << " times\n";

}
#endif