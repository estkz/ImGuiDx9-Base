#include "../menu/gui.h"
#include "../menu/components.h"

#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"

#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9tex.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include <string>

int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

int windowX = (screenWidth - gui::WIDTH) / 2;
int windowY = (screenHeight - gui::HEIGHT) / 2;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

long __stdcall WindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
	{
		if (gui::device && wParam != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(lParam);
			gui::presentParameters.BackBufferHeight = HIWORD(lParam);
			gui::ResetDevice();
		}
	} return 0;



	case WM_SYSCOMMAND:
	{
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
	} break;



	case WM_DESTROY:
	{
		PostQuitMessage(0);
	} return 0;



	case WM_LBUTTONDOWN:
	{
		gui::position = MAKEPOINTS(lParam);
	} return 0;



	case WM_MOUSEMOVE: {
		if (wParam == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(lParam);
			auto rect = ::RECT{ };

			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			// Modify this logic if you wish to change the draggable area.
			if (gui::position.x >= 0 &&
				gui::position.x <= gui::WIDTH &&
				gui::position.y >= 0 && gui::position.y <= 19)
				SetWindowPos(
					gui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}

	}return 0;

	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void gui::CreateHWindow(const char* windowName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "NameOfYourCheat";
	windowClass.hIconSm = 0;

	RegisterClassEx(&windowClass);

	window = CreateWindowExA(0, "NameOfYourCheat", windowName, WS_POPUP, windowX, windowY, WIDTH, HEIGHT, 0, 0, windowClass.hInstance, 0);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters, &device) < 0)
		return false;

	return true;
}

void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	auto& colors = style.Colors;

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	/*
	// Menu - Color Theme Config
	colors[ImGuiCol_WindowBg] = ImColor(20, 20, 20); // Frame Backcolor
	colors[ImGuiCol_ChildBg] = ImColor(24, 24, 24); // Childform Backcolor

	// Button
	colors[ImGuiCol_Button] = ImColor(24, 24, 24); // Button Backcolor
	colors[ImGuiCol_ButtonActive] = ImColor(34, 34, 34); // Button Active
	colors[ImGuiCol_ButtonHovered] = ImColor(24, 24, 24); // Hover Color

	// Checkbox
	colors[ImGuiCol_FrameBg] = ImColor(50, 50, 50); // Checkbox Inside Color
	colors[ImGuiCol_FrameBgHovered] = ImColor(50, 50, 50); // Checkbox Inside Hover Color
	colors[ImGuiCol_FrameBgActive] = ImColor(117, 183, 69); // Checked Color
	*/
}

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			isOpened = !isOpened;
			return;
		}
	}

	// Start of the ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	// handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}



/*
	[+] ---------- Menu Customization ---------- [+]

				  • Component Styling
				  • Render Function

	[+] ---------------------------------------- [+]
*/



void gui::Render() noexcept
{
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"YourCheatName",
		&isOpened,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar
	);

	//
	// Examples showcasing various ImGui components and functions:
	//

// Strings
	ImGui::Text("I am text (scroll for more components)");

	// Button: Set x and y to 0 for the default position; adjust size according to preferences
	ImGui::Button(button::string, ImVec2(0, 0));
	// Alternative: ImGui::Button("String2", ImVec2(100, 0));

	// Checkbox
	ImGui::Checkbox("Enable Checkbox", &checkbox::enableCheckbox1);

	// Combobox
	ImGui::Combo("Select Item", &combobox::combobox, combobox::combobox_items, IM_ARRAYSIZE(combobox::combobox_items));

	// Integer and Float sliders
	ImGui::SliderInt("Integer Slider", &sliderInt::sliderInt, 0, 100, "%d", ImGuiSliderFlags_NoInput);
	ImGui::SliderFloat("Float Slider", &sliderFloat::sliderFloat, 0, 100, "%f", ImGuiSliderFlags_NoInput);

	// ColorEdit3 - ImGuiColorEditFlags_NoInputs removes the sliders if preferred
	ImGui::ColorEdit3("RGB Color Edit", reinterpret_cast<float*>(&RGBEdit::rgbEdit), ImGuiColorEditFlags_NoInputs);

	// ColorEdit4 - ImGuiColorEditFlags_NoInputs removes the sliders if preferred
	ImGui::ColorEdit4("RGBA Color Edit", reinterpret_cast<float*>(&RGBAEdit::rgbaEdit), ImGuiColorEditFlags_NoInputs);

	// Radio Button
	ImGui::RadioButton("Radio Button", &radioButton::radioValue);

	// Input Text
	ImGui::InputText("Input Text ", inputText::inputTextBuffer, IM_ARRAYSIZE(inputText::inputTextBuffer));

	// Dummy - Adds an invisible element to the drawing list, used to create spacing or placeholders in your menu.
	ImGui::Dummy(ImVec2(20, 20));

	// Progress Bar
	ImGui::Text("Progress: %.1f%%", progressBar::progressValue * 100);
	ImGui::ProgressBar(progressBar::progressValue, ImVec2(0.0f, 0.0f)); // Use the default size
	ImGui::Dummy(ImVec2(20, 20));

	// Selectable                                                      // Change the size:
	if (ImGui::Selectable("Select me!", selectable::selectableValue, 0, ImVec2(100, 0))) {
		// This code block runs when the item is selected or clicked
		selectable::selectableValue = !selectable::selectableValue; // Toggle the selection state
	}

	ImGui::Dummy(ImVec2(70, 70));

	// ColorPicker3
	ImGui::Columns(2);  // Split the layout into two columns (without this, the sliders may be too large lol)
	ImGui::ColorPicker3("Pick RGB Color", reinterpret_cast<float*>(&RGBPicker::rgbColor));
	// Alternative ColorPicker3 example with custom flags:
	// ImGui::ColorPicker3("Pick RGB Color", RGB::rgbColor.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

	// ColorPicker4
	ImGui::ColorPicker4("Pick RGBA Color", reinterpret_cast<float*>(&RGBAPicker::rgbaColor));

	/*
		Of course, there are numerous additional components beyond these basics. To explore more, you can type 'ImGui::' and browse through the available options.
		Similarly, for flags, e.g., 'ImGuiColorEditFlags_', will provide a list of options specifically for the Color Editor. For a comprehensive guide on everything ImGui has to
		offer, I highly recommend checking out the 'imgui_demo' example. It provides an extensive overview and can be found in the ImGui repository: https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp
	*/


	ImGui::End();
}

