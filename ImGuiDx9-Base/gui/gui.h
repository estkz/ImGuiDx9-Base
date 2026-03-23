#pragma once
#include <array>
#include "../imgui/imgui.h"

struct GuiState
{
    bool isOpen = true;
    bool enableCheckbox1 = false;
    int  comboIndex = 0;
    int  sliderIntValue = 50;
    float sliderFloatValue = 50.0f;

    std::array<float, 3> rgbEdit = { 1.0f, 1.0f, 1.0f };
    float rgbaEdit[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    std::array<float, 3> rgbPicker = { 1.0f, 1.0f, 1.0f };
    float rgbaPicker[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    bool radioValue = false;
    char inputTextBuffer[256] = "Type here...";
    float progressValue = 0.6f;
    bool selectableValue = false;
};

class Gui
{
public:
    void Render();
    bool IsOpen() const noexcept { return m_state.isOpen; }
    GuiState& GetState() noexcept { return m_state; }

private:
    void RenderMainWindow();

private:
    GuiState m_state;
};