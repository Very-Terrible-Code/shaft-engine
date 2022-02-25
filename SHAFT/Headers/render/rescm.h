#pragma once

#include "logic/math.h"
#include "common.h"
#include <glad/glad.h>

#define FLIPTEX 1
#define NOFLIP 0

tex loadTexture(const char* location, int flip);