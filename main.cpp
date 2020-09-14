#include "proc.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <math.h>
#include <tgmath.h>
#define PI 3.14159265
//WORD m_XPos = 0x0038;
//WORD m_YPos = 0x003C;
//WORD YawVal = 0x0040;
//WORD PitchVal = 0x0044;

int main()
{
	int health = 10000;
	char name[260];

	float xPos = 0;
	float yPos = 0;
	float zPos = 0;

	float xPosBot = 0;
	float yPosBot = 0;
	float zPosBot = 0;

	float xPosTemp = 0;
	float yPosTemp = 0;
	float zPosTemp = 0;

	float yawAngle = 0;
	float pitchAngle = 0;

	float distanceArray[3];
	float magnitude = 0;

	DWORD procID = getProcID(L"ac_client.exe");
	uintptr_t moduleBaseAddress = getModuleBaseAddress(procID, L"ac_client.exe");
	HANDLE handle = 0;
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

	uintptr_t localBaseAddress = 0x50F4F4;
	uintptr_t entityListAddress = 0x400000 + 0x10F4F8;

	std::vector<unsigned int> offsetsOneX = { 0x4 };
	uintptr_t playerX = findDMAAddress(handle, localBaseAddress, offsetsOneX);
	std::vector<unsigned int> offsetsTwoY = { 0x8 };
	uintptr_t playerY = findDMAAddress(handle, localBaseAddress, offsetsTwoY);
	std::vector<unsigned int> offsetsThreeZ = { 0xc };
	uintptr_t playerZ = findDMAAddress(handle, localBaseAddress, offsetsThreeZ);

	std::vector<unsigned int> offsetYaw = { 0x40 };
	uintptr_t yawAddress = findDMAAddress(handle, localBaseAddress, offsetYaw);
	std::vector<unsigned int> offsetPitch = { 0x44 };
	uintptr_t pitchAddress = findDMAAddress(handle, localBaseAddress, offsetPitch);

	std::vector<unsigned int> offsetsOne = { 0x08, 0x4 };
	uintptr_t entityListX = findDMAAddress(handle, entityListAddress, offsetsOne);
	std::vector<unsigned int> offsetsTwo = { 0x08, 0x8 };
	uintptr_t entityListY = findDMAAddress(handle, entityListAddress, offsetsTwo);
	std::vector<unsigned int> offsetsThree = { 0x08, 0xc };
	uintptr_t entityListZ = findDMAAddress(handle, entityListAddress, offsetsThree);
	std::vector<unsigned int> offsetsName = { 0x08, 0x225 };
	uintptr_t nameAddress = findDMAAddress(handle, entityListAddress, offsetsName);

	
	float tempYaw;


	while (true)
	{

		ReadProcessMemory(handle, (BYTE*)playerX, &xPos, sizeof(xPos), 0);
		ReadProcessMemory(handle, (BYTE*)playerY, &yPos, sizeof(yPos), 0);
		ReadProcessMemory(handle, (BYTE*)playerZ, &zPos, sizeof(zPos), 0);
		ReadProcessMemory(handle, (BYTE*)entityListX, &xPosBot, sizeof(xPosBot), 0);
		ReadProcessMemory(handle, (BYTE*)entityListY, &yPosBot, sizeof(yPosBot), 0);
		ReadProcessMemory(handle, (BYTE*)entityListZ, &zPosBot, sizeof(zPosBot), 0);
		ReadProcessMemory(handle, (BYTE*)nameAddress, &name, sizeof(name), 0);
		ReadProcessMemory(handle, (BYTE*)yawAddress, &tempYaw, sizeof(yawAngle), 0);
		ReadProcessMemory(handle, (BYTE*)pitchAddress, &pitchAngle, sizeof(pitchAngle), 0);



		xPosTemp = xPosBot - xPos;
		yPosTemp = yPosBot - yPos;
		zPosTemp = zPosBot - zPos;

		distanceArray[0] = { xPosTemp };
		distanceArray[1] = { yPosTemp };
		distanceArray[2] = { zPosTemp };

		magnitude = sqrt(pow(xPosTemp, 2) + pow(yPosTemp, 2) + pow(zPosTemp, 2));

		yawAngle = (atan2(tempYaw, pitchAngle)) * 180 / PI;
		pitchAngle = acos(zPosTemp / magnitude);

		WriteProcessMemory(handle, (BYTE*)pitchAddress, &pitchAngle, sizeof(pitchAngle), 0);
		WriteProcessMemory(handle, (BYTE*)yawAddress, &yawAngle, sizeof(pitchAngle), 0);


		std::cout << xPosBot << std::endl;
		std::cout << yPosBot << std::endl;
		std::cout << zPosBot << std::endl;
		std::cout << "\n";
		std::cout << xPos << std::endl;
		std::cout << yPos << std::endl;
		std::cout << zPos << std::endl;
		std::cout << "\n";
		std::cout << name << std::endl;
		std::cout << yawAngle << std::endl;
		std::cout << pitchAngle << std::endl;
		std::cout << magnitude << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		system("cls");
	}
	
}
