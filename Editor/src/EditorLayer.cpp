#include "EditorLayer.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Dua {

    EditorLayer::EditorLayer(const std::string& name) : Layer(name), m_CameraController(1280.0f / 720.0f) {}

    void EditorLayer::OnAttach()
    {
        FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        m_Scene = CreateRef<Scene>();

        m_RuntimeState = EditorRuntimeState::Editing;

        // UI
        m_TopMenu = CreateRef<TopMenu>(this);
        m_SceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(this);
        m_ViewportPanel = CreateRef<ViewportPanel>(this);
        m_ContentBrowserPanel = CreateRef<ContentBrowserPanel>(this);


        // test
        
        // 创建实体
        auto rotatingEntity = m_Scene->CreateEntity("Rotating Cube");
        // 添加必须的组件
        auto& sprite = rotatingEntity->AddComponent<SpriteComponent>();
        // 添加 Lua 脚本组件并绑定脚本
        auto& luaScript = rotatingEntity->AddComponent<LuaScriptComponent>(rotatingEntity.get());
        luaScript.LoadScript("Scripts/Try.lua");
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        m_CameraController.OnUpdate(ts);

        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.15,0.15,0.15,1.0 });
        RenderCommand::Clear();

        Renderer2D::BeginScene(m_CameraController.GetCamera());

        if (m_RuntimeState == EditorRuntimeState::Editing)
            m_Scene->OnUpdate(ts);
        else if (m_RuntimeState == EditorRuntimeState::Running)
            m_Scene->OnRuntimeUpdate(ts);

        Renderer2D::EndScene();

        m_Framebuffer->UnBind();
    }

    void InitImGui()
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
    }

    void EditorLayer::OnImGuiRender()
    {
        InitImGui();

        m_TopMenu->OnDraw();
        m_SceneHierarchyPanel->OnDraw();
        m_ViewportPanel->OnDraw();
        m_ContentBrowserPanel->OnDraw();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);
    }

    void EditorLayer::OpenFile(const std::filesystem::path& path)
    {
        // 根据文件类型分发处理
        const std::string ext = path.extension().string();

        if (ext == ".scene")
        {
            //LoadScene(path);
        }
        else if (ext == ".png" || ext == ".jpg")
        {
            //OpenTextureEditor(path);
        }
        else
        {
            std::cout << "Unsupported file type: " << ext;
        }
    }

    void EditorLayer::ToggleRuntimeState(EditorRuntimeState state)
    {
        if (state == EditorRuntimeState::Editing)
        {
            m_Scene->OnRuntimeStop();

            m_RuntimeState = EditorRuntimeState::Editing;
        }
        else if (state == EditorRuntimeState::Running)
        {

            m_Scene->OnRuntimeStart();
            
            m_RuntimeState = EditorRuntimeState::Running;
        }
    }

}