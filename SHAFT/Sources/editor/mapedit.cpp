#include "editor/debug.h"
#include "editor/mapedit.h"
#include "game/map.h"
#include "render/render.h"

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
        static char tag[32] = "Hello!";
        ImGui::InputText("Name", (char *)tag, IM_ARRAYSIZE(tag));
        ImGui::InputFloat2("Position", pos, "%.3f units");
        ImGui::InputFloat2("Scale", scl, "%.3f units");
        ImGui::InputFloat("Rotation", &dct.rot, 0.1, 0.15, "%.3f degrees");
        ImGui::InputInt("Texture ID", (int *)&tex);
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
            base.scr.exist = 0;
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

void map_IMGUIDISPLAYDCT(GAME *game, int id, int drid, drawOIT *sel)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::TreeNodeEx((char *)game->cmap.decorationsTile[id].tag, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);

    if (ImGui::Button(DB_imIDGEN((char *)"Delete", drid)))
    {
        removeIDB(game, drid);
    }
    ImGui::SameLine();
    if (ImGui::Button(DB_imIDGEN((char *)"Select", drid)))
    {
        sel->id = id;
        sel->type = S_DECTILE;
    }

    if (sel->id == id)
    {
        switch (sel->type)
        {
        case S_DECTILE:
            ImGui::TextColored(ImVec4{0., 1., 0., 1.}, "Selected!");
            break;
        }
    }

    ImGui::TableNextColumn();
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImGui::Image((void *)(intptr_t)game->texm.textures[game->cmap.decorationsTile[id].tex].texture.glLoc, ImVec2(50, 50), uv_min, uv_max, tint_col, border_col);

    ImGui::TableNextColumn();

    if (ImGui::Button(DB_imIDGEN((char *)"^", drid)))
    {
        if (drid != 0)
        {
            std::swap(game->cmap.drawOrder[drid], game->cmap.drawOrder[drid - 1]);
        }
    }
    if (ImGui::Button(DB_imIDGEN((char *)"V", drid)))
    {
        if (drid != (int)game->cmap.drawOrder.size() - 1)
        {
            std::swap(game->cmap.drawOrder[drid], game->cmap.drawOrder[drid + 1]);
        }
    }
    ImGui::TableNextColumn();
    ImGui::Text("Pos: %fx%f", game->cmap.decorationsTile[id].pos.x, game->cmap.decorationsTile[id].pos.y);
    ImGui::Text("Size: %fx%f", game->cmap.decorationsTile[id].scl.x, game->cmap.decorationsTile[id].scl.y);
    ImGui::Text("Rotation: %f", game->cmap.decorationsTile[id].rot);
    // printf("TEST: %s", DB_imIDGEN((char*)"Edit", drid));fflush(NULL);
    std::string nameds = DB_imIDGEN((char *)"Edit", drid);
    if (ImGui::CollapsingHeader(nameds.c_str()))
    {

        static float *pos[2] = {&game->cmap.decorationsTile[id].pos.x, &game->cmap.decorationsTile[id].pos.y};
        static float *scl[2] = {&game->cmap.decorationsTile[id].scl.x, &game->cmap.decorationsTile[id].scl.y};

        ImGui::InputText("Name", (char *)game->cmap.decorationsTile[id].tag, 32);
        ImGui::InputFloat2("Position", *pos, "%.3f units");
        ImGui::InputFloat2("Scale", *scl, "%.3f units");
        ImGui::InputFloat("Rotation", &game->cmap.decorationsTile[id].rot, 0.1, 0.15, "%.3f degrees");
        ImGui::InputInt("Texture ID", &game->cmap.decorationsTile[id].tex);
    }
    ImGui::TableNextColumn();
    if (game->cmap.decorationsTile[id].scr.exist == 0)
    {
        ImGui::Text("No script added.");
        std::string g = DB_imIDGEN((char *)"Add Script", drid);
        if (ImGui::Button(g.c_str()))
        {
            initScript(&game->cmap.decorationsTile[id].scr);
            game->cmap.decorationsTile[id].scr.exist = 1;
        }
    }
    else
    {
        std::string g = DB_imIDGEN((char *)"Run Script", drid);
        std::string a = DB_imIDGEN((char *)"Remove", drid);
        if (ImGui::Button(g.c_str()))
        {
            runScriptEmb(&game->cmap.decorationsTile[id].scr);
        }
        ImGui::SameLine();
        if (ImGui::Button(a.c_str()))
        {
            game->cmap.decorationsTile[id].scr.impl = NULL;
            game->cmap.decorationsTile[id].scr.exist = 0;
        }
        ImGui::InputTextMultiline("Edit", (char*)game->cmap.decorationsTile[id].scr.script, IM_ARRAYSIZE(game->cmap.decorationsTile[id].scr.script));
    }
}

void map_dragOBJ(GAME *game, drawOIT *item, bool axis)
{
    axis = axis;
    static bool held = false;
    static int brad = 0;
    if (!ImGui::IsKeyDown(ImGuiKey_LeftShift))
    {
        if (game->cmap.drawOrder.size())
        {
            switch (item->type)
            {
            case S_DECTILE:
            {
                vec2 midpoint = game->cmap.decorationsTile[item->id].pos;
                midpoint.x = midpoint.x + (game->cmap.decorationsTile[item->id].scl.x / 2.);
                midpoint.y = midpoint.y + (game->cmap.decorationsTile[item->id].scl.y / 2.);
                raw_drawBP(game, vec2toGLM(midpoint), glm::vec2(10., 10.), 0., glm::vec3(1., 1., 1.));
                raw_drawBP(game, glm::vec2(midpoint.x, midpoint.y - 20), glm::vec2(10., 10.), 0., glm::vec3(1., 0., 0.));
                raw_drawBP(game, glm::vec2(midpoint.x - 20, midpoint.y), glm::vec2(10., 10.), 0., glm::vec3(0., 1., 0.));

                // HELL TIME
                vec2i mouse;
                SDL_GetMouseState(&mouse.x, &mouse.y);

                raw_drawBP(game, vec2itoGLM(mouse), glm::vec2(8., 8.), 0., glm::vec3(1., 1., 1.));
                SDL_Rect phf = {mouse.x, mouse.y, 8, 8};
                SDL_Rect sax = {(int)midpoint.x, (int)midpoint.y, 10, 10};
                if (!held)
                {
                    if (SDL_HasIntersection(&sax, &phf))
                    {

                        if (ImGui::IsMouseDown(0))
                        {
                            brad = 0;
                            held = true;
                        }
                    }

                    sax = {(int)midpoint.x, (int)midpoint.y - 20, 10, 10};
                    if (SDL_HasIntersection(&sax, &phf))
                    {

                        if (ImGui::IsMouseDown(0))
                        {
                            brad = 1;
                            held = true;
                        }
                    }

                    sax = {(int)midpoint.x - 20, (int)midpoint.y, 10, 10};
                    if (SDL_HasIntersection(&sax, &phf))
                    {

                        if (ImGui::IsMouseDown(0))
                        {
                            brad = 2;
                            held = true;
                        }
                    }
                }
                else
                {
                    vec2 br = vec2itoVec2(mouse);
                    if (!axis)
                    {
                        switch (brad)
                        {
                        case 0:

                            br.x -= (game->cmap.decorationsTile[item->id].scl.x / 2.);
                            br.y -= (game->cmap.decorationsTile[item->id].scl.y / 2.);
                            game->cmap.decorationsTile[item->id].pos = br;
                            break;
                        case 1:
                            br.y -= (game->cmap.decorationsTile[item->id].scl.y / 2.);
                            br.y += 20;
                            br.x = game->cmap.decorationsTile[item->id].pos.x;
                            game->cmap.decorationsTile[item->id].pos = br;
                            break;
                        case 2:
                            br.x -= (game->cmap.decorationsTile[item->id].scl.x / 2.);
                            br.x += 20;
                            br.y = game->cmap.decorationsTile[item->id].pos.y;
                            game->cmap.decorationsTile[item->id].pos = br;
                            break;
                        }
                    }
                    else
                    {
                        switch (brad)
                        {
                        case 1:
                            sax = {(int)game->cmap.decorationsTile[item->id].pos.x, (int)game->cmap.decorationsTile[item->id].pos.y + (int)game->cmap.decorationsTile[item->id].scl.y, (int)game->cmap.decorationsTile[item->id].scl.x, (int)game->cmap.decorationsTile[item->id].scl.y};
                            if (SDL_HasIntersection(&sax, &phf))
                            {
                                game->cmap.decorationsTile[item->id].pos.y += game->cmap.decorationsTile[item->id].scl.y;
                            }
                            sax = {(int)game->cmap.decorationsTile[item->id].pos.x, (int)game->cmap.decorationsTile[item->id].pos.y - (int)game->cmap.decorationsTile[item->id].scl.y, (int)game->cmap.decorationsTile[item->id].scl.x, (int)game->cmap.decorationsTile[item->id].scl.y};
                            if (SDL_HasIntersection(&sax, &phf))
                            {
                                game->cmap.decorationsTile[item->id].pos.y -= game->cmap.decorationsTile[item->id].scl.y;
                            }
                            break;
                        case 2:
                            sax = {(int)game->cmap.decorationsTile[item->id].pos.x + (int)game->cmap.decorationsTile[item->id].scl.x, (int)game->cmap.decorationsTile[item->id].pos.y, (int)game->cmap.decorationsTile[item->id].scl.x, (int)game->cmap.decorationsTile[item->id].scl.y};
                            if (SDL_HasIntersection(&sax, &phf))
                            {
                                game->cmap.decorationsTile[item->id].pos.x += game->cmap.decorationsTile[item->id].scl.x;
                            }
                            sax = {(int)game->cmap.decorationsTile[item->id].pos.x - (int)game->cmap.decorationsTile[item->id].scl.x, (int)game->cmap.decorationsTile[item->id].pos.y, (int)game->cmap.decorationsTile[item->id].scl.x, (int)game->cmap.decorationsTile[item->id].scl.y};
                            if (SDL_HasIntersection(&sax, &phf))
                            {
                                game->cmap.decorationsTile[item->id].pos.x -= game->cmap.decorationsTile[item->id].scl.x;
                            }
                            break;
                        }
                    }
                    if (ImGui::IsMouseReleased(0))
                    {
                        held = false;
                    }
                }

                break;
            }
            }
        }
    }
}

void map_IMGUIMENU(GAME *game)
{

    static char *items[] = {(char *)"Decoration Tile", (char *)"Collision Tile", (char *)"Collision Box"};
    static int selEDMOD = S_DECTILE;
    static bool axise = 1;
    static drawOIT selectedItem = {0, 0};
    map_dragOBJ(game, &selectedItem, axise);
    if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
    {

        if (ImGui::IsMouseClicked(0))
        {
            if (!ImGui::IsAnyItemFocused())
            {
                switch (selEDMOD)
                {
                case S_DECTILE:
                {
                    for (int i = 0; i < (int)game->cmap.decorationsTile.size(); i++)
                    {
                        vec2i mouse;
                        SDL_GetMouseState(&mouse.x, &mouse.y);
                        SDL_Rect sax = {(int)game->cmap.decorationsTile[i].pos.x, (int)game->cmap.decorationsTile[i].pos.y, (int)game->cmap.decorationsTile[i].scl.x, (int)game->cmap.decorationsTile[i].scl.y};
                        SDL_Rect phf = {mouse.x, mouse.y, 10, 10};
                        if (SDL_HasIntersection(&sax, &phf))
                        {
                            selectedItem = {S_DECTILE, i};
                        }
                    }
                    break;
                }
                case S_COLTILE:
                {
                    for (int i = 0; i < (int)game->cmap.collisionTile.size(); i++)
                    {
                        vec2i mouse;
                        SDL_GetMouseState(&mouse.x, &mouse.y);
                        SDL_Rect sax = {(int)game->cmap.collisionTile[i].pos.x, (int)game->cmap.collisionTile[i].pos.y, (int)game->cmap.collisionTile[i].scl.x, (int)game->cmap.collisionTile[i].scl.y};
                        SDL_Rect phf = {mouse.x, mouse.y, 10, 10};
                        if (SDL_HasIntersection(&sax, &phf))
                        {
                            selectedItem = {S_COLTILE, i};
                        }
                    }
                    break;
                }
                case S_COL:
                {
                    for (int i = 0; i < (int)game->cmap.collision.size(); i++)
                    {
                        vec2i mouse;
                        SDL_GetMouseState(&mouse.x, &mouse.y);
                        SDL_Rect sax = {(int)game->cmap.collision[i].pos.x, (int)game->cmap.collision[i].pos.y, (int)game->cmap.collision[i].scl.x, (int)game->cmap.collision[i].scl.y};
                        SDL_Rect phf = {mouse.x, mouse.y, 10, 10};
                        if (SDL_HasIntersection(&sax, &phf))
                        {
                            selectedItem = {S_COL, i};
                        }
                    }
                    break;
                }
                }
            }
        };
    }

    ImGui::Begin("MapEditor");
    ImGui::Text("Currently loaded TextureDB: %s", (char *)game->texm.sourcefile);
    ImGui::Text("Selected Object - %i, %i", selectedItem.type, selectedItem.id);
    ImGui::Checkbox("Enable Grid Snap", &axise);
    ImGui::Combo("Select type", &selEDMOD, items, IM_ARRAYSIZE(items));
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
                if (ImGui::BeginTable("edit", 5, flags))
                {
                    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
                    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
                    ImGui::TableSetupColumn("Preview", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
                    ImGui::TableSetupColumn("Move", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
                    ImGui::TableSetupColumn("Stats", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
                    ImGui::TableSetupColumn("Info", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
                    ImGui::TableHeadersRow();
                    for (int i = 0; i < (int)game->cmap.drawOrder.size(); i++)
                    {
                        switch (game->cmap.drawOrder[i].type)
                        {
                        case S_DECTILE:
                        {
                            map_IMGUIDISPLAYDCT(game, game->cmap.drawOrder[i].id, i, &selectedItem);
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
    static char tempolocsave[64] = "";
    ImGui::InputTextWithHint(" ", "rsc/level1.map", tempolocsave, IM_ARRAYSIZE(tempolocsave));
    ImGui::SameLine();
    if (ImGui::Button("Save DB"))
    {
        saveMAP(tempolocsave, game);
    }
    ImGui::SameLine();
    if (ImGui::Button("Load DB"))
    {
        loadMAP(tempolocsave, game);
    }

    ImGui::End();
}

bool overlap(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    return ((x1 >= x2 && x1 <= x2 + w2 - 1 && y1 >= y2 && y1 <= y2 + h2 - 1) ||
            (x1 + w1 - 1 >= x2 && x1 + w1 - 1 <= x2 + w2 - 1 && y1 >= y2 && y1 <= y2 + h2 - 1) ||
            (x1 >= x2 && x1 <= x2 + w2 - 1 && y1 + h1 - 1 >= y2 && y1 + h1 - 1 <= y2 + h2 - 1) ||
            (x1 + w1 - 1 >= x2 && x1 + w1 - 1 <= x2 + w2 - 1 && y1 + h1 - 1 >= y2 && y1 + h1 - 1 <= y2 + h2 - 1));
}
