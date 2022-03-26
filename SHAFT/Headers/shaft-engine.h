#pragma once
#include "render/rescm.h"
#include "common.h"
#include "logic/scripts.h"

#include "render/render.h"
#include "input/handler.h"
#include "game/map.h"
#include "render/texfuncs.h"

void initGame(GAME *instance, int width, int height);

void initImGui(GAME* game);

std::string readFileIntoString(const std::string& path);