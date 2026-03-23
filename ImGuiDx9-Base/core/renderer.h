#pragma once

#include <Windows.h>
#include <d3d9.h>

class Renderer
{
public:
    Renderer() = default;
    ~Renderer() = default;

    bool Create(HWND hwnd);
    void Destroy();

    void CreateImGui(HWND hwnd);
    void DestroyImGui();

    void BeginFrame();
    void EndFrame();

    void ResetDevice(unsigned int width, unsigned int height);

    IDirect3DDevice9* GetDevice() const noexcept { return m_device; }

private:
    IDirect3D9* m_d3d = nullptr;
    IDirect3DDevice9* m_device = nullptr;
    D3DPRESENT_PARAMETERS m_presentParams{};
    HWND m_hwnd = nullptr;
};