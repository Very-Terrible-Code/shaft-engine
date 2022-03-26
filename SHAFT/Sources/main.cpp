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
    //tex gaming = loadTexture("rsc/awesomeface.png", NOFLIP);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("rsc/test.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    std::cout << texture << std::endl;

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

        WdrawSprite(&game, &texture, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 300.0f), sin(SDL_GetTicks() / 100.) * 100., glm::vec3(1.0f, 1.0f, 1.0f));
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(game.window);
    }

    return 0;
}
