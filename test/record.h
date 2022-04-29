#ifndef RECORD_H
#define RECORD_H

#include<string>
#include<vector>
#include<iostream>
#include "fast-cpp-csv-parser/csv.h"
using namespace std;

struct GameData
{
    int id;
    string name;
    string genres;
    string platforms;
    string runs;
    string releaseDate;
};

vector<GameData *> readCSV(string file)
{
    io::CSVReader<6, io::trim_chars<' '>, io::double_quote_escape<',','\"'>> in("../datasets/game_data.csv");
    in.read_header(io::ignore_extra_column, "ID", "Name", "Genres", "Platforms", "Runs", "ReleaseDate");
    int id; string name; string genres; string platforms; string runs; string releasedate; 
    vector<GameData *> dataset;
    while(in.read_row(id, name, genres, platforms, runs, releasedate))
    {
        GameData *data = new GameData();
        data->id = id;
        data->name = name;
        data->genres = genres;
        data->platforms = platforms;
        data->runs = runs;
        data->releaseDate = releasedate;
        dataset.push_back(data);
    }
    return dataset;

}

#endif