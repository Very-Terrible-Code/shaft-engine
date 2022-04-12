#pragma once
#include "debug.h"



void map_IMGUIMENU(GAME *game);
void map_IMGUIDISPLAYDCT(GAME* game, int id,  int drid, drawOIT *sel);
void map_dragOBJ(GAME *game, drawOIT *item);
void map_eCOL_IMGUIMENU(GAME *game);
bool overlap(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void map_selct(GAME *game);