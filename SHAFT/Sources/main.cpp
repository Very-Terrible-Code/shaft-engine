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
    // Define game instance
    GAME game;

    // Initialises game. (EVERYTHING - OpenGL, SDL, Chipmunk2D)
    initGame(&game, WinWidth, WinHeight);

    // Initialise ImGui
    initImGui(&game);

    // While application is running
    while (game.gameRunning)
    {
        // Process input
        processKeys(&game);
        // If game is running inside editor
        if (game.edgameRunning)
        {
            gameUpdate(&game);
        }

        // Render all objects to screen
        renderScene(&game);

#ifdef ENABLE_EDITOR

        ImGuiBeginRender(&game);

        sh_debugMenu(&game);

        tex_IMGUIMENU(&game);

        map_IMGUIMENU(&game);

        ImGuiEndRender();

#endif

        // Temporary delta time calculation
        if (game.gl.deltaTime < 16) {
            SDL_Delay(16 - game.gl.deltaTime);
        }

        // Swaps out window frame buffer for new frames
        SDL_GL_SwapWindow(game.window);
    }

    return 0;
}
