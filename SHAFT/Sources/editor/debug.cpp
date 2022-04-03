#include "editor/debug.h"

void sh_debugMenu(GAME *game)
{
    static unsigned int debugtex = 0;
    ImGui::Begin("Debug Menu");
    if (ImGui::CollapsingHeader("Debug Texture Viewer"))
    {

        ImGui::InputInt("Texture Input", (int *)&debugtex);
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
        ImGui::Image((void *)(intptr_t)debugtex, ImVec2(100, 100), uv_min, uv_max, tint_col, border_col);
    }
    if (ImGui::Button("Resize render to current window size"))
    {
        game->gl.projection = glm::ortho(0.0f, (float)game->winres.x, (float)game->winres.y, 0.0f, -1.0f, 1.0f);
    }
    if (ImGui::Button("1280x720"))
    {
        SDL_SetWindowSize(game->window, 1280, 720);
        glViewport(0, 0, 1280, 720);
    }
    ImGui::SameLine();
    if (ImGui::Button("1920x1080"))
    {
        SDL_SetWindowSize(game->window, 1920, 1080);
        glViewport(0, 0, 1920, 1080);
    }
    if (ImGui::Button("Fullscreen"))
    {
        SDL_SetWindowFullscreen(game->window, SDL_WINDOW_FULLSCREEN);
    }
    ImGui::SameLine();
    if (ImGui::Button("Windowed"))
    {
        SDL_SetWindowFullscreen(game->window, 0);
    }
    ImGui::End();
}