
#include <Windows.h>
#include <iostream>
#include <time.h>
#include "ECS.h"
#include "ThreadPool.h"

//�Լ�����
void ClearScreen();         //ȭ���� �����.
void GameMain(int Delta); //���Ӹ���
void PrintScreen();         //ȭ��׸���

ECS* ECSObject = nullptr;

int main()
{
	srand(time(NULL)); //������ �õ� �����

	ECSObject = new ECS;
	ECSObject->Init();

	int dwTime = GetTickCount();

	while (true)
	{
		int curDwTime = GetTickCount();
		if (dwTime + 250 < curDwTime)
		{
			dwTime = GetTickCount();

			//�����
			ClearScreen();
			//�÷��̾ ���� �����̰�
			GameMain(1);
			//�׷��ش�.
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
