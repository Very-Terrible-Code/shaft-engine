#pragma once
#include "common.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void initRenderer(GAME *game);

//void drawSprite(GAME *game, tex *texture, vec2 position, vec2 size, float rotate, vec3 color);

void WdrawSprite(GAME *game, GLuint *texture, glm::vec2 position,  glm::vec2 size, float rotate, glm::vec3 color);
