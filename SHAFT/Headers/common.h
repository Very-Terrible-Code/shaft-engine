#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "logic/scripts.h"
#include "logic/math.h"
#include "render/shader.h"
#include <chipmunk/chipmunk.h>

#define STATIC 0
#define DYNAMIC 1

typedef struct phsyInfo{
        bool physOn;
        int dynst;
        cpFloat mass;
        cpFloat friction;
        cpBody *body;
        cpShape *shape;
        cpFloat inert;

}physInfo;

typedef struct tile
{
    vec2 pos;
    vec2 scl;
    float rot;
    int tex;
    script scr;
    char *tag[32];
    int attc;
    physInfo phys;
    int id;
} tile;

typedef struct spwn{
    vec2 loc;
    char *tag[32];
}spwn;

typedef struct map
{
    std::vector<tile> tiles;
    std::vector<spwn> spawns;
    script globalscr;
    cpVect gravity;
} map;

typedef struct mapHeader{
    int s_size;
    int s_spsize;
    cpVect gravity;
    char texloc[64];
    script globalscr;
}mapHeader;

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
    float deltaTime;
    float lastframe;

    unsigned int MSFBO, FBO; // MSFBO = Multisampled FBO. FBO is regular, used for blitting MS color-buffer to texture
    unsigned int RBO; // RBO is used for multisampled color buffer
    unsigned int VAO;
    unsigned int tecg;
    Shader screenShader;
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

typedef struct player{
    vec2 pos;
    physInfo phys;

    int health;

} player;

typedef struct GAME
{
    SDL_Window *window;
    SDL_GLContext context;
    inHandle input;
    texdb texm;
    vec2i winres;
    vec2i orgwinres;
    vec2 percs;
    int fps;
    map cmap;
    openglSet gl;
    cpSpace *world;
    bool gameRunning;
    bool edgameRunning;
    player mplay;

//#ifdef ENABLE_EDITOR
    Shader debugBLOCK;
//#endif
} GAME;
