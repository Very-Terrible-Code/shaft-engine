#pragma once
#include <fstream>
#include "common.h"
#include <vector>
#include "logic/math.h"
#include "common.h"

void loadMAP(char *name, GAME *game);

void saveMAP(char *name, GAME *game);

void map_IMGUIMENU(GAME *game);

void map_eCOL_IMGUIMENU(GAME* game, dectile *dct);

bool isOverlapping(vec2 l1, vec2 r1, vec2 l2, vec2 r2);