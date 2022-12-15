
#include <Windows.h>
#include <iostream>
#include <time.h>
#include "ECS.h"

//�Լ�����
void ClearScreen();         //ȭ���� �����.
void GameMain(float Delta); //���Ӹ���
void PrintScreen();         //ȭ��׸���

ECS* ECSObject = nullptr;

int main()
{
	srand(time(NULL)); //������ �õ� �����

	ECSObject = new ECS;

	int dwTime = GetTickCount();

	while (true)
	{
		if (dwTime + 50 < (int)GetTickCount())
		{
			dwTime = GetTickCount();

			//�����
			ClearScreen();
			//�÷��̾ ���� �����̰�
			GameMain((float)dwTime);
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

void GameMain(float Delta)
{
	ECSObject->Update(Delta);
}

void PrintScreen()
{
	ECSObject->Draw();
}
