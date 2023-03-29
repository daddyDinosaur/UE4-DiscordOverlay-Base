#pragma once
#include <Windows.h>
#include <string>

class Logger
{
public:
    static inline HANDLE file;
    static bool Init();
    static bool Remove();
    static void Log(const char* format, ...);
    inline static char szPath;
};
