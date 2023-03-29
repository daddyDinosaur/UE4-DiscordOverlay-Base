#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <string>
#include <optional>
#include <Psapi.h>

namespace mem
{
    uintptr_t PatternScan(uintptr_t moduleAdress, const char* signature);
}