#include <iostream>
#include <string>

#include "Utils.h"

void Utils::print(const char msg[], int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	std::cout << msg << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT | WHITE);
}

void Utils::cprintf(const char msg[], int color, ...) {
	char buffer[256];
	va_list args;
	va_start(args, color);
	vsprintf_s(buffer, msg, args);
	va_end(args);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	std::cout << buffer << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT | WHITE);

}

void Utils::printf(const char msg[], ...) {
	char buffer[256];
	va_list args;
	va_start(args, msg);
	vsprintf_s(buffer, msg, args);
	va_end(args);
	std::cout << buffer << std::endl;
}

BYTE Utils::getByte(const char charByte[]) {
	return std::stoi(charByte, nullptr, 16);
}

std::vector<BYTE> Utils::BytesFromPattern(const char pattern[]) {
	std::vector<BYTE> bytes;
	for (int i = 0; i < strlen(pattern); i++) {
		if (pattern[i] == ' ')
			continue;
		if (pattern[i] == '?') {
			bytes.push_back('?');
			continue;
		}
		const char charByte[] = { pattern[i], pattern[i + 1] };
		i++;
		bytes.push_back(getByte(charByte));
	}
	return bytes;
}
