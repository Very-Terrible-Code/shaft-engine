#include "logic/scripts.h"

void loadAndRunScript(script *pscript, const char *location)
{
    pscript->impl = luaL_newstate();
    pscript->running = true;
    luaL_openlibs(pscript->impl);
    luaL_dofile(pscript->impl, location);

    // lua_pcall(pscript->impl, 0, 0, 0);
}

void initScript(script *pscript)
{
    pscript->impl = luaL_newstate();
    pscript->running = true;
    luaL_openlibs(pscript->impl);


}

void runScript(script *pscript, const char* location)
{
    luaL_dofile(pscript->impl, location);
}

void runScriptEmb(script *pscript)
{
    luaL_dostring(pscript->impl, (char*)pscript->script);
}

LuaRef getGlobalScr(script *pscript, const char *var)
{
    return getGlobal(pscript->impl, var);
}