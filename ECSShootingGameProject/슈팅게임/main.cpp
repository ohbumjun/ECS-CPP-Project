
#include <Windows.h>
#include <iostream>
#include <time.h>
#include "ECS.h"
#include "ThreadPool.h"

//함수설계
void ClearScreen();         //화면을 지운다.
void GameMain(int Delta); //게임메인
void PrintScreen();         //화면그리기

ECS* ECSObject = nullptr;

int main()
{
	srand(time(NULL)); //랜덤값 시드 만들기

	ECSObject = new ECS;
	ECSObject->Init();

	int dwTime = GetTickCount();

	while (true)
	{
		int curDwTime = GetTickCount();
		if (dwTime + 250 < curDwTime)
		{
			dwTime = GetTickCount();

			//지우고
			ClearScreen();
			//플레이어나 적이 움직이고
			GameMain(1);
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

void GameMain(int Delta)
{
	ECSObject->Update(Delta);
}

void PrintScreen()
{
	ECSObject->Draw();
}
