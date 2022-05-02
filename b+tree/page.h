#ifndef PAGE_H
#define PAGE_H

#include<fstream>
#include<string>

using namespace std;

template<typename RecordType>
struct Page:
{
    private:
        string fileName;
        bool isEmpty;
        long pageIdSize;

        int writesCount = 0;
        int readsCount =  0;
    public: 
        Page(string fileName)
        {
            this->fileName = fileName;
            isEmpty = false;
        }

       	void save(long pos, RecordType& record) 
        {
            ofstream file(fileName, ios::bin);
            file.seekp(pos * sizeof(RecordType), os::beg);
            file.write(reinterpret_cast<char*>(&record), sizeof(record));
            file.close();
            writesCount++;
	    } 
        
        void remove(long pos)
        {
            char pivot = '|';
            fstream fileIn(fileName, ios::in | ios::out);
            file.seekg(pos * sizeof(RecordType), ios::beg);
            file.write(&pivot, sizeof(char));
            writesCount++;
        }        

        RecordType get(long n)
        {
            ifstream file(fileName);
            RecordType newRecord;
            file.seekg(n * sizeof(RecordType), ios::beg);
            file.read(reinterpret_cast<char *>(&newRecord), sizeof(RecordType));
            ++readsCount;
            return newRecord;
        }

        int getReadEntries()
        {
            return this->readsCount;
        }

        int getWriteEntries()
        {
            return this->writesCount;
        }
};
#endif