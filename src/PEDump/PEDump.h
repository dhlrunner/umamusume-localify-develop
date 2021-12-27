#pragma once
#include <string>
#include <windows.h>
#include <psapi.h>

void pedump(HMODULE mod, const std::string& name);