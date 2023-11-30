//
//  Application.cpp
//  example_apple_metal_ios
//
//  Created by Dylan Hamuy on 11/26/23.
//  Copyright © 2023 Warren Moore. All rights reserved.
//

#include "Application.h"
#include "imgui.h"
#include <cmath>
#include <iostream>
#include <string>

namespace MyApp
{
    void RenderFPS()
    {
        ImGuiIO& io = ImGui::GetIO();
        float fps = io.Framerate;

        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::SetNextWindowSize(ImVec2(100, 25));
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::Begin("FPS Tracker", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "FPS: %.2f", fps);
        ImGui::End();
    }

    void RenderHealthBar(std::map<std::string, std::string>& playerInfo)
    {
        float health = std::stof(playerInfo["health"]);
        
        int totalHealth = std::stof(playerInfo["totalHealth"]);
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 barSize(25*200/totalHealth, 40);
        ImVec2 barPos(30, io.DisplaySize.y - barSize.y - 30);
        
        std::string healthText = std::to_string(int(totalHealth*health/100.f)) + " | " + std::to_string(totalHealth);
        ImGui::GetForegroundDrawList()->AddText(ImVec2(barPos.x, barPos.y - 20), IM_COL32(255, 255, 255, 255), healthText.c_str());
        
        int numBoxes = (totalHealth / 25);
        float HPbars = (health/100.0f)*numBoxes;
        
        ImU32 bgColor = IM_COL32(50, 50, 50, 50);
        ImU32 hpColor = IM_COL32(255, 255, 255, 255);
        for (int i = 0; i < numBoxes; i ++) {
            ImGui::GetBackgroundDrawList()->AddRectFilled(barPos, ImVec2(barPos.x + barSize.x, barPos.y + barSize.y), bgColor);
            if (HPbars > 0) {
                ImGui::GetBackgroundDrawList()->AddRectFilled(barPos, ImVec2(barPos.x + fmin(HPbars * barSize.x, 1 * barSize.x), barPos.y + barSize.y), hpColor);
                HPbars -= 1;
            }
            
            barPos.x += barSize.x + 2;
        }
    }

    void RenderMiniMap()
    {
                
        ImVec2 minimapSize(150, 150);
        ImVec2 minimapPos(10, 40);

        int numHorizontalLines = 5;
        int numVerticalLines = 5;

        float lineSpacingX = minimapSize.x / (numVerticalLines + 1);
        float lineSpacingY = minimapSize.y / (numHorizontalLines + 1);

        for (int i = 0; i < numHorizontalLines; ++i)
        {
            float y = minimapPos.y + (i + 1) * lineSpacingY;
            ImGui::GetForegroundDrawList()->AddLine(ImVec2(minimapPos.x, y),ImVec2(minimapPos.x + minimapSize.x, y),
                IM_COL32(255, 255, 255, 100)
            );
        }
        for (int i = 0; i < numVerticalLines; ++i)
        {
            float x = minimapPos.x + (i + 1) * lineSpacingX;
            ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, minimapPos.y), ImVec2(x, minimapPos.y + minimapSize.y),
                IM_COL32(255, 255, 255, 100)
            );
        }
        ImGui::GetForegroundDrawList()->AddRect( minimapPos, ImVec2(minimapPos.x + minimapSize.x, minimapPos.y + minimapSize.y),
            IM_COL32(255, 255, 255, 255)
        );


        ImVec2 playerPosOnMinimap = ImVec2(minimapPos.x + minimapSize.x / 2, minimapPos.y + minimapSize.y / 2);
        ImGui::GetForegroundDrawList()->AddCircleFilled(playerPosOnMinimap, 5.0f, IM_COL32(255, 0, 0, 255));

        ImVec2 poiPositions[] = {
            ImVec2(minimapPos.x + minimapSize.x * 0.8f, minimapPos.y + minimapSize.y * 0.3f),
            ImVec2(minimapPos.x + minimapSize.x * 0.5f, minimapPos.y + minimapSize.y * 0.7f),
            ImVec2(minimapPos.x + minimapSize.x * 0.2f, minimapPos.y + minimapSize.y * 0.5f)
        };

        for (const auto& poiPos : poiPositions)
        {
            ImGui::GetForegroundDrawList()->AddRectFilled(poiPos, ImVec2(poiPos.x + 10, poiPos.y + 10), IM_COL32(0, 0, 255, 255));
        }
    }
    
    void RenderLoreMenu(std::map<std::string, std::string>& playerInfo)
    {
        ImVec2 windowSize = ImVec2(700, 500);
        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - io.DisplaySize.x/2 - windowSize.x/2, io.DisplaySize.y - io.DisplaySize.y/2 - windowSize.y/2));
        ImGui::SetNextWindowSize(windowSize);
        ImGui::Begin("Player Lore", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Learn all about %s below.", playerInfo["name"].c_str());
        ImGui::Separator();
        
        if (ImGui::CollapsingHeader("Origin")) {
            ImGui::Separator();
            ImGui::Text("A short summary of %s", playerInfo["name"].c_str());
            ImGui::Separator();
            ImGui::TextWrapped("%s", playerInfo["origin"].c_str());
        }
        
        if (ImGui::CollapsingHeader("Journey")) {
            ImGui::TextWrapped("%s", playerInfo["journey"].c_str());
        }
        
        static float progress = 0.0f;
        static float last;
        if (ImGui::CollapsingHeader("Statistics")) {
            
            float xp = std::stof(playerInfo["XP"]);
            if (last != xp) {
                progress = 0.0f;
            }
            last = xp;
            
            float progressDir = xp;

            if (progress < xp) {
                progress += progressDir * 0.4f * ImGui::GetIO().DeltaTime;
            }
                

            ImGui::Text("Experience Points: ");
            ImGui::SameLine();
            ImGui::ProgressBar(progress / 100.0f);

            ImGui::Text("Player type: ");
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(255, 0, 0, 0.5));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(255, 0, 0, 0.5));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(255, 0, 0, 0.5));
            ImGui::Button(playerInfo["type"].c_str());
            
            ImGui::PopStyleColor(3);
        } else {
            progress = 0.0f;
        }
        
        if (ImGui::CollapsingHeader("Actions")) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);

            if (ImGui::Button("Take Damage", ImVec2(150, 40))) {
                float currentHealth = std::stof(playerInfo["health"]);
                currentHealth -= 20.0f;
                currentHealth = std::max(currentHealth, 0.0f);
                playerInfo["health"] = std::to_string(currentHealth);
            }
            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar();
            
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 1.0f, 0.3f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.6f, 0.1f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);

            if (ImGui::Button("Regenerate", ImVec2(150, 40))) {
                float currentHealth = std::stof(playerInfo["health"]);
                currentHealth += 20.0f;
                currentHealth = std::min(currentHealth, 100.0f);
                playerInfo["health"] = std::to_string(currentHealth);
            }

            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar();   
        }
        
        ImGui::End();
    }
// real clock
// add user select and change player info map accordingly
}
