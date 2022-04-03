#define ENABLE_EDITOR
#include "shaft-engine.h"

#include <glad/glad.h>

#include <iostream>



#define WinWidth 1280
#define WinHeight 720

int main()
{
    GAME game;

    initGame(&game, WinWidth, WinHeight);

    initImGui(&game);

    // lua test
    script test;
    loadAndRunScript(&test, "rsc/test.lua");
    LuaRef s = getGlobalScr(&test, "testString");
    LuaRef n = getGlobalScr(&test, "number");

    std::string luaString = s.cast<std::string>();
    int answer = n.cast<int>();

    std::cout << luaString << std::endl;
    std::cout << "And here's our number:" << answer << std::endl;

    while (game.gameRunning)
    {

        processKeys(&game);

        clearScreen();

        renderScene(&game);
#ifdef ENABLE_EDITOR
        ImGuiBeginRender(&game);

        sh_debugMenu(&game);

        tex_IMGUIMENU(&game);

        map_IMGUIMENU(&game);

        ImGuiEndRender();
#endif

        SDL_GL_SwapWindow(game.window);
    }

    return 0;
}
