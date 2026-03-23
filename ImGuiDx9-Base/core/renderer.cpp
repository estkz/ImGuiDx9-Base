#include "renderer.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

#pragma comment(lib, "d3d9.lib")

bool Renderer::Create(HWND hwnd)
{
    m_hwnd = hwnd;

    m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!m_d3d)
        return false;

    ZeroMemory(&m_presentParams, sizeof(m_presentParams));
    m_presentParams.Windowed = TRUE;
    m_presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_presentParams.BackBufferFormat = D3DFMT_UNKNOWN;
    m_presentParams.EnableAutoDepthStencil = TRUE;
    m_presentParams.AutoDepthStencilFormat = D3DFMT_D16;
    m_presentParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (m_d3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hwnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &m_presentParams,
        &m_device) < 0)
    {
        return false;
    }

    return true;
}

void Renderer::Destroy()
{
    if (m_device)
    {
        m_device->Release();
        m_device = nullptr;
    }

    if (m_d3d)
    {
        m_d3d->Release();
        m_d3d = nullptr;
    }
}

void Renderer::CreateImGui(HWND hwnd)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(m_device);
}

void Renderer::DestroyImGui()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void Renderer::BeginFrame()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void Renderer::EndFrame()
{
    ImGui::EndFrame();

    m_device->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    m_device->Clear(
        0,
        nullptr,
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_RGBA(0, 0, 0, 255),
        1.0f,
        0
    );

    if (m_device->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        m_device->EndScene();
    }

    const HRESULT result = m_device->Present(nullptr, nullptr, nullptr, nullptr);

    if (result == D3DERR_DEVICELOST &&
        m_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
    {
        RECT rect{};
        GetClientRect(m_hwnd, &rect);
        ResetDevice(
            static_cast<unsigned int>(rect.right - rect.left),
            static_cast<unsigned int>(rect.bottom - rect.top)
        );
    }
}

void Renderer::ResetDevice(unsigned int width, unsigned int height)
{
    if (!m_device)
        return;

    ImGui_ImplDX9_InvalidateDeviceObjects();

    m_presentParams.BackBufferWidth = width;
    m_presentParams.BackBufferHeight = height;

    const HRESULT result = m_device->Reset(&m_presentParams);
    if (result == D3DERR_INVALIDCALL)
        IM_ASSERT(0);

    ImGui_ImplDX9_CreateDeviceObjects();
}