#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include "logic/scripts.h"
#include "logic/math.h"
#include "render/shader.h"

#define S_DECTILE 0
#define S_COLTILE 1
#define S_COL 2

typedef struct dectile
{
    vec2 pos;
    vec2 scl;
    float rot;
    int tex;
    script scr;
    char *tag[32];
    int attc;
} dectile, coltile;

typedef struct col
{
    vec2 pos;
    vec2 scl;
    float rot;
    script scr;
    char *tag[32];
    int attc;
} col;
typedef struct drawOIT
{
    int type;
    int id;
} drawOIT;
typedef struct map
{
    std::vector<dectile> decorationsTile;
    std::vector<coltile> collisionTile;
    std::vector<col> collision;
    std::vector<drawOIT> drawOrder;
    script globalscr;
} map;

typedef struct keyStat
{
    bool keydown;
    bool keyup;
    bool keyheld;
} keyStat;

typedef struct key
{
    unsigned int key;
    char *tag[32];
    keyStat status;
} key;

typedef struct keydb
{
    std::vector<key> keys;

} keydb;

typedef struct inHandle
{
    SDL_Event inps;
    keydb db;
} inHandle;

typedef struct openglSet
{
    glm::mat4 projection;
    GLuint quadVAO;
    Shader shader;

} openglSet;

typedef struct tex
{
    unsigned int glLoc;
    char *location[64];
    vec2i size;
    int channeli;
    bool loaded;
} tex;

typedef struct texdbheader
{
    int texsize;
} texdbheader;

typedef struct tagtex
{
    tex texture;
    // int id;
    char *tag[64];
} tagtex;

typedef struct texdb
{
    std::vector<tagtex> textures;
    bool loadTexCheck;
    char *sourcefile[64];
} texdb;

typedef struct GAME
{
    SDL_Window *window;
    SDL_GLContext context;
    inHandle input;
    texdb texm;
    vec2i winres;
    vec2i orgwinres;
    map cmap;
    openglSet gl;
    bool gameRunning;

//#ifdef ENABLE_EDITOR
    Shader debugBLOCK;
//#endif
} GAME;
