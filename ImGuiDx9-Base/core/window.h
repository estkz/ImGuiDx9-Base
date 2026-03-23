#pragma once
#include <Windows.h>

class Window
{
public:
    Window() = default;
    ~Window() = default;

    bool Create(const char* title, int width, int height);
    void Destroy();
    bool ProcessMessages(bool& running);

    HWND GetHandle() const noexcept { return m_hwnd; }

    void HandleDrag(LPARAM lParam, WPARAM wParam);

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hwnd = nullptr;
    WNDCLASSEXA m_windowClass{};
    int m_width = 0;
    int m_height = 0;
    bool m_isDragging = false;
    POINTS m_dragStart{ 0, 0 };

    static constexpr const char* CLASS_NAME = "RefactoredWindowClass";
    static constexpr int TITLE_BAR_HEIGHT = 19;
};