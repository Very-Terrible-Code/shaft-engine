#pragma once
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
#include <LuaBridge/LuaBridge.h>
using namespace luabridge;

typedef struct script{
    lua_State* impl;
    bool exist;
    char * script[100000]; // Limits = Creativity
    bool running;
}script;


void loadAndRunScript(script* pscript, const char* location);
void initScript(script *pscript);

void runScript(script* pscript, const char* location);
void runScriptEmb(script *pscript);
LuaRef getGlobalScr(script * pscript, const char* var);