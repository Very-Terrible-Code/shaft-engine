#pragma once
#include "common.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void initRenderer(GAME *game);

void raw_drawSP(GAME *game, GLuint *texture, glm::vec2 position,  glm::vec2 size, float rotate, glm::vec3 color);

void clearScreen();

void renderScene(GAME* render);

#ifdef ENABLE_EDITOR
void ImGuiBeginRender(GAME* render);

void ImGuiEndRender();
#endif