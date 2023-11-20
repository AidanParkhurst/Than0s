#pragma once
#include "Feature.h"
#include <thread>
#include "MemoryManager.h"
#include "Offsets.h"

class NoHand : public Feature {
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
			int modelIndex = g_Mem->Read<int>(localBase + m_nModelIndex);
			if (modelIndex != 0)
				oIndex = modelIndex;
			if (toggled) {
				ChangeModelIndex(0);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	void Toggle() {
		toggled = !toggled;
		ChangeModelIndex(oIndex);
	}
};