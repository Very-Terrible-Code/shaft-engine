#include "shaft-engine.h"
#include <glad/glad.h>
#include <iostream>
#include "render/stb_image.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#define WinWidth 640
#define WinHeight 480

int main()
{
    GAME game;

    initGame(&game, WinWidth, WinHeight);

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
    tex gaming = loadTexture("rsc/test.png", NOFLIP);
    

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(game.window, game.context);
    ImGui_ImplOpenGL3_Init();


    SDL_Event e;

    bool quit = false;

    while (quit == false)
    {
        
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            ImGui_ImplSDL2_ProcessEvent(&e);
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(game.window);
        ImGui::NewFrame();




        ImGui::Render();

        glViewport(0, 0, game.winw, game.winh);
        glClearColor(0.2f, 0.2f, 0.2f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        game.gl.shader.SetMatrix4("projection", game.gl.projection);

        WdrawSprite(&game, &gaming.glLoc, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 300.0f), sin(SDL_GetTicks() / 100.) * 100., glm::vec3(1.0f, 1.0f, 1.0f));
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(game.window);
    }

    return 0;
}
