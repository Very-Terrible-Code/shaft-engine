#include "shaft-engine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "render/render.h"
#include <glad/glad.h>

std::string readFileIntoString(const std::string &path)
{
    std::ifstream input_file(path);
    if (!input_file.is_open())
    {
        std::cerr << "Could not open the file - '"
                  << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

void initGame(GAME *instance, int width, int height)
{
    printf("SHAFT Engine 1.00\nhttps://github.com/PipeWarp/SHAFTEng\n");
    fflush(NULL);
#ifdef SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
#endif
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        exit(0);
    }
    instance->window = SDL_CreateWindow("SHAFT", 0, 0, width, height, SDL_WINDOW_OPENGL);
    instance->context = SDL_GL_CreateContext(instance->window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    SDL_GL_SetSwapInterval(1);
    instance->winw = width;
    instance->winh = height;
    instance->gl.shader.Compile(readFileIntoString("rsc/transp.vert").c_str(), readFileIntoString("rsc/texc.frag").c_str());
    initRenderer(instance);

    return;
}
