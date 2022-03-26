#include "input/handler.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

void processKeys(GAME *game)
{
    while (SDL_PollEvent(&game->input.inps) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&game->input.inps);
        if (game->input.inps.type == SDL_QUIT)
        {
            game->gameRunning = true;
        }
    }
}