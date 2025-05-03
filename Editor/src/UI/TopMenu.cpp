#include "TopMenu.h"

#include "Dua/Core/Memory.h"
#include "Dua/Core/Application.h"
#include "Dua/Scene/SceneSerializer.h"
#include "Dua/Utils/PlatformUtils.h"
#include "ImGui/imgui.h"
#include "EditorLayer.h"

#include <string>

namespace Dua {

    TopMenu::TopMenu(EditorLayer* editorLayer) : m_EditorLayer(editorLayer) {}

    void TopMenu::OnDraw()
	{
        if (ImGui::BeginMenuBar())
        {
            DrawLeftMenu();
            ImGui::SameLine(ImGui::GetWindowContentRegionMax().x * 0.8f);
            if (ImGui::Button("Run"))
            {
                //m_EditorLayer->ToggleRuntime();
            }
            ImGui::EndMenuBar();
        }
	}

    void TopMenu::NewScene()
    {
        m_EditorLayer->m_Scene = CreateRef<Scene>();
    }

    void TopMenu::SaveScene()
    {
        std::string filepath = FileDialogs::SaveFile("Text Files (*.duaproj)\0*.duaproj\0All Files (*.*)\0*.*\0");
        if (!filepath.empty())
        {
            SceneSerializer serializer(m_EditorLayer->m_Scene);
            serializer.Serialize(filepath);
        }
    }

    void TopMenu::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("Text Files (*.duaproj)\0*.duaproj\0All Files (*.*)\0*.*\0");
        if (!filepath.empty())
        {
            m_EditorLayer->m_Scene = CreateRef<Scene>();

            SceneSerializer serializer(m_EditorLayer->m_Scene);
            serializer.Deserialize(filepath);
        }
    }


    void TopMenu::ExportScene()
    {
    }

    void TopMenu::DrawLeftMenu()
    {
        if (ImGui::BeginMenu("Scene"))
        {
            if (ImGui::MenuItem("New Scene"))
            {
                NewScene();
            }
            if (ImGui::MenuItem("Save Scene"))
            {
                SaveScene();
            }
            if (ImGui::MenuItem("Open Scene"))
            {
                OpenScene();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Export Scene"))
            {
                ExportScene();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Scene Settings"))
            {
                //
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Import"))
        {
            if (ImGui::MenuItem("Import image"))
            {

            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Editor"))
        {
            if (ImGui::MenuItem("Editor Settings"))
                ;
            if (ImGui::MenuItem("Exit"))
                Application::GetSingleton().Close();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About Dua"))
                ;
            ImGui::EndMenu();
        }
    }

}