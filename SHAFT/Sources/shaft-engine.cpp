#include "shaft-engine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "render/render.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
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
#ifdef DEBUG
void debugCallback(GLenum source, GLenum type, GLuint, GLenum severity,
                   GLsizei, const GLchar *message, const void *)
{
    std::string srcStr = "UNDEFINED";
    severity = severity;
    switch (source)

    {
    case GL_DEBUG_SOURCE_API:
        srcStr = "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        srcStr = "WINDOW_SYSTEM";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        srcStr = "SHADER_COMPILER";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        srcStr = "THIRD_PARTY";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        srcStr = "APPLICATION";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        srcStr = "OTHER";
        break;
    }

    std::string typeStr = "UNDEFINED";

    switch (type)
    {

    case GL_DEBUG_TYPE_ERROR:
        //	__debugbreak();
        typeStr = "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        typeStr = "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        typeStr = "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        typeStr = "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        typeStr = "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        typeStr = "OTHER";
        break;
    }

    std::cout << "OpenGL " << typeStr << " [" << srcStr << "]: " << message << std::endl;
}
#endif
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
    instance->window = SDL_CreateWindow("SHAFT", 0, 0, width, height, SDL_WINDOW_OPENGL + SDL_WINDOW_RESIZABLE);
    instance->context = SDL_GL_CreateContext(instance->window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    SDL_GL_SetSwapInterval(1);
#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, NULL,
                          GL_TRUE);
    glDebugMessageCallback(debugCallback, nullptr);
#endif
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
    instance->winres.x = width;
    instance->winres.y = height;
    instance->orgwinres.x = width;
    instance->orgwinres.y = height;
    instance->percs.x = 1;

    instance->percs.y = 1;
    instance->gl.shader.Compile(readFileIntoString("rsc/shaders/transp.vert").c_str(), readFileIntoString("rsc/shaders/texc.frag").c_str());
    instance->cmap.gravity = 1.;
    instance->gl.screenShader.Compile(readFileIntoString("rsc/shaders/transp.vert").c_str(), readFileIntoString("rsc/shaders/fb.frag").c_str());
    initScript(&instance->cmap.globalscr);
    //#ifdef ENABLE_EDITOR
    instance->debugBLOCK.Compile(readFileIntoString("rsc/shaders/transp.vert").c_str(), readFileIntoString("rsc/shaders/rawcol.frag").c_str());
    //#endif
    initRenderer(instance);
    instance->edgameRunning = false;
    instance->gameRunning = true;
    glViewport(0, 0, instance->winres.x, instance->winres.y);
    return;
}

void initImGui(GAME *game)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(game->window, game->context);
    ImGui_ImplOpenGL3_Init();
}
