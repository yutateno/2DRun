#pragma once
#include <string>
#include "Input.hpp"
#include "Map.hpp"


class Character
{
private:
	enum class MOVE_DIRE { walk = 0, fall = 6, jump };

	/// スプライトの描画のためのID
	int m_ID;

	/// スプライトの最大数
	const int m_spriteNum = 8;

	/// 座標
	float m_x, m_y;

	/// スプライトのコマ数を配列用に -1
	const int m_walkFrame = 6;

	/// コマ送り
	void FrameSprite(MOVE_DIRE direction);

	/// 向きの方向
	bool m_direction[4];

	/// コマ送り待機
	int m_frameWait;

	/// コマ送り待機時間
	const int m_frameWaitTimer = 10;

	/// スプライトサイズ
	float m_xSize, m_ySize;

	/// 右向くかどうか
	bool m_rightDire;

	/// スピード
	int speed;					


	/// マップでのあたり判定のため
	Map* map;

	/// ジャンプ関連
	/// 地面に触れてるか
	bool Groundflag;

	/// ジャンプしているか
	bool Jumpflag;

	/// 長押しジャンプか
	bool Longjump;		

	/// ジャンプ力
	float Jumppower;	

	/// 重力
	float Gravitypower;		


public:
	Character();
	~Character();

	void Process();


	int GetID();
	float GetX();
	float GetY();
	float GetXSize();
	float GetYSize();
	bool GetRightDire();
};

