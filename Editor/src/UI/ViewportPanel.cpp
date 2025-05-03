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
                ImGuiHoveredFlags_ChildWindows |  // �����Ӵ���
                ImGuiHoveredFlags_RootWindow      // ����������
            );

            // ��ȡ��ǰ��������
            ImVec2 viewportSize = ImGui::GetContentRegionAvail();

            // ��̬����֡����ߴ�
            if (m_EditorLayer->m_ViewportSize.x != viewportSize.x || m_EditorLayer->m_ViewportSize.y != viewportSize.y)
            {
                //m_Framebuffer->Resize(viewportSize.x, viewportSize.y);
                m_EditorLayer->m_ViewportSize = { viewportSize.x, viewportSize.y };
                m_EditorLayer->m_CameraController.OnResize((float)viewportSize.x, (float)viewportSize.y);
            }

            // ��ʾ֡�������ݣ��Զ���䴰�ڣ�
            ImGui::Image(
                m_EditorLayer->m_Framebuffer->GetColorAttachmentRendererID(),
                viewportSize,
                ImVec2(0, 1), ImVec2(1, 0)
            ); // ��תY��
        }
        ImGui::End();
        ImGui::PopStyleVar();
	}

}