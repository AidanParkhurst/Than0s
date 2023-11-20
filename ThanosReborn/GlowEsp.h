#pragma once
#include "Feature.h"
#include <thread>
#include "MemoryManager.h"
#include "Offsets.h"

class GlowEsp : public Feature {
private:
	float glowColor[4];
	void ScanPlayers() {
		DWORD localBase = g_Mem->Read<DWORD>(clientBase + dwLocalPlayer);
		int localTeam = g_Mem->Read<int>(localBase + m_iTeamNum);
		for (int i = 0; i < 64; i++) {
			DWORD player = g_Mem->Read<DWORD>(clientBase + dwEntityList + i * 0x10);
			if (player == 0)
				continue;
			int playerTeam = g_Mem->Read<int>(player + m_iTeamNum);
			//TODO: Pick from list or something
			if (localTeam == playerTeam) {
				glowColor[0] = 0.0f;
				glowColor[1] = 1.0f;
				glowColor[2] = 0.0f;
				glowColor[3] = 1.0f;
			}
			else {
				glowColor[0] = 1.0f;
				glowColor[1] = 0.0f;
				glowColor[2] = 0.0f;
				glowColor[3] = 1.0f;
			}

			//Actually making the player glow
			int glowIndex = g_Mem->Read<int>(player + m_iGlowIndex);
			DWORD gObjMngAddr = g_Mem->Read<DWORD>(clientBase + dwGlowObjectManager);

			DWORD playerGlowAddr = glowIndex * 0x38;
			g_Mem->Write<float>(gObjMngAddr + playerGlowAddr + 0x4, glowColor[0]); //r
			g_Mem->Write<float>(gObjMngAddr + playerGlowAddr + 0x8, glowColor[1]); //g
			g_Mem->Write<float>(gObjMngAddr + playerGlowAddr + 0xC, glowColor[2]); //b
			g_Mem->Write<float>(gObjMngAddr + playerGlowAddr + 0x10, glowColor[3]); //a

			g_Mem->Write<bool>(gObjMngAddr + playerGlowAddr + 0x24, true); //RenderWhenOccluded
			g_Mem->Write<bool>(gObjMngAddr + playerGlowAddr + 0x25, false); //RenderWhenUnoccluded
			g_Mem->Write<bool>(gObjMngAddr + playerGlowAddr + 0x26, false); //Full Bloom

			g_Mem->Write<int>(gObjMngAddr + playerGlowAddr + 0x2C, 0); //Glow Style

		/*-----------------------------------------------------------------------------------*\
		|	This shit is incredibly strange and inconsistent, here's my observed behavior:    |
		|	rwo +  rwuo =  glow, !hp														  |
		|	!rwo + !rwuo = !glow, !hp														  |
		|	rwo + !rwuo =  glow,  hp														  |
		|	!rwo +  rwuo =  glow, !hp														  |
		|																					  |
		|	rwuo = !hp																		  |
		|	!rwuo =  hp IFF  rwo															  |
		|																					  |
		|	!rwo = glow IFF  rwuo															  |
		|	rwo = glow IFF !rwuo															  |
		\*-----------------------------------------------------------------------------------*/
		}
	}
public:
	void Run() {
		while (running) {
			if (toggled) {
				ScanPlayers();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
};