#include "editor/debug.h"
#include "editor/mapedit.h"

void map_eCOL_IMGUIMENU(GAME *game)
{
    if (game->texm.textures.size())
    {
        static dectile dct;
        dct.scl.x = 100;
        dct.scl.y = 100;
        static float pos[2] = {dct.pos.x, dct.pos.y};
        static float scl[2] = {dct.scl.x, dct.scl.y};
        static int tex = 0;
        static char tag[32];
        ImGui::InputText("Name", (char *)tag, IM_ARRAYSIZE(tag));
        ImGui::InputFloat2("Position", pos, "%.3f units");
        ImGui::InputFloat2("Scale", scl, "%.3f units");
        ImGui::InputFloat("Rotation", &dct.rot, 0.1, 0.15, "%.3f degrees");
        ImGui::InputInt("Texture ID:", (int *)&tex);
        if (tex > (int)game->texm.textures.size() - 1)
        {
            tex--;
        }
        if ((int)tex < 0)
        {
            tex++;
        }
        ImVec2 oldP = ImGui::GetCursorPos();
        ImGui::SameLine();
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImGui::SetCursorPos(ImVec2{ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - 70});
        ImGui::Image((void *)(intptr_t)game->texm.textures[tex].texture.glLoc, ImVec2(100, 100), uv_min, uv_max, tint_col, border_col);
        ImGui::SetCursorPos(oldP);

        if (ImGui::Button("Add"))
        {
            dectile base;
            drawOIT drawo;
            base.pos.x = pos[0];
            base.pos.y = pos[1];
            base.scl.x = scl[0];
            base.scl.y = scl[1];
            base.rot = dct.rot;
            base.attc = -1;
            base.scr.impl = NULL;
            base.tex = (unsigned int)tex;
            memcpy(&base.tag, tag, strlen(tag) + 1);
            game->cmap.decorationsTile.push_back(base);
            drawo.id = (int)game->cmap.decorationsTile.size() - 1;
            drawo.type = S_DECTILE;
            game->cmap.drawOrder.push_back(drawo);
        }
    }
    else
    {
        ImGui::TextColored(ImVec4{1., 0., 0., 1.}, "No Texture Database Loaded!");
    }
}

void map_IMGUIMENU(GAME *game)
{

    static char *items[] = {(char *)"Decoration Tile", (char *)"Collision Tile", (char *)"Collision Box"};
    //static drawOIT selectedItem = {0,0};
    ImGui::Begin("MapEditor");
    
    if (ImGui::BeginTabBar("Map Editor"))
    {

        if (ImGui::BeginTabItem("Add to Scene"))
        {
            static int itemsel;

            ImGui::Combo("Type", &itemsel, items, IM_ARRAYSIZE(items));
            switch (itemsel)
            {
            case 0:
                map_eCOL_IMGUIMENU(game);
                break;
            }
            ImGui::EndTabItem();
        }
        static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        if (ImGui::BeginTabItem("Edit"))
        {
            if (game->cmap.drawOrder.size())
            {
                if (ImGui::BeginTable("edit", 6, flags))
                {
                    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
                    ImGui::TableSetupColumn("Move", ImGuiTableColumnFlags_NoHide);
                    
                    ImGui::TableSetupColumn("Preview", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
                    ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
                    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
                    ImGui::TableSetupColumn("Stats", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
                    ImGui::TableSetupColumn("Info", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
                    ImGui::TableHeadersRow();
                    for (int i = 0; i < (int)game->cmap.drawOrder.size(); i++)
                    {
                        switch (game->cmap.drawOrder[i].type)
                        {
                        case S_DECTILE:
                        {

                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Button("^");
                            ImGui::Button("V");
                            ImGui::TableNextColumn();
                            ImVec2 uv_min = ImVec2(0.0f, 0.0f);
                            ImVec2 uv_max = ImVec2(1.0f, 1.0f);
                            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                            ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                            ImGui::Image((void *)(intptr_t)game->texm.textures[game->cmap.decorationsTile[game->cmap.drawOrder[i].id].tex].texture.glLoc, ImVec2(50, 50), uv_min, uv_max, tint_col, border_col);
                            ImGui::TableNextColumn();
                            ImGui::Text("Draw ID: %i", i);
                            ImGui::Text("RAW ID: %i", game->cmap.drawOrder[i].id);
                            ImGui::TableNextColumn();
                            ImGui::Text("Name: %s", (char*)game->cmap.decorationsTile[game->cmap.drawOrder[i].id].tag);
                            ImGui::TableNextColumn();
                            ImGui::Text("Pos: %fx%f", game->cmap.decorationsTile[game->cmap.drawOrder[i].id].pos.x, game->cmap.decorationsTile[game->cmap.drawOrder[i].id].pos.y);
                            ImGui::Text("Size: %fx%f", game->cmap.decorationsTile[game->cmap.drawOrder[i].id].scl.x, game->cmap.decorationsTile[game->cmap.drawOrder[i].id].scl.y);
                            ImGui::Text("Rotation: %f", game->cmap.decorationsTile[game->cmap.drawOrder[i].id].rot);
                            ImGui::TableNextColumn();
                            if (game->cmap.decorationsTile[game->cmap.drawOrder[i].id].scr.impl == NULL)
                            {
                                ImGui::Text("No script added.");
                            }
                            else
                            {
                                ImGui::Text("Script avalible");
                            }
                            break;
                        }
                        }
                    }
                    ImGui::EndTable();

                }
                
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::Separator();
    ImGui::End();
}
