#define ENABLE_EDITOR
#define DEBUG
#include "shaft-engine.h"
#include "editor/debug.h"
#include <glad/glad.h>

#include <iostream>

#define WinWidth 1280
#define WinHeight 720

int main()
{
    GAME game;

    initGame(&game, WinWidth, WinHeight);

    initImGui(&game);

    while (game.gameRunning)
    {
        processKeys(&game);
        if (game.edgameRunning)
        {
            gameUpdate(&game);
        }

        renderScene(&game);

#ifdef ENABLE_EDITOR

        ImGuiBeginRender(&game);

        sh_debugMenu(&game);

        tex_IMGUIMENU(&game);

        map_IMGUIMENU(&game);

        ImGuiEndRender();

#endif

        if (game.gl.deltaTime < 16) {
            SDL_Delay(16 - game.gl.deltaTime);
        }

        SDL_GL_SwapWindow(game.window);
    }

    return 0;
}
