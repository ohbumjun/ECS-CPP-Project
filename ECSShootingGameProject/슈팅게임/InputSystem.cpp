#include "InputSystem.h"
#include "TransformComponent.h"
#include "ECS.h"

bool InputSystem::Init()
{
	//�÷��̾� ��ǥ��ġ
	// playerX = 0;
	// playerY = 12;

	// Transform ������ Mask �� �߰��Ѵ�
	addComponent<TransformComponent>();

    return true;
}

void InputSystem::updateComponents(float delta)
{
	char pressKey;

	// Player Entity �� Transform Component ���� ��������
	TransformComponent* PlayerTransformComp = m_ECS->GetEntityComponent<TransformComponent>(m_PlayerEntityID);

	//Ű�� �ԷµǸ� ����ش�.
	if (_kbhit())
	{
		pressKey = _getch();

		switch (pressKey)
		{
		case 'w':
		{
			//���ʹ���
			PlayerTransformComp->IncPosY(-1);
			// playerY--;
			if (PlayerTransformComp->GetPosY() < 0)
				PlayerTransformComp->SetPosY(0);
		}
			break;
		case 'a':
		{
			PlayerTransformComp->IncPosX(-1);
			// playerX--;
			if (PlayerTransformComp->GetPosX() < 0)
				PlayerTransformComp->SetPosX(0);
		}
			break;
		case 'd':
		{
			PlayerTransformComp->IncPosX(1);
			// playerX--;
			if (PlayerTransformComp->GetPosX() > 75)
				PlayerTransformComp->SetPosX(75);
		}
			break;
		case 's':
		{
			PlayerTransformComp->IncPosY(1);
			// playerY++;
			if (PlayerTransformComp->GetPosY() > 23)
				PlayerTransformComp->SetPosY(23);
		}
			break;
		case 32: //�����̹� ������ �Ѿ� �߻�
			TransformComponent* PlayerTransformComp = m_ECS->GetEntityComponent<TransformComponent>(m_PlayerEntityID);

			EntityID BulletEntityID = m_ECS->CreateBulletEntity();

			TransformComponent* BulletTransform = m_ECS->GetEntityComponent<TransformComponent>(BulletEntityID);
			BulletTransform->SetPosX(PlayerTransformComp->GetPosX() + 5);
			BulletTransform->SetPosY(PlayerTransformComp->GetPosY());
		}
	}
}
