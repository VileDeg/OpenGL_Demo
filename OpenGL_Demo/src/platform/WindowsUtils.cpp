#include "pch.h"
#include "WindowsUtils.h"

#include <sstream>
//#include <commdlg.h>
#include <Windows.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "GLContext.h"

namespace WinUtils
{
    std::string GetExecutableDirectory()
    {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);

        std::string str = buffer;
        size_t index = 0;
        while (true) {
            /* Locate the substring to replace. */
            index = str.find("\\", index);
            if (index == std::string::npos)
                break;

            /* Make the replacement. */
            str.replace(index, 1, "/");

            /* Advance index forward so the next iteration doesn't pick it up as well. */
            index += 1;
        }
        std::string::size_type pos = str.find_last_of("\\/");

        return str.substr(0, pos);
    }

    std::optional<std::string> OpenFile(const char* filter)
    {
        OPENFILENAMEA ofn;

        CHAR szFileName[MAX_PATH] = "";

        ZeroMemory(&ofn, sizeof(ofn));

        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = glfwGetWin32Window(GLContext::Get().GetWindow().Handle());;
        ofn.lpstrFile = szFileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;
        return std::nullopt;
    }

    std::optional<std::string> SaveFile(const char* filter)
    {
        OPENFILENAMEA ofn;

        CHAR szFileName[MAX_PATH] = "";

        ZeroMemory(&ofn, sizeof(ofn));

        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = glfwGetWin32Window(GLContext::Get().GetWindow().Handle());;
        ofn.lpstrFile = szFileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        ofn.lpstrDefExt = strchr(filter, '\0') + 1;

        if (GetSaveFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;
        return std::nullopt;
    }
      
}
