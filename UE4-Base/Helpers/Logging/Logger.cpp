#include "Logger.h"
#include <Shlobj.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>

bool Enabled = true;

void Logger::Log(const char* format, ...)
{
    SYSTEMTIME rawtime;
    GetSystemTime(&rawtime);
    char buf[MAX_PATH];
    auto size = GetTimeFormatA(LOCALE_CUSTOM_DEFAULT, 0, &rawtime, "[HH':'mm':'ss] ", buf, MAX_PATH) - 1;
    va_list argptr;
    va_start(argptr, format);
    size += vsnprintf(buf + size, sizeof(buf) - size, format, argptr);
    WriteFile(file, buf, size, NULL, NULL);
    va_end(argptr);
}


bool Logger::Remove()
{
    if (!file) return true;
    return CloseHandle(file);
}

bool Logger::Init()
{
    if (Enabled)
    {
        std::wstring FilePath(L"C:\\debug.sol");
        file = CreateFileW(FilePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        return file != INVALID_HANDLE_VALUE;
    }
}