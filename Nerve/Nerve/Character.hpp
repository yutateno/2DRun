#pragma once
#include <string>
#include "Input.hpp"

enum class MOVE_DIRE { leftDown = 0, down, rightDown, left, right, leftUp, up, rightUp };
enum class DIRECTION { left = 0, down, right, up };


class Character
{
private:
	/// スプライトの描画のためのID
	int m_ID;

	/// スプライトの最大数
	const int m_spriteNum = 24;

	/// 座標
	float m_x, m_y;

	/// スプライトのコマ数を配列用に -1
	const int m_frame = 3;

	/// コマ送り
	void FrameSprite(MOVE_DIRE direction);

	/// 向きの方向
	bool m_direction[4];

	/// コマ送り待機
	int m_frameWait;
	/// コマ送り待機時間
	const int m_frameWaitTimer = 500;


public:
	Character();
	~Character();

	void Process();


	int GetID();
	float GetX();
	float GetY();
};

