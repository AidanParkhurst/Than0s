#pragma once
#include <Windows.h>
#include <TlHelp32.h>

class MemoryManager {
public:
	DWORD pId;
	HANDLE pHandle;

	MemoryManager() {
		pId = 0;
		pHandle = INVALID_HANDLE_VALUE;
	}

	bool Open(const wchar_t pName[]) {
		HANDLE t32 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		PROCESSENTRY32 proc;
		proc.dwSize = sizeof(PROCESSENTRY32);
		Process32First(t32, &proc);
		do {
			if (!wcscmp(proc.szExeFile, pName)) {
				pId = proc.th32ProcessID;
				pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
				CloseHandle(t32);
				return true;
			}
		} while (Process32Next(t32, &proc));
		CloseHandle(t32);
		return false;
	}

	bool FindModule(const wchar_t mName[], DWORD* mBaseAddrBuffer) {
		HANDLE t32 = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pId);
		MODULEENTRY32 mod;
		mod.dwSize = sizeof(MODULEENTRY32);
		Module32First(t32, &mod);
		do {
			if (!wcscmp(mod.szModule, mName)) {
				if(mBaseAddrBuffer)
					*mBaseAddrBuffer = (DWORD)mod.modBaseAddr;
				CloseHandle(t32);
				return true;
			}
		} while (Module32Next(t32, &mod));
		CloseHandle(t32);
		return false;
	}

	template <typename T>
	T Read(DWORD address) {
		T tBuffer;
		ReadProcessMemory(pHandle, (LPCVOID)address, (LPVOID)&tBuffer, sizeof(T), NULL);
		return tBuffer;
	}

	template <typename T>
	bool Write(DWORD address, T value) {
		bool result = WriteProcessMemory(pHandle, (LPVOID)address, (LPCVOID)&value, sizeof(T), NULL);
		return result;
	}
};
extern MemoryManager* g_Mem;