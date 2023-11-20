#pragma once
#include <Windows.h>
#include "Scanner.h"

namespace OffsetManager {
	void scanOffsets();
};

extern DWORD clientBase;
extern DWORD engineBase;

//Signatures
extern DWORD dwLocalPlayer;
extern DWORD dwGlowObjectManager;
extern DWORD dwForceJump;
extern DWORD dwForceAttack;
extern DWORD dwEntityList;

//Netvars TODO: Implement netvar dumping
const DWORD m_iTeamNum = 0xF0;
const DWORD m_iGlowIndex = 0xA320;
const DWORD m_bSpotted = 0x939;
const DWORD m_fFlags = 0x100;
const DWORD m_clrRender = 0x70;
const DWORD m_nModelIndex = 0x254;
const DWORD m_iCrosshairId = 0xB2B8;