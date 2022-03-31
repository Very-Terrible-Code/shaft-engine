#pragma once
#include "textures/rescm.h"
#include "common.h"
#include <vector>



void loadTextureDB(char* name, GAME* texdata);

void saveTextureDB(char* name, GAME* texdata);

void clearTEX(GAME* texdata);

void removeTEX(int item, GAME* texdata);

void tex_IMGUIMENU(GAME *game);