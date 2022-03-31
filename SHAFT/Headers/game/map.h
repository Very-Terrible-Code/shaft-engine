#pragma once
#include <fstream>
#include "common.h"
#include <vector>
#include "logic/math.h"

typedef struct dectile{
    vec2 pos;
    vec2 scl;
    rot rcl;
    int tex;
}dectile, col, coltile;


typedef struct map{
    std::vector<dectile> decorationsTile;
    std::vector<coltile> collisionTile;
    std::vector<col> collision;
}map;


void loadMAP(char *name, GAME *game);

void saveMAP(char *name, GAME *game);



