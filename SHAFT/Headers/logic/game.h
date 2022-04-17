#pragma once
#include "common.h"

void gameUpdate(GAME* game);

void physUpdate(GAME* game);

void aabbCheck(GAME *game, int item, std::vector<int> *collist);