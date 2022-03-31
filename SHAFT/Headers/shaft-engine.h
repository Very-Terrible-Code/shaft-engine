#pragma once
#include "common.h"


#include "logic/scripts.h"
#include "textures/manager.h"
#include "render/render.h"
#include "input/handler.h"
#include "game/map.h"


void initGame(GAME *instance, int width, int height);

void initImGui(GAME* game);

std::string readFileIntoString(const std::string& path);