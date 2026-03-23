#include "gui.h"

void Gui::Render()
{
    if (!m_state.isOpen)
        return;

    RenderMainWindow();
}

void Gui::RenderMainWindow()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(650.0f, 400.0f), ImGuiCond_Always);

    constexpr ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar;

    ImGui::Begin("Application Name", &m_state.isOpen, flags);

    ImGui::TextUnformatted("I am text (scroll for more components)");

    ImGui::Button("String", ImVec2(0.0f, 0.0f));

    ImGui::Checkbox("Enable Checkbox", &m_state.enableCheckbox1);

    static const char* comboItems[] = { "Option 1", "Option 2", "Option 3" };
    ImGui::Combo(
        "Select Item",
        &m_state.comboIndex,
        comboItems,
        IM_ARRAYSIZE(comboItems)
    );

    ImGui::SliderInt(
        "Integer Slider",
        &m_state.sliderIntValue,
        0,
        100,
        "%d",
        ImGuiSliderFlags_NoInput
    );

    ImGui::SliderFloat(
        "Float Slider",
        &m_state.sliderFloatValue,
        0.0f,
        100.0f,
        "%.2f",
        ImGuiSliderFlags_NoInput
    );

    ImGui::ColorEdit3(
        "RGB Color Edit",
        m_state.rgbEdit.data(),
        ImGuiColorEditFlags_NoInputs
    );

    ImGui::ColorEdit4(
        "RGBA Color Edit",
        m_state.rgbaEdit,
        ImGuiColorEditFlags_NoInputs
    );

    ImGui::RadioButton("Radio Button", m_state.radioValue);

    ImGui::InputText(
        "Input Text",
        m_state.inputTextBuffer,
        IM_ARRAYSIZE(m_state.inputTextBuffer)
    );

    ImGui::Dummy(ImVec2(20.0f, 20.0f));

    ImGui::Text("Progress: %.1f%%", m_state.progressValue * 100.0f);
    ImGui::ProgressBar(m_state.progressValue, ImVec2(0.0f, 0.0f));

    ImGui::Dummy(ImVec2(20.0f, 20.0f));

    if (ImGui::Selectable("Select me!", m_state.selectableValue, 0, ImVec2(100.0f, 0.0f)))
    {
        m_state.selectableValue = !m_state.selectableValue;
    }

    ImGui::Dummy(ImVec2(70.0f, 70.0f));

    ImGui::Columns(2, nullptr, false);
    ImGui::ColorPicker3("Pick RGB Color", m_state.rgbPicker.data());
    ImGui::NextColumn();
    ImGui::ColorPicker4("Pick RGBA Color", m_state.rgbaPicker);
    ImGui::Columns(1);

    ImGui::End();
}