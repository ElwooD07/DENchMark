#pragma once
#include <Windows.h>

DWORD WINAPI ThreadProcCPU(PVOID parameters);
DWORD WINAPI ThreadProcMemory(PVOID parameters);
DWORD WINAPI ThreadProcCPUandMemory(PVOID parameters);
