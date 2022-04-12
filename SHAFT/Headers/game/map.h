#pragma once
#include <fstream>
#include "common.h"
#include <vector>
#include "logic/math.h"
#include "common.h"

void loadMAP(char *name, GAME *game);

void saveMAP(char *name, GAME *game);

void removeIDB(GAME* game, int id);