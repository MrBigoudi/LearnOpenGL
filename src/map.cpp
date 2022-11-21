#include "map.hpp"

#include <stdio.h>
#include <vector>
#include <fstream>

std::vector<std::vector<int>>initMap(std::string map){
    std::ifstream fileMap(map);

    std::string line;
    getline(fileMap, line);
    int raws = atoi(line.c_str());
    getline(fileMap, line);
    int cols = atoi(line.c_str());

    std::vector<std::vector<int>> res;
    for(int i=0; i<raws; i++){
        getline(fileMap, line);
        std::vector<int> raw;
        for(int j=0; j<cols; j++){
            // printf("%s\n", line.c_str());
            raw.push_back((int)(line[j]-'0'));
        }
        res.push_back(raw);
    }

    return res;
}

void printMap(std::vector<std::vector<int>> map){
    int raws = map.size();
    int cols = map[0].size();

    for (int i=0; i<raws; i++){
        for (int j=0; j<cols; j++){
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}