#include "MemoryManager.h"
#include <iostream>
#include <thread>
#include <vector>
#include "Offsets.h"
#include "Utils.h"
#include "Scanner.h"

#include "Feature.h"
#include "Bhop.h"
#include "GlowEsp.h"
#include "Radar.h"
#include "NoHands.h"
#include "TriggerBot.h"

//Defining the global memory manager
MemoryManager* g_Mem = new MemoryManager();
Scanner* g_Scan = new Scanner();

int main() {
	//Setting up process variables
	SetConsoleTitle(L"Than0s Reborn");
	Utils::print(" -- [ Welcome to Than0s Reborn ] -- ", LIGHT | RED | BLUE);

	Utils::print("[Waiting for CSGO process...]", LIGHT | BLUE);
	while (!g_Mem->Open(L"csgo.exe")) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	Utils::print("[CSGO Found!]", LIGHT | GREEN);

	Utils::print("[Waiting for all modules to load...]", LIGHT | BLUE);
	while (!g_Mem->FindModule(L"serverbrowser.dll", NULL)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	Utils::print("[All modules have loaded!]", LIGHT | GREEN);

	Utils::print("[Finding necessary modules...]", LIGHT | BLUE);
	while (!g_Mem->FindModule(L"client_panorama.dll", &clientBase)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	while (!g_Mem->FindModule(L"engine.dll", &engineBase)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	Utils::print("[Necessary modules found!]", LIGHT | GREEN);

	Utils::print("[Scanning for offsets...]", LIGHT | BLUE);
	OffsetManager::scanOffsets();
	Utils::print("[Offsets found!]", LIGHT | GREEN);

	Utils::print("[Starting features...]", LIGHT | BLUE);
	//Setting up features
	std::vector<Feature*> features;
	std::vector<std::thread*> featureThreads;

	Bhop bhop;
	std::thread tBhop = std::thread(&Bhop::Run, &bhop);
	features.push_back(&bhop);
	featureThreads.push_back(&tBhop);

	GlowEsp glow;
	std::thread tGlow = std::thread(&GlowEsp::Run, &glow);
	features.push_back(&glow);
	featureThreads.push_back(&tGlow);

	Radar radar;
	std::thread tRadar = std::thread(&Radar::Run, &radar);
	features.push_back(&radar);
	featureThreads.push_back(&tRadar);

	NoHand noHand;
	std::thread tNoHand = std::thread(&NoHand::Run, &noHand);
	features.push_back(&noHand);
	featureThreads.push_back(&tNoHand);

	TriggerBot trig;
	std::thread tTrig = std::thread(&TriggerBot::Run, &trig);
	features.push_back(&trig);
	featureThreads.push_back(&tTrig);
	Utils::print("[All features running!]", LIGHT | GREEN);
	Utils::print(" -- [ Startup Completed! Press DELETE to shutdown! ] --", LIGHT | RED | BLUE);

	//Checking for input TODO: Use an overlay instead
	while (!GetAsyncKeyState(VK_DELETE)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	//Shutting down all features before closing
	for (auto feature : features) {
		feature->Stop();
	}
	for (auto fThread : featureThreads) {
		fThread->join();
	}
	return EXIT_SUCCESS;
}

