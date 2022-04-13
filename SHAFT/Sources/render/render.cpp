#include "render/render.h"
#include "logic/math.h"
#include "common.h"

void initRenderer(GAME *game)
{

    
    glGenFramebuffers(1, &game->gl.MSFBO);
    glGenFramebuffers(1, &game->gl.FBO);
    glGenRenderbuffers(1, &game->gl.RBO);
    // initialize renderbuffer storage with a multisampled color buffer (don't need a depth/stencil buffer)
    glBindFramebuffer(GL_FRAMEBUFFER, game->gl.MSFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, game->gl.RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, game->winres.x, game->winres.y); // allocate storage for render buffer object
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, game->gl.RBO); // attach MS render buffer object to framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;
    // also initialize the FBO/texture to blit multisampled color-buffer to; used for shader operations (for postprocessing effects)
    glBindFramebuffer(GL_FRAMEBUFFER, game->gl.FBO);
    glGenTextures(1, &game->gl.tecg);
    glBindTexture(GL_TEXTURE_2D, game->gl.tecg);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, game->winres.x, game->winres.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, game->gl.tecg, 0); // attach texture to framebuffer as its color attachment
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    configFrameBuffer(game);
}

void configFrameBuffer(GAME* game)
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

void beginRenderFrameBuffer(GAME* game)
{
    glBindFramebuffer(GL_FRAMEBUFFER, game->gl.MSFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void endRenderFrameBuffer(GAME* game)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, game->gl.MSFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, game->gl.FBO);
    glBlitFramebuffer(0, 0, game->winres.x, game->winres.y, 0, 0, game->winres.x, game->winres.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // binds both READ and WRITE framebuffer to default framebuffer
}

void fRender(GAME* game){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    game->gl.screenShader.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, game->gl.tecg);	
    glBindVertexArray(game->gl.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
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
    for (int i = 0; i < (int)game->cmap.drawOrder.size(); i++)
    {
        switch (game->cmap.drawOrder[i].type)
        {
        case S_DECTILE:
        {
            raw_drawSP(game, (GLuint *)&game->texm.textures[game->cmap.decorationsTile[game->cmap.drawOrder[i].id].tex].texture.glLoc,
                       vec2toGLM(game->cmap.decorationsTile[game->cmap.drawOrder[i].id].pos),
                       vec2toGLM(game->cmap.decorationsTile[game->cmap.drawOrder[i].id].scl),
                       game->cmap.decorationsTile[game->cmap.drawOrder[i].id].rot, glm::vec3(1));
            break;
        }
        case S_COLTILE:
        {
            raw_drawSP(game, (GLuint *)&game->texm.textures[game->cmap.collisionTile[game->cmap.drawOrder[i].id].tex].texture.glLoc,
                       vec2toGLM(game->cmap.collisionTile[game->cmap.drawOrder[i].id].pos),
                       vec2toGLM(game->cmap.collisionTile[game->cmap.drawOrder[i].id].scl),
                       game->cmap.collisionTile[game->cmap.drawOrder[i].id].rot, glm::vec3(1));
            break;
        }
        }
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