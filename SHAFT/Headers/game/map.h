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