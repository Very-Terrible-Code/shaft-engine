#pragma once
#include "debug.h"


void map_IMGUIMENUBY(GAME *game);
void map_IMGUIMENU(GAME *game);
void map_dragOBJ(GAME *game, int item);

bool overlap(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void map_selct(GAME *game);