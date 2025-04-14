#include "EditorLayer.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Dua {

    EditorLayer::EditorLayer(const std::string& name) : Layer(name), m_CameraController(1280.0f / 720.0f) {}

    class MovementScript {
    public:
        void OnCreate(Entity entity) {
            if (entity.HasComponent<TransformComponent>()) {
                m_Transform = &entity.GetComponent<TransformComponent>();
            }
        }

        void OnUpdate(Entity entity, Timestep ts) {
            if (m_Transform && entity.IsValid()) {
                // 绕 Y 轴旋转（单位：弧度/秒）
                m_Transform->SetRotation(m_Transform->Rotation + speed * ts);
            }
        }

        void OnDestroy(Entity entity) {
            m_Transform = nullptr;
        }

        float speed = 1.0f; // 默认 1 弧度/秒（约 57.3 度/秒）

    private:
        TransformComponent* m_Transform = nullptr;
    };

    void EditorLayer::OnAttach()
    {
        m_Texture = Texture2D::Create("Assets/Textures/ntx.png");

        FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

        m_Scene = CreateRef<Scene>();

        auto sprite = m_Scene->CreateEntity("sprite");
        sprite->AddComponent<SpriteComponent>();
        auto& script = sprite->AddComponent<NativeScriptComponent>();
        script.Bind<MovementScript>();

        // 配置脚本参数（类型安全方式）
        if (auto* movement = static_cast<MovementScript*>(script.instance.get())) {
            movement->speed = -0.5f;
        }

        // Panels
        m_SceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(m_Scene);

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
        m_Scene->OnUpdate(ts);
        Renderer2D::EndScene();

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
            if (ImGui::BeginMenu("Project"))
            {
                if (ImGui::MenuItem("Project Settings"))
                    ;
                if (ImGui::MenuItem("Exit"))
                    Application::GetSingleton().Close();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Editor"))
            {
                if (ImGui::MenuItem("Editor Settings"))
                    ;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("About Dua"))
                    ;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        //-----------------------------
        // 4. 左侧面板（场景+内容浏览器）
        //-----------------------------
        m_SceneHierarchyPanel->OnImGuiRender();

        if (ImGui::Begin("File", nullptr, ImGuiWindowFlags_NoCollapse))
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
                //m_Framebuffer->Resize(viewportSize.x, viewportSize.y);
                m_ViewportSize = { viewportSize.x, viewportSize.y };

                m_CameraController.OnResize((float)viewportSize.x, (float)viewportSize.y);
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

        }
        ImGui::End();

        //-----------------------------
        // 7. 结束主窗口
        //-----------------------------
        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);
    }

}