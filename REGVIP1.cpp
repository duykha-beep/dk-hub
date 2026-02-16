// NoRecoil.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "windows.h"
#include <iostream>
#include <string>
#include <signal.h>

//with different weapons, there are different fire rate and recoil.
DWORD fireRate;


bool LeftMouseDown = true;
int leftMouseVKCode = 1;
int RecoilState = 4;       //default state is turned off;

TCHAR windowsName[13] = { 'U', 'n', 'r', 'e', 'a', 'l', 'w', 'i', 'n', 'd', 'o', 'w', '\0' };

//LPCTSTR windowsName = L"UnrealWindow";

size_t iWeaponIndex = 0;
const size_t MAX_INDEX_WEAPON = 6;
const size_t MAX_INDEX_RECOIL = 31;
DWORD recoilTable[MAX_INDEX_WEAPON][MAX_INDEX_RECOIL] = {
	{ 24,24,24,24,24,24,24,24,24,24,24,28,28,28,28,29,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30 }, //AKM
	{ 20,21,22,21,22,22,23,22,23,23,24,24,25,25,25,25,26,27,28,29,30,32,34,34,35,34,35,34,35,34,35 }, //SCAR-L	
	{ 25,25,25,29,33,33,32,33,32,32,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30 }, //M16A4
	{ 21,21,21,21,21,21,21,21,21,23,23,24,23,24,25,25,26,27,27,32,31,31,31,31,31,31,31,32,32,32,35 }, //M416
	{ 18,19,18,19,18,19,19,21,23,24,23,24,23,24,23,24,23,24,23,24,23,24,24,25,24,25,24,25,24,25,24 }, //ump9
	{ 16,17,18,20,21,22,23,24,25,26,28,30,32,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34 } //UZI
};

float iScope[4] = { 0.75, 
					1.5, //2x scope
					3,   //4x scope
					6};  //8x scope

void __stdcall RemoveRecoil()
{
	HWND foregroundWin;
	float correntScope = iScope[0];
	leftMouseVKCode = 1;
	while (1)
	{
		foregroundWin = GetForegroundWindow();
		if (foregroundWin == FindWindow(windowsName, 0) && RecoilState == 3)
		{
			size_t iCount = 0;
			while (LeftMouseDown)
			{
				mouse_event(1u, 0, recoilTable[iWeaponIndex][iCount]*correntScope, 0, 3u);
				if (iCount < MAX_INDEX_RECOIL) iCount++;
				Sleep(fireRate);
			}
		}
		Sleep(1u);
	}
}

void __stdcall KeyHandlerThread()
{
	while (1)
	{
		if (GetAsyncKeyState(0x78) < 0)         
		{
			RecoilState = 3;
			Beep(0x320u, 0xC8u);
		}
		if (GetAsyncKeyState(0x79) < 0)           
		{
			RecoilState = 4;
			Beep(0x64u, 0xC8u);
		}
		LeftMouseDown = GetAsyncKeyState(leftMouseVKCode) < 0;
		Sleep(1u);
	}
}

bool bQuit = false;
void quit(int a) {
	bQuit = true;
}

int main() {

	std::cout << "please select favorite weapon: [0-5]\n";
	std::cout << "0. AK47\n";
	std::cout << "1. MP40\n";
	std::cout << "2. M1887\n";
	std::cout << "3. MP5\n";
	std::cout << "4. UMP\n";
	std::cout << "5. MINIUZI\n";

	char choice;
	std::cin.get(choice);
	iWeaponIndex = choice - '0';
	if (iWeaponIndex >= MAX_INDEX_WEAPON) {
		std::cout << "please input a number between 0 and 5\n";
		return -1;
	}
	std::string sWeaponName;
	switch (iWeaponIndex) {
	case 0:
		fireRate = 100u;
		sWeaponName = "AK47";
		break;
	case 1:
		fireRate = 96u;
		sWeaponName = "MP40";
		break;
	case 2:
		fireRate = 75u;
		sWeaponName = "M1887";
		break;
	case 3:
		fireRate = 86u;
		sWeaponName = "MP5";
		break;
	case 4:
		fireRate = 92u;
		sWeaponName = "UMP";
		break;
	case 5:
		fireRate = 48u;
		sWeaponName = "MINIUZI";
		break;

	default:
		std::cout << "Please the input a number between 0 to 5.\n";
		return -1;
	}

	std::cout << "Selected weapon: " << sWeaponName << std::endl;
	std::cout << "press 'F9' to enable, 'F10' to disable, 'Ctrl+C' to quit\n";
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)RemoveRecoil, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)KeyHandlerThread, 0, 0, 0);

	signal(SIGINT, quit);
	while (!bQuit) { Sleep(1000u); }
}