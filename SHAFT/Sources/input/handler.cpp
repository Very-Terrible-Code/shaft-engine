#include "input/handler.h"
//#ifdef ENABLE_EDITOR
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
//#endif

void processKeys(GAME *game)
{
    while (SDL_PollEvent(&game->input.inps) != 0)
    {
//#ifdef ENABLE_EDITOR
        ImGui_ImplSDL2_ProcessEvent(&game->input.inps);
//#endif
        if (game->input.inps.type == SDL_QUIT)
        {
            game->gameRunning = false;
        }
        if (game->input.inps.type == SDL_WINDOWEVENT)
        {
            switch (game->input.inps.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
            {
                
                SDL_GetWindowSize(game->window, &game->winres.x, &game->winres.y);
                game->percs.x = (float)(((float)game->winres.x * 100) / (float)game->orgwinres.x)  / 100 ;
                game->percs.y = (float)(((float)game->winres.y * 100) / (float)game->orgwinres.y)  / 100 ;
                if(game->percs.x == 0){
                    game->percs.x = 1;
                }
                if(game->percs.y == 0){
                    game->percs.y = 1;
                }
                printf("------\nX: %f\nY: %f\n", game->percs.x, game->percs.y);
                glViewport(0, 0, game->winres.x, game->winres.y);
            }
            }
        }
    }
}