/*
   This header file provides example data for ImGui components and variables.
   It includes namespaces for various components such as buttons, checkboxes, sliders, color pickers, radio buttons, and more.
   Modify the values and add or remove components as needed for your ImGui application.
*/

#pragma once
#include <array>
#include "../imgui/imgui.h"

namespace button
{
	const char* string = "String";
}

namespace checkbox
{
	static bool enableCheckbox1 = false;
	static bool enableCheckbox2 = false;
	// etc..
}

namespace combobox
{
	static int combobox = 0;
	const char* combobox_items[3] = { "Option 1", "Option 2", "Option 3" };
}

namespace sliderInt
{
	int sliderInt = 50;
}

namespace sliderFloat
{
	float sliderFloat = 50.0f;
}

namespace RGBEdit
{
	//                                R      G      B
	std::array<float, 3> rgbEdit = { 255.f, 255.f, 255.f };
}

namespace RGBAEdit
{
	//                           R    G    B    A
	ImColor rgbaEdit = ImColor{ 255, 255, 255, 255 };
}

namespace RGBPicker
{
	//                                R      G      B
	std::array<float, 3> rgbColor = { 255.f, 255.f, 255.f };
}

namespace RGBAPicker
{
	//                           R    G    B    A
	ImColor rgbaColor = ImColor{ 255, 255, 255, 255 };
}

namespace radioButton
{
	bool radioValue = false;;
}

namespace inputText
{
	char inputTextBuffer[256] = "Type here...";
}

namespace progressBar
{
	float progressValue = 0.6f;
}

namespace selectable
{
	bool selectableValue = false;
}