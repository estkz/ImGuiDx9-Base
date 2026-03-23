#include "app.h"

int App::Run()
{
    if (!Initialize())
        return 1;

    while (m_running)
    {
        if (!m_window.ProcessMessages(m_running))
            break;

        if (!m_menu.IsOpen())
        {
            DestroyWindow(m_window.GetHandle());
            break;
        }

        Frame();
    }

    Shutdown();
    return 0;
}

bool App::Initialize()
{
    if (!m_window.Create("Application Name", 650, 400))
        return false;

    if (!m_renderer.Create(m_window.GetHandle()))
        return false;

    m_renderer.CreateImGui(m_window.GetHandle());
    return true;
}

void App::Shutdown()
{
    m_renderer.DestroyImGui();
    m_renderer.Destroy();
    m_window.Destroy();
}

void App::Frame()
{
    m_renderer.BeginFrame();
    m_menu.Render();
    m_renderer.EndFrame();
}