#include "shaft-engine.h"
#include <glad/glad.h>

#include <iostream>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

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

    // tex load
    tex gaming = loadTexture("rsc/test.png", NOFLIPTEX);

    static unsigned int debugtex = 0;
    while (game.gameRunning)
    {

        processKeys(&game);

        clearScreen();

        ImGuiBeginRender(&game);

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
            game.gl.projection = glm::ortho(0.0f, (float)game.winres.x, (float)game.winres.y, 0.0f, -1.0f, 1.0f);
        }
        if (ImGui::Button("1280x720"))
        {
            SDL_SetWindowSize(game.window, 1280, 720);
            glViewport(0, 0, 1280, 720);
        }
        ImGui::SameLine();
        if (ImGui::Button("1920x1080"))
        {
            SDL_SetWindowSize(game.window, 1920, 1080);
            glViewport(0, 0, 1920, 1080);
        }
        if (ImGui::Button("Fullscreen"))
        {
            SDL_SetWindowFullscreen(game.window, SDL_WINDOW_FULLSCREEN);
        }
        ImGui::SameLine();
        if (ImGui::Button("Windowed"))
        {
            SDL_SetWindowFullscreen(game.window, 0);
        }
        ImGui::End();

        tex_IMGUIMENU(&game);

        ImGui::Render();

        game.gl.shader.SetMatrix4("projection", game.gl.projection);

        WdrawSprite(&game, &gaming.glLoc, glm::vec2(100. + sin(SDL_GetTicks() / 100.) * 100., 200.0f), glm::vec2(100.0f, 100.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

        ImGuiEndRender();
        SDL_GL_SwapWindow(game.window);
    }

    return 0;
}
