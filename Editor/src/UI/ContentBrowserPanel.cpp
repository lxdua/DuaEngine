#include "ContentBrowserPanel.h"

#include "Dua/Utils/PlatformUtils.h"
#include "ImGui/imgui_internal.h"
#include "EditorLayer.h"

namespace Dua {

    ContentBrowserPanel::ContentBrowserPanel(EditorLayer* editorLayer)
        : m_EditorLayer(editorLayer)
    {
        m_BaseDirectory = std::filesystem::current_path() / "Assets";
        m_CurrentDirectory = m_BaseDirectory;
    }

    void ContentBrowserPanel::OnDraw()
    {
        if (ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_NoCollapse))
        {
            if (ImGui::BeginTable("##ContentBrowserLayout", 2,
                ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("Tree", ImGuiTableColumnFlags_WidthFixed, 200);
                ImGui::TableSetupColumn("Files", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                if (ImGui::BeginChild("##DirectoryTree", ImVec2(0, 0), true))
                {
                    DrawDirectoryTree(m_BaseDirectory);
                }
                ImGui::EndChild();

                ImGui::TableSetColumnIndex(1);
                if (ImGui::BeginChild("##FileListContainer"))
                {
                    DrawCurrentPathHeader();

                    if (ImGui::BeginChild("##ScrollableFileList", ImVec2(0, 0), false,
                        ImGuiWindowFlags_AlwaysVerticalScrollbar))
                    {
                        DrawFileList();
                    }
                    ImGui::EndChild();
                }
                ImGui::EndChild();

                ImGui::EndTable();
            }
        }
        ImGui::End();
    }

    void ContentBrowserPanel::DrawDirectoryTree(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path)) return;

        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                const auto& dirPath = entry.path();
                std::string name = dirPath.filename().string();

                ImGui::PushID(dirPath.string().c_str());

                ImGuiTreeNodeFlags flags =
                    ImGuiTreeNodeFlags_OpenOnArrow |
                    ImGuiTreeNodeFlags_SpanFullWidth |
                    ImGuiTreeNodeFlags_AllowItemOverlap;

                bool isOpen = ImGui::TreeNodeEx("##TreeNode", flags);

                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                {
                    m_CurrentDirectory = dirPath.lexically_normal();
                }

                ImGui::SameLine();
                ImGui::Text("%s", name.c_str());

                if (isOpen)
                {
                    DrawDirectoryTree(dirPath);
                    ImGui::TreePop();
                }

                ImGui::PopID();
            }
        }
    }

    void ContentBrowserPanel::DrawCurrentPathHeader()
    {
        // ¬∑æ∂œ‘ æ«¯”Ú
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 6));

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted("Current Path: ");
        ImGui::SameLine(0, 4);

        const auto relativePath = m_CurrentDirectory.lexically_relative(m_BaseDirectory);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s",
            relativePath.empty() ? "/" : relativePath.string().c_str());

        ImGui::PopStyleVar();
        ImGui::Separator();

        // ÃÌº”º‰æ‡±‹√‚’⁄µ≤
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
    }

    void ContentBrowserPanel::DrawFileList()
    {
        static float padding = 16.0f;
        static float thumbnailSize = 96.0f;

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = static_cast<int>(panelWidth / (thumbnailSize + padding));
        columnCount = std::max(1, columnCount);

        if (ImGui::BeginTable("##FileList", columnCount))
        {
            for (const auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
            {
                if (!entry.is_directory())
                {
                    ImGui::TableNextColumn();

                    const auto& filePath = entry.path();
                    const std::string filename = filePath.filename().string();

                    ImGui::PushID(filePath.string().c_str());

                    ImGui::Button("[File]", ImVec2(thumbnailSize, thumbnailSize));
                    ImGui::TextWrapped("%s", filename.c_str());

                    ImGui::PopID();
                }
            }
            ImGui::EndTable();
        }
    }

}