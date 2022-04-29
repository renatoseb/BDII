#ifndef RECORD_H
#define RECORD_H

#include<string>
#include<vector>
#include<iostream>
#include<cstring>
#include "fast-cpp-csv-parser/csv.h"
using namespace std;

struct GameData
{
    int id;
    char name[30];
    char genres[15];
    char platforms[100];
    int runs;
    char releaseDate[10];
};

vector<GameData *> readCSV(string file)
{
    io::CSVReader<6, io::trim_chars<' '>, io::double_quote_escape<',','\"'>> in("../datasets/game_data.csv");
    in.read_header(io::ignore_extra_column, "ID", "Name", "Genres", "Platforms", "Runs", "ReleaseDate");
    int id; string name; string genres; string platforms; int runs; string releasedate; 
    vector<GameData *> dataset;
    while(in.read_row(id, name, genres, platforms, runs, releasedate))
    {
        GameData *data = new GameData();
        data->id = id;
        strcpy(data->name, name.c_str());
        strcpy(data->genres, genres.c_str());
        strcpy(data->platforms, platforms.c_str());
        data->runs = runs;
        strcpy(data->releaseDate, releasedate.c_str());
        dataset.push_back(data);
    }
    return dataset;

}

#endif