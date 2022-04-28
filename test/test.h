#ifndef TEST
#define TEST

#include "../b+tree/b+tree.h"
#include<chrono>

using namespace std;

void testBTree(string dataFile)
{
    cout << "Running test of B+Tree\n";

    auto begin = chrono::high_resolution_clock::now();

    fstream file(dataFile);
    


}
#endif