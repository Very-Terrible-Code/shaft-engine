#include "shaft-engine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "render/render.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include <glad/glad.h>

// Utility function to read file to string
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
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        exit(0);
    }
    // Create SDL window and context
    instance->window = SDL_CreateWindow("SHAFT", 0, 0, width, height, SDL_WINDOW_OPENGL + SDL_WINDOW_RESIZABLE);
    instance->context = SDL_GL_CreateContext(instance->window);
    // Load OpenGL functions
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    // VSync
    SDL_GL_SetSwapInterval(1);
#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, NULL,
                          GL_TRUE);
    glDebugMessageCallback(debugCallback, nullptr);
#endif

    // Anti-aliasing MSAA
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

    // Window resolution
    instance->winres.x = width;
    instance->winres.y = height;
    instance->orgwinres.x = width;
    instance->orgwinres.y = height;

    // Chipmunk2D gravity
    instance->cmap.gravity.x = 0.f;
    instance->cmap.gravity.y = -100.f;

    // General purpose rendering shader
    instance->gl.shader.Compile(readFileIntoString("rsc/shaders/transp.vert").c_str(), readFileIntoString("rsc/shaders/texc.frag").c_str());
    // Frame buffer shader
    instance->gl.screenShader.Compile(readFileIntoString("rsc/shaders/transp.vert").c_str(), readFileIntoString("rsc/shaders/fb.frag").c_str());

    // Init Lua scripting
    initScript(&instance->cmap.globalscr);

    //#ifdef ENABLE_EDITOR
    // Colour shader
    instance->debugBLOCK.Compile(readFileIntoString("rsc/shaders/transp.vert").c_str(), readFileIntoString("rsc/shaders/rawcol.frag").c_str());
    //#endif

    initRenderer(instance);

    // Utility variables
    instance->fps = 1. / 60.;
    instance->edgameRunning = false;
    instance->gameRunning = true;

    // OpenGL viewport
    glViewport(0, 0, instance->winres.x, instance->winres.y);
    return;
}

void initImGui(GAME *game)
{
    // Check version
    IMGUI_CHECKVERSION();

    // Create ImGui
    ImGui::CreateContext();
    // Retrieves variables to display
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    // Style
    // TODO: Look nice SHFT-12
    // https://www.unknowncheats.me/forum/c-and-c-/189635-imgui-style-settings.html
    ImGui::StyleColorsDark();

    // OpenGL ImGui integration initialisation
    ImGui_ImplSDL2_InitForOpenGL(game->window, game->context);
    ImGui_ImplOpenGL3_Init();
}
