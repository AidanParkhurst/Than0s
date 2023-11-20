#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

namespace ScanType {
	static int READ = 0x01;
	static int SUB = 0x10;
}

class Scanner {
private:
	DWORD procId;
	HANDLE procHandle;
	std::vector<BYTE> savedDump;
	std::wstring savedModName;
	bool DumpModule(MODULEENTRY32 mod, std::vector<BYTE>* dumpBuffer);
public:
	bool init(const wchar_t procName[]);
	void init(DWORD procId, HANDLE procHandle);
	bool FindProcessId(const wchar_t procName[], DWORD* procAddrBuffer);
	bool FindModule(const wchar_t modName[], MODULEENTRY32* modEntryBuffer);
	DWORD DumpOffset(const char cPattern[], int type, const wchar_t modName[], int patternOffset, int addrOffset);
	void ClearDump();
};
extern Scanner* g_Scan;