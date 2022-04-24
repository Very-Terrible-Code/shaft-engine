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
    // Position
    vec2 pos;
    // Scale
    vec2 scl;
    // Rotation
    float rot;
    // Texture
    int tex;
    // Script
    script scr;
    // Name
    char *tag[32];
    // Chipmunk 2D info
    physInfo phys;
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

// Header section of map file. Stores gravity and global scripts etc.
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

// Texture
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

// Texture with tag
typedef struct tagtex
{
    tex texture;
    // Name
    char *tag[64];
} tagtex;

typedef struct texdb
{
    std::vector<tagtex> textures;
    bool loadTexCheck;
    char *sourcefile[64];
} texdb;

// Player object
typedef struct player{
    vec2 pos;
    physInfo phys;

    int health;

} player;

// Game instance
typedef struct GAME
{
    // SDL2 Window
    SDL_Window *window;
    // Stores states associated with the OpenGL instance
    SDL_GLContext context;
    inHandle input;
    // Texture database
    texdb texm;
    // Window resolutions
    vec2i winres;
    // Original window resolution
    vec2i orgwinres;
    // Current FPS of game
    int fps;
    // The map
    map cmap;
    // Stores random OpenGL stuff
    openglSet gl;
    // Part of Chipmunk 2D - essentially a physics environment
    cpSpace *world;
    // If application is running
    bool gameRunning;
    // If game is running in editor (ie. when start/stop testing is pressed)
    bool edgameRunning;
    // The player object
    player mplay;

//#ifdef ENABLE_EDITOR
    Shader debugBLOCK;
//#endif
} GAME;
