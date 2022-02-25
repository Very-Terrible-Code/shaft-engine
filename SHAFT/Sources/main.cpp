#include "shaft-engine.h"
#include <glad/glad.h>
#include <iostream>

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

    SDL_Event e;

    bool quit = false;

    while (quit == false)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        glViewport(0, 0, game.winw, game.winh);
        glClearColor(0.2f, 0.2f, 0.2f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        game.gl.shader.SetInteger("image", gaming.glLoc);
        game.gl.shader.SetMatrix4("projection", game.gl.projection);
        WdrawSprite(&game, &gaming.glLoc, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), sin(SDL_GetTicks() / 100.) * 100., glm::vec3(1.0f, 1.0f, 1.0f));

        SDL_GL_SwapWindow(game.window);
    }

    return 0;
}
