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

        // ��ȡGLFW���ڵ�HWND
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetSingleton().GetWindow().GetNativeWindow());
        HWND hwnd = glfwGetWin32Window(window);

        // ��ʼ���ṹ��
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

        // ��ʼ���ṹ��
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;

        // �޸ı�־λ���������ļ�
        ofn.Flags = OFN_OVERWRITEPROMPT |   // ���������ļ�ʱ��ʾ
            OFN_PATHMUSTEXIST |     // ·���������
            OFN_ENABLESIZING |      // ��������Ի����С
            OFN_EXPLORER |          // ʹ���°���Դ���������
            OFN_NOCHANGEDIR;        // ���ı䵱ǰĿ¼

        // ��ȷ����Ĭ����չ��
        const char* ext_start = strstr(filter, "*.duaproj");
        if (ext_start)
        {
            ofn.lpstrDefExt = "duaproj";  // ֱ��ָ����չ��
        }

        if (GetSaveFileNameA(&ofn) == TRUE)
        {
            // �Զ������չ��������û�δ���룩
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