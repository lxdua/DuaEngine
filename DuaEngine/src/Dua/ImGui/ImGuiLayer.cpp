#include "duapch.h"
#include "ImGuiLayer.h"

#include "Dua/Core/Application.h"

#include "ImGui/backends/imgui_impl_glfw.h"
#include "ImGui/backends/imgui_impl_opengl3.h"

#include "Platform/Windows/WindowsWindow.h"

#include "GLFW/glfw3.h"

namespace Dua {

	void SetDarkThemeColors() 
	{
		auto& colors = ImGui::GetStyle().Colors;

		// 基础设置
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// 标题栏
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.18f, 0.18f, 0.18f, 1.0f };

		// 选项卡
		// 基础状态
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };      // 默认未选中状态
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };      // 悬停状态
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };      // 激活状态（鼠标按下）

		// 选中状态（需要与Active区分）
		colors[ImGuiCol_TabSelected] = ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f };        // 选中背景
		colors[ImGuiCol_TabSelectedOverline] = ImVec4{ 0.8f, 0.8f, 0.8f, 1.0f };           // 选中时顶部横线

		// 非聚焦状态（窗口失去焦点时）
		colors[ImGuiCol_TabDimmed] = ImVec4{ 0.12f, 0.12f, 0.12f, 1.0f };        // 非聚焦未选中
		colors[ImGuiCol_TabDimmedSelected] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };           // 非聚焦但选中
		colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4{ 0.6f, 0.6f, 0.6f, 1.0f };       // 非聚焦选中横线

		// 菜单栏
		colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// 边框
		colors[ImGuiCol_Border] = ImVec4{ 0.15f, 0.1505f, 0.151f, 0.88f };
		colors[ImGuiCol_BorderShadow] = ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };

		// 框架背景（输入框、下拉列表等）
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// 按钮
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// 标题（CollapsingHeader等）
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// 滚动条
		colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{ 0.4f, 0.405f, 0.41f, 1.0f };

		// 滑块
		colors[ImGuiCol_SliderGrab] = ImVec4{ 0.4f, 0.405f, 0.41f, 1.0f };
		colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.5f, 0.505f, 0.51f, 1.0f };

		// 复选框
		colors[ImGuiCol_CheckMark] = ImVec4{ 0.8f, 0.8f, 0.8f, 1.0f };

		// 文字
		colors[ImGuiCol_Text] = ImVec4{ 0.9f, 0.9f, 0.9f, 1.0f };
		colors[ImGuiCol_TextDisabled] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };

		// 弹出窗口
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 0.98f };

		// 分隔线
		colors[ImGuiCol_Separator] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// 高亮边框
		colors[ImGuiCol_NavHighlight] = ImVec4{ 0.4f, 0.4f, 0.4f, 1.0f };

		// 调整圆角风格
		ImGui::GetStyle().FrameRounding = 4.0f;
		ImGui::GetStyle().GrabRounding = 4.0f;
		ImGui::GetStyle().WindowRounding = 7.0f;
	}

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

	ImGuiLayer::~ImGuiLayer() {}

	void ImGuiLayer::OnAttach()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetSingleton().GetWindow().GetNativeWindow());
		const char* glsl_version = "#version 410";

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();
		SetDarkThemeColors();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		// Support Chinese characters in simhei
		io.Fonts->AddFontFromFileTTF(
			"Assets/Fonts/Open_Sans/OpenSans-VariableFont_wdth,wght.ttf",
			24.0f,
			NULL,
			io.Fonts->GetGlyphRangesChineseSimplifiedCommon()
		);
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();

		Application& app = Application::GetSingleton();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	
}