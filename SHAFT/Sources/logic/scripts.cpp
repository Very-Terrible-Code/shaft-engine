#include "logic/scripts.h"

void loadAndRunScript(script *pscript, const char *location)
{
    pscript->impl = luaL_newstate();
    pscript->running = true;
    luaL_openlibs(pscript->impl);
    luaL_dofile(pscript->impl, location);

    // lua_pcall(pscript->impl, 0, 0, 0);
}

LuaRef getGlobalScr(script *pscript, const char *var)
{
    return getGlobal(pscript->impl, var);
}