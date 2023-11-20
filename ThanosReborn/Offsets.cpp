#include "Offsets.h"
#include "MemoryManager.h"

//Bases
DWORD clientBase = 0;
DWORD engineBase = 0;

//Sigs
DWORD dwLocalPlayer = 0;
DWORD dwGlowObjectManager = 0;
DWORD dwForceJump = 0;
DWORD dwEntityList = 0;
DWORD dwForceAttack = 0;

void OffsetManager::scanOffsets() {
	g_Scan->init(g_Mem->pId, g_Mem->pHandle);
	dwLocalPlayer = g_Scan->DumpOffset("A3 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? E8 ? ? ? ? 59 C3 6A ?", ScanType::READ | ScanType::SUB, L"client_panorama.dll", 1, 16);
	dwGlowObjectManager = g_Scan->DumpOffset("A1 ? ? ? ? A8 01 75 4B", ScanType::READ | ScanType::SUB, L"client_panorama.dll", 1, 4);
	dwForceJump = g_Scan->DumpOffset("8B 0D ? ? ? ? 8B D6 8B C1 83 CA 02", ScanType::READ | ScanType::SUB, L"client_panorama.dll", 2, 0);
	dwForceAttack = g_Scan->DumpOffset("89 0D ? ? ? ? 8B 0D ? ? ? ? 8B F2 8B C1 83 CE 04", ScanType::READ | ScanType::SUB, L"client_panorama.dll", 2, 0);
	dwEntityList = g_Scan->DumpOffset("BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8", ScanType::READ | ScanType::SUB, L"client_panorama.dll", 1, 0);
}