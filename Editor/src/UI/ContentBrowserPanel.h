#pragma once

#include "Dua/Core/Memory.h"
#include "ImGui/imgui.h"

#include <filesystem>
#include <vector>

namespace Dua {

    class EditorLayer;

    class ContentBrowserPanel
    {
    public:
        ContentBrowserPanel(EditorLayer* editorLayer);

        void OnDraw();

        void DrawDirectoryTree(const std::filesystem::path& path);
        void DrawCurrentPathHeader();
        void DrawFileList();

    private:

        EditorLayer* m_EditorLayer;
        std::filesystem::path m_CurrentDirectory;
        std::filesystem::path m_BaseDirectory;
    };

}