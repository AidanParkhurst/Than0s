#pragma once
#include "Feature.h"
#include <thread>
#include "MemoryManager.h"
#include "Offsets.h"

class Bhop : public Feature {
public:
	void Run() {
		while (running) {
			if (toggled) {
				DWORD localBase = g_Mem->Read<DWORD>((DWORD)(clientBase + dwLocalPlayer));
				if (GetAsyncKeyState(VK_SPACE) && g_Mem->Read<int>(localBase + m_fFlags) & 1)
					g_Mem->Write<int>(clientBase +dwForceJump, 6);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
};