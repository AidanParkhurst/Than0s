#include "Scanner.h"
#include "Utils.h"

bool Scanner::init(const wchar_t procName[]) {
	if (FindProcessId(procName, &procId)) {
		procHandle = OpenProcess(PROCESS_VM_READ, false, procId);
		return true;
	}
	return false;
}

void Scanner::init(DWORD pId, HANDLE pHandle) {
	procId = pId;
	procHandle = pHandle;
}

bool Scanner::FindProcessId(const wchar_t procName[], DWORD* procAddrBuffer) {
	HANDLE t32 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 proc;
	proc.dwSize = sizeof(PROCESSENTRY32);
	Process32First(t32, &proc);
	do {
		if (!wcscmp(proc.szExeFile, procName)) {
			if (procAddrBuffer)
				*procAddrBuffer = proc.th32ProcessID;
			CloseHandle(t32);
			return true;
		}
	} while (Process32Next(t32, &proc));
	CloseHandle(t32);
	return false;
}

bool Scanner::DumpModule(MODULEENTRY32 mod, std::vector<BYTE>* dumpBuffer) {
	std::vector<BYTE> dump = std::vector<BYTE>(mod.modBaseSize);
	if (!ReadProcessMemory(procHandle, (LPCVOID)(mod.modBaseAddr), &dump[0], mod.modBaseSize, NULL))
		return false;
	*dumpBuffer = dump;
	return true;
}

bool Scanner::FindModule(const wchar_t modName[], MODULEENTRY32* modEntryBuffer) {
	HANDLE t32 = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	MODULEENTRY32 mod;
	mod.dwSize = sizeof(MODULEENTRY32);
	Module32First(t32, &mod);
	do {
		if (!wcscmp(mod.szModule, modName)) {
			if (modEntryBuffer)
				*modEntryBuffer = mod;
			CloseHandle(t32);
			return true;
		}
	} while (Module32Next(t32, &mod));
	CloseHandle(t32);
	return false;
}

DWORD Scanner::DumpOffset(const char cPattern[], int type, const wchar_t modName[], int patternOffset, int addrOffset) {
	std::vector<BYTE> pattern = Utils::BytesFromPattern(cPattern);
	if (!procHandle) {
		Utils::print("[ Failed to dump offset: NULL procHandle ]", LIGHT | RED);
		return NULL;
	}
	MODULEENTRY32 mod;
	if (!FindModule(modName, &mod)) {
		Utils::print("[ Failed to dump offset: Can't find module! ]", LIGHT | RED);
		return NULL;
	}
	std::vector<BYTE> modBytes;
	if (!wcscmp(savedModName.c_str(), modName)) {
		modBytes = savedDump;
	}
	else {
		if (!DumpModule(mod, &modBytes)) {
			Utils::print("[ Failed to dump offset: Can't dump module! ]", LIGHT | RED);
			return NULL;
		}
		savedDump = modBytes;
		savedModName = modName;
	}
	for (int i = 0; i < modBytes.size() - pattern.size(); i++) {
		bool success = true;
		for (DWORD j = 0; j < pattern.size(); j++) {
			if (pattern.at(j) == '?') {
				continue;
			}
			BYTE buffer = modBytes.at(i + j);
			success = (pattern.at(j) == buffer);
			if (!success)
				break;
		}
		if (success) {
			DWORD result;
			if (type & ScanType::READ) {
				ReadProcessMemory(procHandle, mod.modBaseAddr + i + patternOffset, &result, sizeof(DWORD), NULL);
			}
			if (type & ScanType::SUB) {
				result -= (DWORD)mod.modBaseAddr;
			}
			return result + addrOffset;
		}
	}
	Utils::print("[ Failed to dump offset: Can't find pattern in module memory! ]", LIGHT | RED);
	return NULL;
}

void Scanner::ClearDump() {
	savedDump.clear();
	savedModName = L" ";
}