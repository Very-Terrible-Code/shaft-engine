#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "logic/scripts.h"
#include "logic/math.h"
#include "render/shader.h"

typedef struct openglSet
{
    glm::mat4 projection;
    GLuint quadVAO;
    Shader shader;

} openglSet;

typedef struct GAME
{
    SDL_Window *window;
    int winw;
    int winh;
    openglSet gl;
} GAME;

typedef struct tex
{
    unsigned int glLoc;
    char *location;
    vec2i size;
    int channeli;
} tex;
