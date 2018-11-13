#include "Character.hpp"



void Character::FrameSprite(MOVE_DIRE direction)
{
	if (m_frameWait >= m_frameWaitTimer)
	{
		switch (direction)
		{
		case MOVE_DIRE::walk:
			if (m_ID >= m_walkFrame - 1)
			{
				m_ID = 0;
			}
			else
			{
				m_ID++;
			}
			break;

		case MOVE_DIRE::jump:
			m_ID = static_cast<int>(MOVE_DIRE::jump);
			break;

		case MOVE_DIRE::fall:
			m_ID = static_cast<int>(MOVE_DIRE::fall);
			break;
		}
		m_frameWait = 0;
	}
	else
	{
		m_frameWait++;
	}
}

Character::Character()
{
	m_ID = 0;

	m_x = 500.0f;
	m_y = 500.0f;

	m_frameWait = 0;

	m_xSize = 64.0f;
	m_ySize = 64.0f;

	m_rightDire = true;

	speed = 4;

	map = new Map;

	Groundflag = false;
	Jumpflag = false;
	Longjump = false;
	Jumppower = 10;
	Gravitypower = 0;

	ZeroMemory(m_direction, sizeof(m_direction));
}


Character::~Character()
{
}

void Character::Process()
{
	FrameSprite(MOVE_DIRE::walk);

	// 右
	if (m_rightDire)
	{
		m_x += speed;
		// 埋まったら
		if (map->GetMapID()[static_cast<int>((m_y + 3) / map->GetSpriteSize())][static_cast<int>((m_x + 61) / map->GetSpriteSize())] != 0
			|| map->GetMapID()[static_cast<int>((m_y + 61) / map->GetSpriteSize())][static_cast<int>((m_x + 61) / map->GetSpriteSize())] != 0)
		{
			while (map->GetMapID()[static_cast<int>((m_y + 3) / map->GetSpriteSize())][static_cast<int>((m_x + 61) / map->GetSpriteSize())] != 0
				|| map->GetMapID()[static_cast<int>((m_y + 61) / map->GetSpriteSize())][static_cast<int>((m_x + 61) / map->GetSpriteSize())] != 0)
			{
				m_x -= 1;
			}
		}
	}
	// 左
	else
	{
		m_x -= speed;
		// 埋まったら
		if (map->GetMapID()[static_cast<int>((m_y + 3) / map->GetSpriteSize())][static_cast<int>((m_x + 3) / map->GetSpriteSize())] != 0
			|| map->GetMapID()[static_cast<int>((m_y + 61) / map->GetSpriteSize())][static_cast<int>((m_x + 3) / map->GetSpriteSize())] != 0)
		{
			while (map->GetMapID()[static_cast<int>((m_y + 3) / map->GetSpriteSize())][static_cast<int>((m_x + 3) / map->GetSpriteSize())] != 0
				|| map->GetMapID()[static_cast<int>((m_y + 61) / map->GetSpriteSize())][static_cast<int>((m_x + 3) / map->GetSpriteSize())] != 0)
			{
				m_x += 1;
			}
		}
	}

	// 地面に触れてない(浮いてる
	if (this->Groundflag == false)
	{
		this->Gravitypower += 2;
		m_y += this->Gravitypower;

		// 地面に埋まったら
		if (map->GetMapID()[static_cast<int>((m_y + 61) / map->GetSpriteSize())][static_cast<int>((m_x + 3) / map->GetSpriteSize())] != 0
			|| map->GetMapID()[static_cast<int>((m_y + 61) / map->GetSpriteSize())][static_cast<int>((m_x + 61) / map->GetSpriteSize())] != 0)
		{
			while (map->GetMapID()[static_cast<int>((m_y + 61) / map->GetSpriteSize())][static_cast<int>((m_x + 3) / map->GetSpriteSize())] != 0
				|| map->GetMapID()[static_cast<int>((m_y + 61) / map->GetSpriteSize())][static_cast<int>((m_x + 61) / map->GetSpriteSize())] != 0)
			{
				m_y -= 1;
				this->Gravitypower = 0;
				Jumppower = 10;
				this->Groundflag = true;
				this->Jumpflag = false;
			}
		}
	}

	// 上の端から落ちたら
	if (this->Groundflag == true
		&& map->GetMapID()[static_cast<int>((m_y + 64) / map->GetSpriteSize())][static_cast<int>((m_x + 3) / map->GetSpriteSize())] == 0
		&& map->GetMapID()[static_cast<int>((m_y + 64) / map->GetSpriteSize())][static_cast<int>((m_x + 61) / map->GetSpriteSize())] == 0)
	{
		this->Groundflag = false;
	}

	// 地面にいてジャンプボタン押したら
	if (this->Groundflag == true
		&& InputPad::GetPadButtonData(XINPUT_PAD::NUM01,XINPUT_PAD::BUTTON_A) == 1)
	{
		this->Jumpflag = true;
		this->Longjump = true;
		this->Groundflag = false;
		this->Jumppower = 10;
	}

	// ジャンプ動作していたら
	if (this->Jumpflag == true)
	{
		if (InputPad::GetPadButtonData(XINPUT_PAD::NUM01, XINPUT_PAD::BUTTON_A) == -1)
		{
			this->Longjump = false;
		}
		// 長押ししていたら
		if (this->Longjump == true
			&& InputPad::GetPadButtonData(XINPUT_PAD::NUM01, XINPUT_PAD::BUTTON_A) > 1
			&& this->Jumppower <= 30)
		{
			this->Jumppower += 5;
		}
		m_y -= this->Jumppower;

		// 上に埋まったら
		if (map->GetMapID()[static_cast<int>((m_y + 3) / map->GetSpriteSize())][static_cast<int>((m_x + 3) / map->GetSpriteSize())] != 0
			|| map->GetMapID()[static_cast<int>((m_y + 3) / map->GetSpriteSize())][static_cast<int>((m_x + 61) / map->GetSpriteSize())] != 0)
		{
			while (map->GetMapID()[static_cast<int>((m_y + 3) / map->GetSpriteSize())][static_cast<int>((m_x + 3) / map->GetSpriteSize())] != 0
				|| map->GetMapID()[static_cast<int>((m_y + 3) / map->GetSpriteSize())][static_cast<int>((m_x + 61) / map->GetSpriteSize())] != 0)
			{
				m_y += 1;
				this->Jumpflag = false;
			}
		}
	}
}

int Character::GetID()
{
	return m_ID;
}

float Character::GetX()
{
	return m_x;
}

float Character::GetY()
{
	return m_y;
}

float Character::GetXSize()
{
	return m_xSize;
}

float Character::GetYSize()
{
	return m_ySize;
}

bool Character::GetRightDire()
{
	return m_rightDire;
}
