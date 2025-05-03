#include "duapch.h"
#include "Dua/Utils/PlatformUtils.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Dua/Core/Application.h"

namespace Dua {

    std::string FileDialogs::OpenFile(const char* filter)
    {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAME));

        // 获取GLFW窗口的HWND
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetSingleton().GetWindow().GetNativeWindow());
        HWND hwnd = glfwGetWin32Window(window);

        // 初始化结构体
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }
        return std::string();
    }

    std::string FileDialogs::SaveFile(const char* filter)
    {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAME));

        GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetSingleton().GetWindow().GetNativeWindow());
        HWND hwnd = glfwGetWin32Window(window);

        // 初始化结构体
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;

        // 修改标志位允许创建新文件
        ofn.Flags = OFN_OVERWRITEPROMPT |   // 覆盖已有文件时提示
            OFN_PATHMUSTEXIST |     // 路径必须存在
            OFN_ENABLESIZING |      // 允许调整对话框大小
            OFN_EXPLORER |          // 使用新版资源管理器风格
            OFN_NOCHANGEDIR;        // 不改变当前目录

        // 正确设置默认扩展名
        const char* ext_start = strstr(filter, "*.duaproj");
        if (ext_start)
        {
            ofn.lpstrDefExt = "duaproj";  // 直接指定扩展名
        }

        if (GetSaveFileNameA(&ofn) == TRUE)
        {
            // 自动添加扩展名（如果用户未输入）
            std::string path = ofn.lpstrFile;
            if (path.find(".duaproj") == std::string::npos)
            {
                path += ".duaproj";
            }
            return path;
        }
        return std::string();
    }

}