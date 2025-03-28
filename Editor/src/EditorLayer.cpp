#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Dua {

    EditorLayer::EditorLayer(const std::string& name) : Layer(name), m_CameraController(1280.0f / 720.0f) {}

    void EditorLayer::OnAttach()
    {
        m_Texture = Dua::Texture2D::Create("Assets/Textures/ntx.png");

        Dua::FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Dua::Framebuffer::Create(fbSpec);
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(Dua::Timestep ts)
    {
        m_CameraController.OnUpdate(ts);

        m_Framebuffer->Bind();
        Dua::RenderCommand::SetClearColor({ 0.15,0.15,0.15,1.0 });
        Dua::RenderCommand::Clear();

        Dua::Renderer2D::BeginScene(m_CameraController.GetCamera());

        Dua::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f,1.0f }, 0.0f, m_Texture, m_SquareColor);

        for (float x = -5.0f; x <= 5.0f; x += 1)Dua::Renderer2D::DrawQuad({ x,-0.5f }, { 1.0f,1.0f }, 0, m_Texture, { 1.0f,1.0f,1.0f,1.0f });
        for (float x = -5.0f; x <= 5.0f; x += 1)Dua::Renderer2D::DrawQuad({ x,0.5f }, { 1.0f,1.0f }, 0, m_Texture, { (x + 5) / 10.0f,0.5f,1.0f,0.5f });

        Dua::Renderer2D::EndScene();
        m_Framebuffer->UnBind();
    }

    void EditorLayer::OnImGuiRender()
    {
        //-----------------------------
        // 1. 全屏基础设置
        //-----------------------------
        static bool dockspaceOpen = true;
        static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus;

        //-----------------------------
        // 2. 主窗口和DockSpace
        //-----------------------------
        ImGui::Begin("Editor Dockspace", &dockspaceOpen, windowFlags);
        ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspaceID = ImGui::GetID("EditorDockspace");
            ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
        }

        //-----------------------------
        // 3. 菜单栏
        //-----------------------------
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                    Dua::Application::GetSingleton().Close();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        //-----------------------------
        // 4. 左侧面板（场景+内容浏览器）
        //-----------------------------
        if (ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoCollapse))
        {
            // 场景层级显示
            ImGui::Text("Scene Hierarchy");
            ImGui::Separator();
            ImGui::Button("Camera");
            ImGui::Button("Light");
            ImGui::Button("Player");
        }
        ImGui::End();

        if (ImGui::Begin("Content", nullptr, ImGuiWindowFlags_NoCollapse))
        {
            // 文件浏览器实现
            ImGui::Text("Assets Directory");
            ImGui::Separator();
            ImGui::Button("Scenes");
            ImGui::Button("Textures");
            ImGui::Button("Scripts");
        }
        ImGui::End();

        //-----------------------------
        // 5. 视口区域（自由宽高比）
        //-----------------------------
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar))
        {
            // 获取当前可用区域
            ImVec2 viewportSize = ImGui::GetContentRegionAvail();

            // 动态调整帧缓冲尺寸
            if (m_ViewportSize.x != viewportSize.x || m_ViewportSize.y != viewportSize.y)
            {
                m_Framebuffer->Resize(viewportSize.x, viewportSize.y);
                m_ViewportSize = { viewportSize.x, viewportSize.y };
            }

            // 显示帧缓冲内容（自动填充窗口）
            ImGui::Image(m_Framebuffer->GetColorAttachmentRendererID(),
                viewportSize,
                ImVec2(0, 1), ImVec2(1, 0)); // 翻转Y轴
        }
        ImGui::End();
        ImGui::PopStyleVar();

        //-----------------------------
        // 6. 右侧检查器面板
        //-----------------------------
        if (ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse))
        {
            /*
            static float rotation[3] = { 0 };
            static float scale[3] = { 1,1,1 };

            ImGui::Text("Transform");
            ImGui::Separator();
            ImGui::DragFloat3("Position", m_Position, 0.1f);
            ImGui::DragFloat3("Rotation", rotation, 1.0f);
            ImGui::DragFloat3("Scale", scale, 0.1f);

            ImGui::Spacing();
            ImGui::Text("Material");
            ImGui::ColorEdit3("Albedo", m_AlbedoColor);
            ImGui::DragFloat("Metallic", &m_Metallic, 0.01f, 0.0f, 1.0f);
            */
        }
        ImGui::End();

        //-----------------------------
        // 7. 结束主窗口
        //-----------------------------
        ImGui::End();
    }

    void EditorLayer::OnEvent(Dua::Event& event)
    {
        m_CameraController.OnEvent(event);
    }


}