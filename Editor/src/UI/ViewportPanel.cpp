#include "ViewportPanel.h"

//#include "Dua/Core/Memory.h"
#include "ImGui/imgui.h"
#include "EditorLayer.h"

namespace Dua {

	ViewportPanel::ViewportPanel(EditorLayer* editorLayer) : m_EditorLayer(editorLayer) {}

	void ViewportPanel::OnDraw()
	{
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar))
        {
            m_EditorLayer->m_CameraController.m_EnableControl = ImGui::IsWindowHovered(
                ImGuiHoveredFlags_ChildWindows |  // 包含子窗口
                ImGuiHoveredFlags_RootWindow      // 仅检测根窗口
            );

            // 获取当前可用区域
            ImVec2 viewportSize = ImGui::GetContentRegionAvail();

            // 动态调整帧缓冲尺寸
            if (m_EditorLayer->m_ViewportSize.x != viewportSize.x || m_EditorLayer->m_ViewportSize.y != viewportSize.y)
            {
                //m_Framebuffer->Resize(viewportSize.x, viewportSize.y);
                m_EditorLayer->m_ViewportSize = { viewportSize.x, viewportSize.y };
                m_EditorLayer->m_CameraController.OnResize((float)viewportSize.x, (float)viewportSize.y);
            }

            // 显示帧缓冲内容（自动填充窗口）
            ImGui::Image(
                m_EditorLayer->m_Framebuffer->GetColorAttachmentRendererID(),
                viewportSize,
                ImVec2(0, 1), ImVec2(1, 0)
            ); // 翻转Y轴
        }
        ImGui::End();
        ImGui::PopStyleVar();
	}

}