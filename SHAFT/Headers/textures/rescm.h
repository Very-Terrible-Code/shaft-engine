#pragma once

#include "logic/math.h"
#include "common.h"
#include <glad/glad.h>

#define FLIPTEX 1
#define NOFLIPTEX 0

tex loadTexture(const char* location, int flip);

void loadTextureFromDB(tex *texturea);