#include "fast-cpp-csv-parser/csv.h"
#define CSV_IO_NO_THREAD
#include<string>
#include<iostream>

using namespace std;

int main()
{
    io::CSVReader<6, io::trim_chars<' '>, io::double_quote_escape<',','\"'>> in("../datasets/game_data.csv");
    in.read_header(io::ignore_extra_column, "ID", "Name", "Genres", "Platforms", "Runs", "ReleaseDate");
    int id; string name; string genres; string platforms; string runs; string releasedate; 
    while(in.read_row(id, name, genres, platforms, runs, releasedate)){
        cout << id << " " << name << " "<< runs << " " << platforms << releasedate << endl; 
    }

}