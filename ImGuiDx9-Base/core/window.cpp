#include "window.h"
#include "../imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool Window::Create(const char* title, int width, int height)
{
    m_width = width;
    m_height = height;

    m_windowClass = {};
    m_windowClass.cbSize = sizeof(WNDCLASSEXA);
    m_windowClass.style = CS_CLASSDC;
    m_windowClass.lpfnWndProc = WndProc;
    m_windowClass.cbClsExtra = 0;
    m_windowClass.cbWndExtra = 0;
    m_windowClass.hInstance = GetModuleHandleA(nullptr);
    m_windowClass.hIcon = nullptr;
    m_windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    m_windowClass.hbrBackground = nullptr;
    m_windowClass.lpszMenuName = nullptr;
    m_windowClass.lpszClassName = CLASS_NAME;
    m_windowClass.hIconSm = nullptr;

    if (!RegisterClassExA(&m_windowClass))
        return false;

    const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    const int posX = (screenWidth - width) / 2;
    const int posY = (screenHeight - height) / 2;

    m_hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        title,
        WS_POPUP,
        posX, posY,
        width, height,
        nullptr,
        nullptr,
        m_windowClass.hInstance,
        this
    );

    if (!m_hwnd)
        return false;

    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hwnd);
    return true;
}

void Window::Destroy()
{
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }

    if (m_windowClass.hInstance)
        UnregisterClassA(CLASS_NAME, m_windowClass.hInstance);
}

bool Window::ProcessMessages(bool& running)
{
    MSG msg{};
    while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);

        if (msg.message == WM_QUIT)
        {
            running = false;
            return false;
        }
    }
    return true;
}

void Window::HandleDrag(LPARAM lParam, WPARAM wParam)
{
    if (!(wParam & MK_LBUTTON))
    {
        m_isDragging = false;
        return;
    }

    if (!m_isDragging)
        return;

    const POINTS current = MAKEPOINTS(lParam);
    RECT rect{};
    GetWindowRect(m_hwnd, &rect);
    rect.left += current.x - m_dragStart.x;
    rect.top += current.y - m_dragStart.y;

    SetWindowPos(
        m_hwnd,
        nullptr,
        rect.left,
        rect.top,
        0, 0,
        SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW
    );
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return TRUE;

    Window* window = nullptr;

    if (msg == WM_NCCREATE)
    {
        CREATESTRUCTA* createStruct = reinterpret_cast<CREATESTRUCTA*>(lParam);
        window = reinterpret_cast<Window*>(createStruct->lpCreateParams);
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        window->m_hwnd = hwnd;
    }
    else
    {
        window = reinterpret_cast<Window*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
    }

    if (window)
        return window->HandleMessage(msg, wParam, lParam);

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

LRESULT Window::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_LBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        if (pt.y >= 0 && pt.y <= TITLE_BAR_HEIGHT)
        {
            m_isDragging = true;
            m_dragStart = pt;
        }
        return 0;
    }

    case WM_LBUTTONUP:
        m_isDragging = false;
        return 0;

    case WM_MOUSEMOVE:
        HandleDrag(lParam, wParam);
        return 0;
    }

    return DefWindowProcA(m_hwnd, msg, wParam, lParam);
}