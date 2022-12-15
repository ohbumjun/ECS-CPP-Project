
#include <Windows.h>
#include <iostream>
#include <time.h>
#include "ECS.h"

//함수설계
void ClearScreen();         //화면을 지운다.
void GameMain(float Delta); //게임메인
void PrintScreen();         //화면그리기

ECS* ECSObject = nullptr;

int main()
{
	srand(time(NULL)); //랜덤값 시드 만들기

	ECSObject = new ECS;

	int dwTime = GetTickCount();

	while (true)
	{
		if (dwTime + 50 < (int)GetTickCount())
		{
			dwTime = GetTickCount();

			//지우고
			ClearScreen();
			//플레이어나 적이 움직이고
			GameMain((float)dwTime);
			//그려준다.
			PrintScreen();
		}
	}

	delete ECSObject;
}

void ClearScreen()
{
	ECSObject->Refresh();
}

void GameMain(float Delta)
{
	ECSObject->Update(Delta);
}

void PrintScreen()
{
	ECSObject->Draw();
}
