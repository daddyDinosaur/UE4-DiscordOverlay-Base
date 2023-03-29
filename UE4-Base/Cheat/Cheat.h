#pragma once
#include "../Includes.h"
#define M_PI       3.14159265358979323846

//===================================================================================================================================

template<typename t>
t read(uintptr_t addr) {
    if (addr < 0xffffff)
        return t();
    if (addr > 0x7fffffff0000)
        return t();

    return *reinterpret_cast<t*>(addr);
}

template<typename t>
bool write(uintptr_t addr, t buffer) {
    *reinterpret_cast<t*>(addr) = buffer;
    return true;
}

typedef struct _D3DXMATRIX
{
    union
    {
        struct
        {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;
        };
        float m[4][4];
    };
} D3DXMATRIX;

inline D3DXMATRIX Matrix(FRotator Rotation, FVector Origin = FVector(0, 0, 0))
{
    float radPitch = (Rotation.Pitch * M_PI / 180.f);
    float radYaw = (Rotation.Yaw * M_PI / 180.f);
    float radRoll = (Rotation.Roll * M_PI / 180.f);
    float SP = sinf(radPitch);
    float CP = cosf(radPitch);
    float SY = sinf(radYaw);
    float CY = cosf(radYaw);
    float SR = sinf(radRoll);
    float CR = cosf(radRoll);
    D3DXMATRIX matrix;
    matrix.m[0][0] = CP * CY;
    matrix.m[0][1] = CP * SY;
    matrix.m[0][2] = SP;
    matrix.m[0][3] = 0.f;
    matrix.m[1][0] = SR * SP * CY - CR * SY;
    matrix.m[1][1] = SR * SP * SY + CR * CY;
    matrix.m[1][2] = -SR * CP;
    matrix.m[1][3] = 0.f;
    matrix.m[2][0] = -(CR * SP * CY + SR * SY);
    matrix.m[2][1] = CY * SR - CR * SP * SY;
    matrix.m[2][2] = CR * CP;
    matrix.m[2][3] = 0.f;
    matrix.m[3][0] = Origin.X;
    matrix.m[3][1] = Origin.Y;
    matrix.m[3][2] = Origin.Z;
    matrix.m[3][3] = 1.f;
    return matrix;
}

inline bool W2S(FVector WorldLocation, FVector2D& ScreenLocation)
{
    if (WorldLocation == 0.f) return false;
    auto Location = Data.MyCamLocation;//PlayerController->PlayerCameraManager->CameraCache.POV.Rotation;
    auto Rotation = Data.MyCamRotation;//PlayerController->PlayerCameraManager->CameraCache.POV.Rotation;
    D3DXMATRIX tempMatrix = Matrix(Rotation); 
    FVector vAxisX, vAxisY, vAxisZ;
    vAxisX = FVector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
    vAxisY = FVector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
    vAxisZ = FVector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);
    float w = tempMatrix.m[3][0] * WorldLocation.X + tempMatrix.m[3][1] * WorldLocation.Y + tempMatrix.m[3][2] * WorldLocation.Z + tempMatrix.m[3][3];
    if (w < 0.01) return false;
    FVector vDelta = WorldLocation - Location;
    FVector vTransformed = FVector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));
    if (vTransformed.Z < 1.0f) vTransformed.Z = 1.f;
    float fovAngle = Data.DefaultFOV;//PlayerController->PlayerCameraManager->CameraCache.POV.FOV;
    float screenCenterX = Settings.WindowSizeX / 2;
    float screenCenterY = Settings.WindowSizeY / 2;
    ScreenLocation.X = (screenCenterX + vTransformed.X * (screenCenterX / (float)tan(fovAngle * M_PI / 360)) / vTransformed.Z);
    ScreenLocation.Y = (screenCenterY - vTransformed.Y * (screenCenterX / (float)tan(fovAngle * M_PI / 360)) / vTransformed.Z);
    if (ScreenLocation.X < -50 || ScreenLocation.X >(Settings.WindowSizeX + 250)) return false;
    if (ScreenLocation.Y < -50 || ScreenLocation.Y >(Settings.WindowSizeY + 250)) return false;
    return true;
}

inline void HPBar(float HealthPercent, float offset, FVector2D ActorScreenLocation, ImDrawList* windowshit)
{
    float width = 60;
    float height = 6;

    float pixels = HealthPercent * width;
    windowshit->AddRectFilled(ImVec2(ActorScreenLocation.X - (0.5 * width) + 15, ActorScreenLocation.Y + offset), ImVec2(ActorScreenLocation.X + (0.5 * width) + 15, ActorScreenLocation.Y + height + offset),
        ImColor(0, 0, 0, 255), 0.1f);
    ImColor healthbar;
    if (HealthPercent > 0.66) healthbar = ImColor(2, 216, 1, 255);
    if (HealthPercent < 0.66) healthbar = ImColor(255, 206, 10, 255);
    if (HealthPercent < 0.33) healthbar = ImColor(216, 1, 2, 255);

    windowshit->AddRectFilled(ImVec2(ActorScreenLocation.X - (0.5 * width) + 15, ActorScreenLocation.Y + offset), ImVec2(ActorScreenLocation.X - (0.5 * width) + pixels + 15, ActorScreenLocation.Y + height + offset),
        healthbar, 0.5f);
}

inline void AlertText(ImVec2 ScreenPosition, ImColor Color, const char* Text, int WidthText)
{
    if (!Text) return;
    auto ImScreen = *reinterpret_cast<const ImVec2*>(&ScreenPosition);
    if (ScreenPosition.x > 0 && ScreenPosition.y > 0 && Settings.WindowSizeX > ScreenPosition.x && Settings.WindowSizeY > ScreenPosition.y)
    {
        auto Size = ImGui::CalcTextSize(Text);
        ImScreen.x -= Size.x * 0.5f;
        ImScreen.y -= Size.y;
        ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 20, ImScreen, Color, Text, Text + strlen(Text), WidthText);
    }
}

inline int ReturnDistance(int X1, int Y1, int X2, int Y2) { return sqrt(pow(X2 - X1, 2) + pow(Y2 - Y1, 2) * 1); }

inline bool WithinAimFOV(int CircleX, int CircleY, int R, int X, int Y)
{
    int Dist = (X - CircleX) * (X - CircleX) + (Y - CircleY) * (Y - CircleY);
    if (Dist <= R * R) return true;
    else return false;
}

inline void sendinput(WPARAM key)
{
    HWND hwnd = GetForegroundWindow();
    PostMessageW(hwnd, WM_KEYDOWN, key, 0);
    PostMessageW(hwnd, WM_KEYUP, key, 0);
}

__forceinline void RenderESP(FVector2D ScreenPosition, std::vector<std::string> text, ImColor color, float height, ImDrawList* Drawlist) {
    auto start_pos = *reinterpret_cast<const FVector2D*>(&ScreenPosition);
    for (size_t size = 0; size < text.size(); size++)
    {
        ImVec2 textSize;
        auto& DisplayString = text[size];
        if (DisplayString.length() < 1) continue;
        auto CurrentPosition = FVector2D(start_pos.X, start_pos.Y);

        //hud->GetTextSize(FString(s2ws(DisplayString).c_str()), &size_x, &size_y, settings.fonts.Roboto18, 0.8f);
        textSize = ImGui::CalcTextSize(DisplayString.c_str());
        auto position_x = (CurrentPosition.X - textSize.x) * 0.5f;

        //hud->DrawTextA(FString(s2ws(DisplayString).c_str()), color, position_x, start_pos.Y,
        //    settings.fonts.Roboto18, 0.8f, false);
        Drawlist->AddText(ImVec2(position_x, start_pos.Y), color, DisplayString.c_str());
        start_pos.Y += height;
    }
}

//===================================================================================================================================

namespace ZeChair {
    inline void EntityLoop()
    {
        ImDrawList* Renderer = ImGui::GetOverlayDrawList();

        Settings.WindowSizeX = LI_FN(GetSystemMetrics)(SM_CXSCREEN);
        Settings.WindowSizeY = LI_FN(GetSystemMetrics)(SM_CYSCREEN);

        if (Settings.Visuals.Crosshair)
        {
            Renderer->AddLine(ImVec2(Settings.WindowSizeX / 2 - 14, Settings.WindowSizeY / 2), ImVec2(Settings.WindowSizeX / 2 + 1, Settings.WindowSizeY / 2), Settings.Colors.Crosshair, Settings.Visuals.CrosshairThikkness);
            Renderer->AddLine(ImVec2(Settings.WindowSizeX / 2 + 15, Settings.WindowSizeY / 2), ImVec2(Settings.WindowSizeX / 2 + 1, Settings.WindowSizeY / 2), Settings.Colors.Crosshair, Settings.Visuals.CrosshairThikkness);
            Renderer->AddLine(ImVec2(Settings.WindowSizeX / 2, Settings.WindowSizeY / 2 - 14), ImVec2(Settings.WindowSizeX / 2, Settings.WindowSizeY / 2), Settings.Colors.Crosshair, Settings.Visuals.CrosshairThikkness);
            Renderer->AddLine(ImVec2(Settings.WindowSizeX / 2, Settings.WindowSizeY / 2 + 15), ImVec2(Settings.WindowSizeX / 2, Settings.WindowSizeY / 2), Settings.Colors.Crosshair, Settings.Visuals.CrosshairThikkness);
        }

        if (Settings.Visuals.FOVCircle)
        {
            Renderer->AddCircle(ImVec2(Settings.WindowSizeX / 2, Settings.WindowSizeY / 2), Settings.Visuals.FOV, Settings.Colors.FovCircle, Settings.Visuals.FOVCricleSides, Settings.Visuals.FOVCircleThikkness);
        }

        try
        {
            Data.mBaseAddr = (uintptr_t)GetModuleHandle(nullptr);
            if (!Data.mBaseAddr) return;
            do
            {
                Data.gWorld = read<uintptr_t>(Data.mBaseAddr + Settings.EngineOffsets.GWorld);
                if (!Data.gWorld) break;
                auto GameInstance = read<uintptr_t>(Data.gWorld + Offsets.UGameInstance); //UGameInstance
                auto LocalPlayers = read<uintptr_t>(GameInstance + Offsets.ULocalPlayer); //ULocalPlayer
                Data.LocalPlayer = read<uintptr_t>(LocalPlayers); //[0] Local Player
                if (!Data.LocalPlayer) break;
                Data.PlayerController = read<uintptr_t>(Data.LocalPlayer + Offsets.PlayerController);//PlayerController
                if (!Data.PlayerController) break;

                Data.CameraManager = read<uintptr_t>(Data.PlayerController + Offsets.PlayerCamaeraManager/*PlayerCamaeraManager*/);
                if (!Data.CameraManager) break;
                auto CameraCache = read<uintptr_t>(Data.CameraManager + Offsets.CameraCache/*CameraCache*/);
                Data.CameraCachePOV = read<FMinimalViewInfo>(CameraCache + Offsets.POV/*POV*/);

                Data.MyCamLocation = Data.CameraCachePOV.Location;          //PlayerController->PlayerCameraManager->CameraCache.POV.Location;
                Data.MyCamRotation = Data.CameraCachePOV.Rotation;          //PlayerController->PlayerCameraManager->CameraCache.POV.Rotation;
                Data.DefaultFOV = Data.CameraCachePOV.FOV;             //PlayerController->PlayerCameraManager->CameraCache.POV.FOV

                Data.NearbyEnemies = 0;
                std::string ShortName;

                auto Levels = read<TArray<uintptr_t>>(Data.gWorld + Offsets.ULevels); //Levels offset

                for (int i = 0; i < Levels.Count; i++)
                {
                    auto TheLevel = Levels[i];
                    if (!TheLevel) continue;

                    auto Actors = read<TArray<uintptr_t>>(TheLevel + Offsets.AActors); //AActors (Generally 0xA0)
                    if (!Actors.Count) continue;

                    if (Actors.Count < 50) continue;

                    for (int i = 0; i < Actors.Count; i++)
                    {
                        auto Actor = Actors[i];

                        if (!Actor) continue;

                        std::vector<std::string>renderInfo;
                        std::string tempInfoOne;
                        std::string tempInfoTwo;

                        /// <summary>
                        /// To Render ESP, Use TempInfoOne / TempInfoTwo if you need to store multiple pieces of info in one line, renderInfo.pushback() it and then render with  RenderESP
                        /// </summary>
                        
                        if (Actor == read<uintptr_t>(Data.PlayerController + Offsets.AcknowledgedPawn)) //AcknowledgedPawn
                        {
                            Data.LocalActor = Actor;
                            continue;
                        }

                        

                    }
                }

            } while (false);
        }
        catch (std::exception E)
        {
            Logger::Log("[EXECUTION_LOOP]: Execution Loop Failed! [EXCEPTION]: %s\n", E);
        }
    }
}