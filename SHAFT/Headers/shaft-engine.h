#pragma once
#include "logic/scripts.h"
#include "render/rescm.h"
#include "render/render.h"
#include "game/map.h"
#include "common.h"
#include "render/texfuncs.h"

void initGame(GAME *instance, int width, int height);

std::string readFileIntoString(const std::string& path);