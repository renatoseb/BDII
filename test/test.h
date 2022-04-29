#ifndef TEST
#define TEST

#include "../b+tree/b+tree.h"
#include "record.h"
#include<chrono>

using namespace std;

void testBTree(string dataFile)
{
    cout << "Running test of B+Tree\n";
    BPTree<int, GameData> *dataset = insertTest("data.dat");
    searchTest(dataset);
    rangeSearchTest(dataset); 
       
}

BPTree<int, GameData>* insertTest(string filename)
{
    auto begin = chrono::high_resolution_clock::now();

    fstream file(filename);
    vector<GameData *> dataset = readCSV("../datasets/game_data.csv");

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

void searchTest(BPTree<int, GameData> *btree)
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
    
    vector<GameData> search3 = btree->rangesearch(369, 400);

    auto final = chrono::high_resolution_clock::now();
    auto searchRangeTime = chrono::duration_cast<chrono::seconds>(final - begin).count();
    
    cout << "Time of search range: " << searchRangeTime << " seconds\n";
    cout << "Reads on disk: " << "----" << " times\n";
    cout << "Writes on disk: " << "----" << " times\n";

}
#endif