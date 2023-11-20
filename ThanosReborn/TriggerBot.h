#pragma once
#include "Feature.h"
#include <thread>
#include "MemoryManager.h"
#include "Offsets.h"

class TriggerBot : public Feature {
private:
	DWORD localBase;
	int oIndex;
	void ChangeModelIndex(int index) {
		g_Mem->Write<int>(localBase + m_nModelIndex, index);
	}
public:
	void Run() {
		while (running) {
			localBase = g_Mem->Read<DWORD>(clientBase + dwLocalPlayer);
			if (toggled && GetAsyncKeyState(VK_MENU)) {
				int localTeam = g_Mem->Read<int>(localBase + m_iTeamNum);
				int crossId = g_Mem->Read<int>(localBase + m_iCrosshairId);
				if (crossId != 0) {
					int player = g_Mem->Read<int>(clientBase + dwEntityList + (crossId * 0x10) - 0x10);
					int crossTeam = g_Mem->Read<int>(player + m_iTeamNum);
					if (crossTeam != localTeam) {
						g_Mem->Write<int>(clientBase + dwForceAttack, 6);
					}
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
};