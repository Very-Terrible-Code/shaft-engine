#include "render/render.h"
#include "logic/math.h"
#include "common.h"

void initRenderer(GAME *game)
{

    glGenFramebuffers(1, &game->gl.MSFBO);
    glGenFramebuffers(1, &game->gl.FBO);
    glGenRenderbuffers(1, &game->gl.RBO);
    glBindFramebuffer(GL_FRAMEBUFFER, game->gl.MSFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, game->gl.RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, game->winres.x, game->winres.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, game->gl.RBO);
    glBindFramebuffer(GL_FRAMEBUFFER, game->gl.FBO);
    glGenTextures(1, &game->gl.tecg);
    glBindTexture(GL_TEXTURE_2D, game->gl.tecg);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, game->winres.x, game->winres.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, game->gl.tecg, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    configFrameBuffer(game);
}

void configFrameBuffer(GAME *game)
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

void beginRenderFrameBuffer(GAME *game)
{
    glBindFramebuffer(GL_FRAMEBUFFER, game->gl.MSFBO);
    glClearColor(0.192f, 0.2f, 0.208f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void endRenderFrameBuffer(GAME *game)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, game->gl.MSFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, game->gl.FBO);
    glBlitFramebuffer(0, 0, game->winres.x, game->winres.y, 0, 0, game->winres.x, game->winres.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void fRender(GAME *game)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    game->gl.screenShader.Use();
    raw_drawTX(game, &game->gl.tecg, vec2itoGLM(game->orgwinres));
}

void clearScreen()
{

    glClearColor(0.2f, 0.2f, 0.2f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
}
void renderScene(GAME *game)
{
    game->gl.shader.Use();
    game->gl.shader.SetMatrix4("projection", game->gl.projection);

    beginRenderFrameBuffer(game);
    for (int i = 0; i < (int)game->cmap.tiles.size(); i++)
    {

            raw_drawSP(game, (GLuint *)&game->texm.textures[game->cmap.tiles[i].tex].texture.glLoc,
                       vec2toGLM(game->cmap.tiles[i].pos),
                       vec2toGLM(game->cmap.tiles[i].scl),
                       game->cmap.tiles[i].rot, glm::vec3(1));

    }
    if (game->edgameRunning)
    {
        raw_drawBP(game, vec2toGLM(game->mplay.pos), glm::vec2(10, 40), 0., glm::vec3(0.15));
    }
    endRenderFrameBuffer(game);
    fRender(game);
}
void raw_drawSP(GAME *game, GLuint *texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{

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
void raw_drawTX(GAME *game, GLuint *texture, glm::vec2 size)
{
    game->gl.screenShader.SetMatrix4("projection", game->gl.projection);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.f, 0.f, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    game->gl.screenShader.SetMatrix4("model", model);
    game->gl.screenShader.SetVector2f("res", vec2itoGLM(game->winres));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glBindVertexArray(game->gl.quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
//#ifdef ENABLE_EDITOR
void raw_drawBP(GAME *game, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{

    game->debugBLOCK.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    game->debugBLOCK.SetMatrix4("projection", game->gl.projection);
    game->debugBLOCK.SetMatrix4("model", model);
    game->debugBLOCK.SetVector3f("icolor", color);

    glBindVertexArray(game->gl.quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    game->gl.shader.Use();
}
//#endif