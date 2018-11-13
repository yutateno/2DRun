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
	m_preY = m_y;

	m_frameWait = 0;

	m_xSize = 64.0f;
	m_ySize = 64.0f;

	m_rightDire = true;

	m_speed = 4;

	m_map = new Map;

	m_groundFlag = false;
	m_jumpFlag = false;
	m_longJump = false;
	m_jumpPower = 10;
	m_gravityPower = 0;
	m_fallNow = false;

	ZeroMemory(m_direction, sizeof(m_direction));
}


Character::~Character()
{
}

void Character::Process()
{
	FrameSprite(MOVE_DIRE::walk);

	m_preY = m_y;

	// 右
	if (m_rightDire)
	{
		m_x += m_speed;
		// 埋まったら
		if (m_map->GetMapID()[static_cast<int>((m_y + 3) / m_map->GetSpriteSize())][static_cast<int>((m_x + 61) / m_map->GetSpriteSize())] != 0
			|| m_map->GetMapID()[static_cast<int>((m_y + 61) / m_map->GetSpriteSize())][static_cast<int>((m_x + 61) / m_map->GetSpriteSize())] != 0)
		{
			while (m_map->GetMapID()[static_cast<int>((m_y + 3) / m_map->GetSpriteSize())][static_cast<int>((m_x + 61) / m_map->GetSpriteSize())] != 0
				|| m_map->GetMapID()[static_cast<int>((m_y + 61) / m_map->GetSpriteSize())][static_cast<int>((m_x + 61) / m_map->GetSpriteSize())] != 0)
			{
				m_x -= 1;
			}
		}
	}
	// 左
	else
	{
		m_x -= m_speed;
		// 埋まったら
		if (m_map->GetMapID()[static_cast<int>((m_y + 3) / m_map->GetSpriteSize())][static_cast<int>((m_x + 3) / m_map->GetSpriteSize())] != 0
			|| m_map->GetMapID()[static_cast<int>((m_y + 61) / m_map->GetSpriteSize())][static_cast<int>((m_x + 3) / m_map->GetSpriteSize())] != 0)
		{
			while (m_map->GetMapID()[static_cast<int>((m_y + 3) / m_map->GetSpriteSize())][static_cast<int>((m_x + 3) / m_map->GetSpriteSize())] != 0
				|| m_map->GetMapID()[static_cast<int>((m_y + 61) / m_map->GetSpriteSize())][static_cast<int>((m_x + 3) / m_map->GetSpriteSize())] != 0)
			{
				m_x += 1;
			}
		}
	}

	// 地面に触れてない(浮いてる
	if (!m_groundFlag)
	{
		// 落ちている最中に動いている方向の壁が壁ジャン出来るやつだったら
		if (m_fallNow
			&&((m_rightDire
				&& m_map->GetMapID()[static_cast<int>((m_y + 3) / m_map->GetSpriteSize())][static_cast<int>((m_x + 64) / m_map->GetSpriteSize())] == 2
				|| m_map->GetMapID()[static_cast<int>((m_y + 61) / m_map->GetSpriteSize())][static_cast<int>((m_x + 64) / m_map->GetSpriteSize())] == 2)
				|| (!m_rightDire
					&& m_map->GetMapID()[static_cast<int>((m_y + 3) / m_map->GetSpriteSize())][static_cast<int>((m_x) / m_map->GetSpriteSize())] == 2
					|| m_map->GetMapID()[static_cast<int>((m_y + 61) / m_map->GetSpriteSize())][static_cast<int>((m_x) / m_map->GetSpriteSize())] == 2)))
		{
			m_gravityPower += 0.1f;

			if (InputPad::GetPadButtonData(XINPUT_PAD::NUM01, XINPUT_PAD::BUTTON_A) == 1)
			{
				if (m_rightDire)
				{
					m_x -= 1;
				}
				else
				{
					m_x += 1;
				}
				m_rightDire = !m_rightDire;
				m_gravityPower = 0;
				m_jumpFlag = true;
				m_longJump = true;
				m_groundFlag = false;
				m_fallNow = false;
				m_jumpPower = 10;
			}
		}
		else
		{
			m_gravityPower += 2;
		}
		m_y += m_gravityPower;

		// 地面に埋まったら
		if (m_map->GetMapID()[static_cast<int>((m_y + 61) / m_map->GetSpriteSize())][static_cast<int>((m_x + 3) / m_map->GetSpriteSize())] != 0
			|| m_map->GetMapID()[static_cast<int>((m_y + 61) / m_map->GetSpriteSize())][static_cast<int>((m_x + 61) / m_map->GetSpriteSize())] != 0)
		{
			while (m_map->GetMapID()[static_cast<int>((m_y + 61) / m_map->GetSpriteSize())][static_cast<int>((m_x + 3) / m_map->GetSpriteSize())] != 0
				|| m_map->GetMapID()[static_cast<int>((m_y + 61) / m_map->GetSpriteSize())][static_cast<int>((m_x + 61) / m_map->GetSpriteSize())] != 0)
			{
				m_y -= 1;
				m_fallNow = false;
				m_gravityPower = 0;
				m_jumpPower = 10;
				m_groundFlag = true;
				m_jumpFlag = false;
			}
		}
	}

	// 上の端から落ちたら
	if (m_groundFlag
		&& m_map->GetMapID()[static_cast<int>((m_y + 64) / m_map->GetSpriteSize())][static_cast<int>((m_x + 3) / m_map->GetSpriteSize())] == 0
		&& m_map->GetMapID()[static_cast<int>((m_y + 64) / m_map->GetSpriteSize())][static_cast<int>((m_x + 61) / m_map->GetSpriteSize())] == 0)
	{
		m_groundFlag = false;
		m_fallNow = true;
	}

	// 地面にいてジャンプボタン押したら
	if (m_groundFlag && InputPad::GetPadButtonData(XINPUT_PAD::NUM01,XINPUT_PAD::BUTTON_A) == 1)
	{
		m_jumpFlag = true;
		m_longJump = true;
		m_groundFlag = false;
		m_fallNow = false;
		m_jumpPower = 10;
	}

	// ジャンプ動作していたら
	if (m_jumpFlag)
	{
		if (InputPad::GetPadButtonData(XINPUT_PAD::NUM01, XINPUT_PAD::BUTTON_A) == -1)
		{
			m_longJump = false;
		}
		// 長押ししていたら
		if (m_longJump
			&& InputPad::GetPadButtonData(XINPUT_PAD::NUM01, XINPUT_PAD::BUTTON_A) > 1
			&& m_jumpPower <= 30)
		{
			m_jumpPower += 5;
		}
		m_y -= m_jumpPower;

		// 上に埋まったら
		if (m_map->GetMapID()[static_cast<int>((m_y + 3) / m_map->GetSpriteSize())][static_cast<int>((m_x + 3) / m_map->GetSpriteSize())] != 0
			|| m_map->GetMapID()[static_cast<int>((m_y + 3) / m_map->GetSpriteSize())][static_cast<int>((m_x + 61) / m_map->GetSpriteSize())] != 0)
		{
			while (m_map->GetMapID()[static_cast<int>((m_y + 3) / m_map->GetSpriteSize())][static_cast<int>((m_x + 3) / m_map->GetSpriteSize())] != 0
				|| m_map->GetMapID()[static_cast<int>((m_y + 3) / m_map->GetSpriteSize())][static_cast<int>((m_x + 61) / m_map->GetSpriteSize())] != 0)
			{
				m_y += 1;
				m_fallNow = true;
				m_jumpFlag = false;
			}
		}
	}

	// 直前のY座標が今のY座標より上だったら
	if (m_preY < m_y)
	{
		m_fallNow = true;
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
