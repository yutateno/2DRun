#include "Character.hpp"



void Character::FrameSprite(MOVE_DIRE direction)
{
	if (m_frameWait >= m_frameWaitTimer)
	{
		if (m_ID >= static_cast<int>(direction) * m_frame && m_ID <= static_cast<int>(direction) * m_frame + (m_frame - 1))
		{
			if (m_ID >= static_cast<int>(direction) * m_frame + (m_frame - 1))
			{
				m_ID = static_cast<int>(direction) * m_frame;
			}
			else
			{
				m_ID++;
			}
		}
		else
		{
			m_ID = static_cast<int>(direction) * m_frame;
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
	m_ID = static_cast<int>(MOVE_DIRE::right) * m_frame;

	m_x = 0.0f;
	m_y = 0.0f;

	m_frameWait = 0;

	ZeroMemory(m_direction, sizeof(m_direction));
}


Character::~Character()
{
}

void Character::Process()
{
	/// コントローラーによる操作
	if (InputPad::GetPadThumbData(XINPUT_PAD::NUM01, XINPUT_PAD::STICK_LEFT_X) > 0)
	{
		m_direction[static_cast<int>(DIRECTION::right)] = true;
	}
	else
	{
		m_direction[static_cast<int>(DIRECTION::right)] = false;
	}
	if (InputPad::GetPadThumbData(XINPUT_PAD::NUM01, XINPUT_PAD::STICK_LEFT_X) < 0)
	{
		m_direction[static_cast<int>(DIRECTION::left)] = true;
	}
	else
	{
		m_direction[static_cast<int>(DIRECTION::left)] = false;
	}
	if (InputPad::GetPadThumbData(XINPUT_PAD::NUM01, XINPUT_PAD::STICK_LEFT_Y) > 0)
	{
		m_direction[static_cast<int>(DIRECTION::up)] = true;
	}
	else
	{
		m_direction[static_cast<int>(DIRECTION::up)] = false;
	}
	if (InputPad::GetPadThumbData(XINPUT_PAD::NUM01, XINPUT_PAD::STICK_LEFT_Y) < 0)
	{
		m_direction[static_cast<int>(DIRECTION::down)] = true;
	}
	else
	{
		m_direction[static_cast<int>(DIRECTION::down)] = false;
	}


	/// 操作に対する向きと動き
	if (m_direction[static_cast<int>(DIRECTION::left)])
	{
		if (m_direction[static_cast<int>(DIRECTION::down)])
		{
			m_x -= 0.01f / sqrtf(2);
			m_y += 0.01f / sqrtf(2);
			FrameSprite(MOVE_DIRE::leftDown);
		}
		else if (m_direction[static_cast<int>(DIRECTION::up)])
		{
			m_x -= 0.01f / sqrtf(2);
			m_y -= 0.01f / sqrtf(2);
			FrameSprite(MOVE_DIRE::leftUp);
		}
		else
		{
			m_x -= 0.01f;
			FrameSprite(MOVE_DIRE::left);
		}
	}
	if (m_direction[static_cast<int>(DIRECTION::right)])
	{
		if (m_direction[static_cast<int>(DIRECTION::down)])
		{
			m_x += 0.01f / sqrtf(2);
			m_y += 0.01f / sqrtf(2);
			FrameSprite(MOVE_DIRE::rightDown);
		}
		else if (m_direction[static_cast<int>(DIRECTION::up)])
		{
			m_x += 0.01f / sqrtf(2);
			m_y -= 0.01f / sqrtf(2);
			FrameSprite(MOVE_DIRE::rightUp);
		}
		else
		{
			m_x += 0.01f;
			FrameSprite(MOVE_DIRE::right);
		}
	}
	if (!m_direction[static_cast<int>(DIRECTION::right)] && !m_direction[static_cast<int>(DIRECTION::left)])
	{
		if (m_direction[static_cast<int>(DIRECTION::down)])
		{
			m_y += 0.01f;
			FrameSprite(MOVE_DIRE::down);
		}
		if (m_direction[static_cast<int>(DIRECTION::up)])
		{
			m_y -= 0.01f;
			FrameSprite(MOVE_DIRE::up);
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
