#pragma once
#include "Feature.h"
#include <thread>
#include "MemoryManager.h"
#include "Offsets.h"

class Radar : public Feature {
private:
	void SetPlayersSpotted(bool bSpotted) {
		DWORD localBase = g_Mem->Read<DWORD>(clientBase + dwLocalPlayer);
		g_Mem->Write<int>(localBase + m_nModelIndex, 0);
		int localTeam = g_Mem->Read<int>(localBase + m_iTeamNum);

		for (int i = 0; i < 64; i++) {
			DWORD player = g_Mem->Read<DWORD>(clientBase + dwEntityList + i * 0x10);
			if (player == 0)
				continue;

			int playerTeam = g_Mem->Read<int>(player + m_iTeamNum);
			if (playerTeam == localTeam)
				continue;

			g_Mem->Write<bool>(player + m_bSpotted, bSpotted);
		}
	}
public:
	void Run() {
		while (running) {
			if (toggled) {
				SetPlayersSpotted(true);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void Toggle() {
		toggled = !toggled;
		SetPlayersSpotted(false);
	}
};