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
            game->gameRunning = false;
        }
        if (game->input.inps.type == SDL_WINDOWEVENT)
        {
            switch (game->input.inps.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:{
                SDL_GetWindowSize(game->window, &game->winres.x, &game->winres.y);
                glViewport(0, 0, game->winres.x, game->winres.y);
                }
            }
        }
    }
}