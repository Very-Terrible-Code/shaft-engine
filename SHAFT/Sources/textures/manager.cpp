#include "textures/manager.h"
#include <fstream>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

void loadTextureDB(char *name, GAME *game)
{
    std::ifstream is(name, std::ifstream::binary);
    if (is)
    {
        game->texm.loadTexCheck = true;
        clearTEX(game);
        game->texm.textures.clear();

        texdbheader fh;

        is.read(reinterpret_cast<char *>(&fh), sizeof(texdbheader));

        game->texm.textures.resize(fh.texsize);

        is.read(reinterpret_cast<char *>(game->texm.textures.data()), fh.texsize * sizeof(tagtex));

        is.close();

        for (int i = 0; i < (int)game->texm.textures.size(); i++)
        {
            loadTextureFromDB(&game->texm.textures[i].texture);
        }

        game->texm.loadTexCheck = false;
    }
    else
    {
        is.close();
    }
}

void saveTextureDB(char *name, GAME *game)
{
    std::ofstream savedata;
    savedata.open(name, std::ofstream::binary);
    if (savedata)
    {
        texdbheader fh;
        fh.texsize = (int)game->texm.textures.size();

        savedata.write(reinterpret_cast<char *>(&fh),
                       sizeof(texdbheader));
        savedata.write(reinterpret_cast<char *>(&game->texm.textures[0]),
                       sizeof(tagtex) * game->texm.textures.size());
        savedata.close();
    }
    else
    {
        savedata.close();
    }
}

void clearTEX(GAME *texdata)
{
    for (int i = 0; i < (int)texdata->texm.textures.size(); i++)
    {
        glDeleteTextures(1, &texdata->texm.textures[i].texture.glLoc);
    }
}
void removeTEX(int item, GAME *texdata)
{
    glDeleteTextures(1, &texdata->texm.textures[item].texture.glLoc);
    texdata->texm.textures.erase(texdata->texm.textures.begin() + item);
}

void tex_IMGUIMENU(GAME *game)
{
    ImGui::Begin("Texture Database Manager");

    static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
    if (!game->texm.loadTexCheck)
    {
        if (ImGui::BeginTable("3ways", 4, flags))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
            const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
            ImGui::TableSetupColumn("Location", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
            ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
            ImGui::TableSetupColumn("Preview", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
            ImGui::TableHeadersRow();
            for (int i = 0; i < (int)game->texm.textures.size(); i++)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TreeNodeEx((char *)game->texm.textures[i].tag, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
                if (ImGui::BeginPopupContextItem())
                {
                    if (ImGui::Button("delete"))
                    {
                        removeTEX(i, game);
                    }
                    ImGui::EndPopup();
                }
                ImGui::TableNextColumn();
                ImGui::Text((char *)game->texm.textures[i].texture.location);
                ImGui::NewLine();
                ImGui::Text("OpenGL Loaction:%i", game->texm.textures[i].texture.glLoc);
                ImGui::TableNextColumn();
                ImGui::Text("%dx%d", game->texm.textures[i].texture.size.x, game->texm.textures[i].texture.size.y);
                ImGui::TableNextColumn();

                ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 
                ImVec2 uv_max = ImVec2(1.0f, 1.0f);                
                ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  
                ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); 
                ImGui::Image((void *)(intptr_t)game->texm.textures[i].texture.glLoc, ImVec2(100, 100), uv_min, uv_max, tint_col, border_col);
            }

            ImGui::EndTable();
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    static char tempoloc[64] = "";
    static char tempolocsave[64] = "";
    static char temponame[64] = "";
    static bool flip = 0;
    static bool defsavel = 0;

    ImGui::InputTextWithHint("Texure Name", "Wooden Block", temponame, IM_ARRAYSIZE(temponame));
    ImGui::InputTextWithHint("Texure Location", "rsc/image.png", tempoloc, IM_ARRAYSIZE(tempoloc));
    ImGui::SameLine();
    if (ImGui::Button("Load"))
    {
        tex btexture = loadTexture((char *)tempoloc, flip);
        if (btexture.loaded)
        {
            tagtex texture;
            texture.texture = btexture;
            // texture.tag = (char *)malloc((strlen(temponame) + 1) * sizeof(char));
            memcpy(texture.tag, temponame, strlen(temponame) + 1);
            game->texm.textures.push_back(texture);
        }
    }
    ImGui::Checkbox("Flip Texture on Load?", &flip);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    switch (defsavel)
    {
    case 1:
        ImGui::InputTextWithHint(" ", "rsc/textures.tdb", tempolocsave, IM_ARRAYSIZE(tempolocsave));
        ImGui::SameLine();
        if (ImGui::Button("Save DB"))
        {
            saveTextureDB(tempolocsave, game);
        }
        ImGui::SameLine();
        if (ImGui::Button("Load DB"))
        {
            loadTextureDB(tempolocsave, game);
        }
        break;
    case 0:
        if (ImGui::Button("Save DB"))
        {
            saveTextureDB((char *)"rsc/textures.tdb", game);
        }
        ImGui::SameLine();
        if (ImGui::Button("Load DB"))
        {
            loadTextureDB((char *)"rsc/textures.tdb", game);
        }
        break;
    }
    ImGui::Checkbox("Use Custom file location", &defsavel);

    ImGui::End();
}

