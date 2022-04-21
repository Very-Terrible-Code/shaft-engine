#pragma once
#include "common.h"


#include "logic/scripts.h"
#include "textures/manager.h"
#include "render/render.h"
#include "input/handler.h"
#include "game/map.h"
#include "game/game.h"


#ifdef ENABLE_EDITOR
#include "editor/debug.h"
#include "editor/mapedit.h"
#include "editor/texedit.h"
#endif
void  debugCallback(GLenum source, GLenum type, GLuint , GLenum severity,
                    GLsizei , const GLchar *message, const void *);

void initGame(GAME *instance, int width, int height);

void initImGui(GAME* game);

std::string readFileIntoString(const std::string& path);