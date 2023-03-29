#include "../Includes.h"
#include "Cheat.h"

Hooks::tProcessEvent oProcessEvent;

//=====================================================================================================================================================================================

// Rendering Helpers

typedef HRESULT(__stdcall* Td3D11Present)(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags);
Td3D11Present              OPresent = nullptr;

ID3D11Device* pD11Device = nullptr;
ID3D11DeviceContext* pD11DeviceContext = nullptr;
ID3D11RenderTargetView* pD11RenderTargetView = nullptr;

void    StartHooking();
HRESULT HookedPresentScene(IDXGISwapChain* dxSwapChain, UINT syncInterval, UINT flags);

//=====================================================================================================================================================================================

// Misc

void* Hooks::FindFunction(const char* FunctionName)
{
    return (void*)DetourFindFunction("ShooterGame.exe", FunctionName);
}

void Hooks::ProcessEvent(uintptr_t pObject, uintptr_t pFunction, void* params)
{
    if (FaggotKeyState(Settings.Keybinds.MenuKey) & 1)
    {
        Settings.ShowMenu = !Settings.ShowMenu;
    }

    oProcessEvent(pObject, pFunction, params);
}

//=====================================================================================================================================================================================

// Rendering 

HRESULT HookedPresentScene(IDXGISwapChain* dxSwapChain, const UINT syncInterval, UINT flags)
{
    if (!pD11Device || !pD11DeviceContext)
    {
        ImGui::CreateContext();

        if (SUCCEEDED(dxSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pD11Device)))
        {
            dxSwapChain->GetDevice(__uuidof(pD11Device), (void**)&pD11Device);
            pD11Device->GetImmediateContext(&pD11DeviceContext);
        }

        DXGI_SWAP_CHAIN_DESC desc;
        dxSwapChain->GetDesc(&desc);

        ImGui_ImplWin32_Init(desc.OutputWindow);
        ImGui_ImplDX11_Init(pD11Device, pD11DeviceContext);
    }
    else
    {
        ID3D11Texture2D* renderTargetTexture = nullptr;
        if (!pD11RenderTargetView)
        {
            if (SUCCEEDED(dxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&renderTargetTexture))))
            {
                pD11Device->CreateRenderTargetView(renderTargetTexture, nullptr, &pD11RenderTargetView);
                renderTargetTexture->Release();
            }
        }
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();
    auto& window = IRendering::CreateScene();

    //============================================== Render Here ============================================================================================================\

    if (Settings.Visuals.RGB) { IRendering::ColorChange(); }

    IRendering::Watermark(window);

    ZeChair::EntityLoop();

    //============================================== Stop Rendering =========================================================================================================


    //============================================== Menu Here ============================================================================================================

    if (Settings.ShowMenu) {
        IRendering::Input();
        renderIMenu(window);
    }

    //============================================== End Menu =========================================================================================================

    pD11DeviceContext->OMSetRenderTargets(1, &pD11RenderTargetView, nullptr);
    ImGui::Render();

    if (pD11RenderTargetView)
    {
        pD11RenderTargetView->Release();
        pD11RenderTargetView = nullptr;
    }

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return OPresent(dxSwapChain, syncInterval, flags);
}

//=====================================================================================================================================================================================

//Startup

void Hooks::InitializeAll()
{
    Logger::Init();
    Logger::Log("Logger Initialized\n");
    MH_Initialize();
    Logger::Log("Minhook Initialized\n");


    FName::GNames = *reinterpret_cast<decltype(FName::GNames)*>(Data.mBaseAddr + Settings.EngineOffsets.Names);
}

void Hooks::StartUp()
{
    Hooks::InitializeAll();

    //========================================================================================

    Settings.WindowSizeX = LI_FN(GetSystemMetrics)(SM_CXSCREEN);
    Settings.WindowSizeY = LI_FN(GetSystemMetrics)(SM_CYSCREEN);

    Data.mBaseAddr = (uintptr_t)GetModuleHandle(nullptr);

    //========================================================================================

    MH_CreateHook(reinterpret_cast<void*>(FindFunction("UObject::ProcessEvent")), &ProcessEvent, reinterpret_cast<PVOID*>(&oProcessEvent));

    auto presentSceneAdress = mem::PatternScan((uintptr_t)LI_FN(GetModuleHandleA)("DiscordHook64.dll"), "56 57 53 48 83 EC 30 44 89 C6");
    DISCORD.HookFunction(presentSceneAdress, (uintptr_t)HookedPresentScene, (uintptr_t)&OPresent);
    FaggotKeyState = (LPFN_MBA)LI_FN(GetProcAddress)(LI_FN(GetModuleHandleA)(("win32u.dll")), ("NtUserGetAsyncKeyState"));

    //========================================================================================

    MH_EnableHook(MH_ALL_HOOKS);

    Beep(1000, 500);
}