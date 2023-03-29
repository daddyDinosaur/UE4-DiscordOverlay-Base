#pragma once
#include "../Includes.h"

typedef int (WINAPI* LPFN_MBA)(DWORD);
inline static LPFN_MBA FaggotKeyState;
inline float color_red = 1.;
inline float color_green = 0;
inline float color_blue = 0;
inline float color_random = 0.0;


namespace IRendering {
    inline void ColorChange()
    {
        static float Color[3];
        static DWORD Tickcount = 0;
        static DWORD Tickcheck = 0;
        ImGui::ColorConvertRGBtoHSV(color_red, color_green, color_blue, Color[0], Color[1], Color[2]);
        if (GetTickCount() - Tickcount >= 1)
        {
            if (Tickcheck != Tickcount)
            {
                Color[0] += 0.001f * -10;
                Tickcheck = Tickcount;
            }
            Tickcount = GetTickCount();
        }
        if (Color[0] < 0.0f) Color[0] += 1.0f;
        ImGui::ColorConvertHSVtoRGB(Color[0], Color[1], Color[2], color_red, color_green, color_blue);
    }

    inline void ColorAndStyle() {
        ImGuiStyle* style = &ImGui::GetStyle();

        style->WindowPadding = ImVec2(15, 15);
        style->WindowRounding = 5.0f;
        style->FramePadding = ImVec2(4.5f, 2.5f);
        style->FrameRounding = 0.0f;
        style->ItemSpacing = ImVec2(12, 8);

        style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style->IndentSpacing = 25.0f;


        //Tabs
        style->ItemInnerSpacing = ImVec2(18, 6);
        //style->TabRounding = 0.0f;

        style->ScrollbarSize = 0.0f;
        style->ScrollbarRounding = 0.0f;

        //Sliders
        style->GrabMinSize = 6.0f;
        style->GrabRounding = 0.0f;


        style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
        style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        style->Colors[ImGuiCol_WindowBg] = ImColor(27, 26, 35, 255);
        style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        style->Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);
        style->Colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);
        style->Colors[ImGuiCol_FrameBg] = ImColor(37, 38, 51, 255);
        style->Colors[ImGuiCol_FrameBgHovered] = ImColor(42, 43, 56, 255);
        style->Colors[ImGuiCol_FrameBgActive] = ImColor(37, 38, 51, 255);

        style->Colors[ImGuiCol_TitleBg] = ImColor(27, 26, 35, 255);
        style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(37, 38, 51, 40);
        style->Colors[ImGuiCol_TitleBgActive] = ImColor(37, 38, 51, 255);

        style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);
        style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_Button] = ImColor(79, 79, 81, 255);
        style->Colors[ImGuiCol_ButtonHovered] = ImColor(79, 79, 81, 255);
        style->Colors[ImGuiCol_ButtonActive] = ImColor(84, 84, 86, 255);
        style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    }

    inline void Input()
    {
        ImGuiIO& io = ImGui::GetIO();
        POINT p_mouse_pos;
        GetCursorPos(&p_mouse_pos);
        io.MousePos.x = p_mouse_pos.x;
        io.MousePos.y = p_mouse_pos.y;

        if (FaggotKeyState(VK_LBUTTON)) {
            io.MouseDown[0] = true;
            io.MouseClicked[0] = true;
            io.MouseClickedPos[0].x = io.MousePos.x;
            io.MouseClickedPos[0].y = io.MousePos.y;
        }
        else {
            io.MouseDown[0] = false;
        }
    }

    inline ImGuiWindow& CreateScene() {
        ImGui_ImplDX11_NewFrame();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
        ImGui::Begin(("##mainscenee"), nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);

        auto& io = ImGui::GetIO();
        ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

        return *ImGui::GetCurrentWindow();
    }

    inline void DrawWaterMark(ImGuiWindow& windowshit, std::string str, ImVec2 loc, ImU32 colr, bool centered = false)
    {
        ImVec2 size = { 0,0 };
        float minx = 0;
        float miny = 0;
        if (centered)
        {
            size = ImGui::GetFont()->CalcTextSizeA(windowshit.DrawList->_Data->FontSize, 0x7FFFF, 0, str.c_str());
            minx = size.x / 2.f;
            miny = size.y / 2.f;
        }

        windowshit.DrawList->AddText(ImVec2((loc.x - 1) - minx, (loc.y - 1) - miny), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), str.c_str());
        windowshit.DrawList->AddText(ImVec2((loc.x + 1) - minx, (loc.y + 1) - miny), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), str.c_str());
        windowshit.DrawList->AddText(ImVec2((loc.x + 1) - minx, (loc.y - 1) - miny), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), str.c_str());
        windowshit.DrawList->AddText(ImVec2((loc.x - 1) - minx, (loc.y + 1) - miny), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), str.c_str());
        windowshit.DrawList->AddText(ImVec2(loc.x - minx, loc.y - miny), colr, str.c_str());
    }

    inline void Watermark(ImGuiWindow& window) {
        auto RGB = ImGui::GetColorU32({ color_red, color_green, color_blue, 255 });
        DrawWaterMark(window, "GamingChair.Pro [E-Sex Edition]", ImVec2(5, 5), RGB, false);
    }
}

namespace ImGui
{
    inline IMGUI_API bool Tab(unsigned int index, const char* label, int* selected, float width = 46, float height = 17)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImColor color = ImColor(27, 26, 35, 255)/*style.Colors[ImGuiCol_Button]*/;
        ImColor colorActive = ImColor(79, 79, 81, 255); /*style.Colors[ImGuiCol_ButtonActive]*/;
        ImColor colorHover = ImColor(62, 62, 66, 255)/*style.Colors[ImGuiCol_ButtonHovered]*/;


        if (index > 0)
            ImGui::SameLine();

        if (index == *selected)
        {
            style.Colors[ImGuiCol_Button] = colorActive;
            style.Colors[ImGuiCol_ButtonActive] = colorActive;
            style.Colors[ImGuiCol_ButtonHovered] = colorActive;
        }
        else
        {
            style.Colors[ImGuiCol_Button] = color;
            style.Colors[ImGuiCol_ButtonActive] = colorActive;
            style.Colors[ImGuiCol_ButtonHovered] = colorHover;
        }

        if (ImGui::Button(label, ImVec2(width, height)))
            *selected = index;

        style.Colors[ImGuiCol_Button] = color;
        style.Colors[ImGuiCol_ButtonActive] = colorActive;
        style.Colors[ImGuiCol_ButtonHovered] = colorHover;

        return *selected == index;
    }
}