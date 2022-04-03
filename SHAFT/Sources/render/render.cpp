#include "render/render.h"
#include "logic/math.h"
#include "common.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
void initRenderer(GAME *game)
{
    unsigned int VBO;
    float vertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f};
    game->gl.projection = glm::ortho(0.0f, (float)game->winres.x, (float)game->winres.y, 0.0f, -1.0f, 1.0f);
    glGenVertexArrays(1, &game->gl.quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(game->gl.quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ImGuiBeginRender(GAME *game)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(game->window);
    ImGui::NewFrame();
}
void ImGuiEndRender()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void clearScreen()
{

    glClearColor(0.2f, 0.2f, 0.2f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
}
void renderScene(GAME *game)
{
    for (int i = 0; i < (int)game->cmap.drawOrder.size(); i++)
    {
        switch (game->cmap.drawOrder[i].type)
        {
        case S_DECTILE:
        {
            raw_drawSP(game,  (GLuint*)&game->texm.textures[game->cmap.decorationsTile[game->cmap.drawOrder[i].id].tex].texture.glLoc, 
                                       vec2toGLM(game->cmap.decorationsTile[game->cmap.drawOrder[i].id].pos), 
                                       vec2toGLM(game->cmap.decorationsTile[game->cmap.drawOrder[i].id].scl), 
                                       game->cmap.decorationsTile[game->cmap.drawOrder[i].id].rot, glm::vec3(1));
        }
        }
    }
}
void raw_drawSP(GAME *game, GLuint *texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    // prepare transformations
    
    game->gl.shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    game->gl.shader.SetMatrix4("model", model);
    game->gl.shader.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glBindVertexArray(game->gl.quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}