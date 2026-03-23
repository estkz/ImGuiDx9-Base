#pragma once

#include "core/window.h"
#include "core/renderer.h"
#include "gui/gui.h"

class App
{
public:
    int Run();

private:
    bool Initialize();
    void Shutdown();
    void Frame();

private:
    Window m_window;
    Renderer m_renderer;
    Gui m_menu;
    bool m_running = true;
};