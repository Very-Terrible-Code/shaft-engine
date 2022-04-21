#pragma once
#include "common.h"
#include "game/phys.h"

void gameUpdate(GAME* game);



void aabbCheck(GAME *game, int item, std::vector<int> *collist);