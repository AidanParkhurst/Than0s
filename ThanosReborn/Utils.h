#pragma once
#include <Windows.h>
#include <vector>

const int WHITE = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
const int GREEN = FOREGROUND_GREEN;
const int RED = FOREGROUND_RED;
const int BLUE = FOREGROUND_BLUE;
const int LIGHT = FOREGROUND_INTENSITY;

namespace Utils {
	void print(const char msg[], int color);
	void printf(const char msg[], ...);
	void cprintf(const char msg[], int color, ...);
	BYTE getByte(const char charByte[]);
	std::vector<BYTE> BytesFromPattern(const char pattern[]);
}