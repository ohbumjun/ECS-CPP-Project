#include "InputSystem.h"
#include "TransformComponent.h"
#include "ECS.h"

bool InputSystem::Init()
{
	//플레이어 좌표위치
	// playerX = 0;
	// playerY = 12;

	// Transform 정보를 Mask 에 추가한다
	addComponent<TransformComponent>();

    return true;
}

void InputSystem::updateComponents(float delta)
{
	char pressKey;

	// Player Entity 의 Transform Component 정보 가져오기
	TransformComponent* PlayerTransformComp = m_ECS->GetEntityComponent<TransformComponent>(m_PlayerEntityID);

	//키가 입력되면 잡아준다.
	if (_kbhit())
	{
		pressKey = _getch();

		switch (pressKey)
		{
		case 'w':
		{
			//위쪽방향
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
		case 32: //스페이바 누르면 총알 발사
			TransformComponent* PlayerTransformComp = m_ECS->GetEntityComponent<TransformComponent>(m_PlayerEntityID);

			EntityID BulletEntityID = m_ECS->CreateBulletEntity();

			TransformComponent* BulletTransform = m_ECS->GetEntityComponent<TransformComponent>(BulletEntityID);
			BulletTransform->SetPosX(PlayerTransformComp->GetPosX() + 5);
			BulletTransform->SetPosY(PlayerTransformComp->GetPosY());
		}
	}
}
