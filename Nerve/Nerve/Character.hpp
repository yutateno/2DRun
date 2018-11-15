#pragma once
#include <string>
#include "Input.hpp"
#include "Map.hpp"


class Character
{
private:
	enum class MOVE_DIRE { walk = 0, fall = 6, jump, wallJump };

	/// スプライトの描画のためのID
	int m_ID;

	/// スプライトの最大数
	const int m_spriteNum = 8;

	/// 座標
	float m_x, m_y;

	/// 描画での座標
	float m_drawX, m_drawY;

	/// スプライトのコマ数を配列用に -1
	const int m_walkFrame = 6;

	/// コマ送り
	void FrameSprite(MOVE_DIRE direction);

	/// 向きの方向
	bool m_direction[4];

	/// コマ送り待機
	int m_frameWait;

	/// コマ送り待機時間
	const int m_frameWaitTimer = 5;

	/// スプライトサイズ
	float m_xSize, m_ySize;

	/// 右向くかどうか
	bool m_rightDire;

	/// スピード
	int m_speed;					


	/// マップでのあたり判定のため
	Map* m_map;

	/// ジャンプ関連
	/// 地面に触れてるか
	bool m_groundFlag;

	/// ジャンプしているか
	bool m_jumpFlag;

	/// 長押しジャンプか
	bool m_longJump;		

	/// ジャンプ力
	float m_jumpPower;	

	/// 重力
	float m_gravityPower;	

	/// 落ちているかどうか
	float m_fallNow;

	/// 直前のY座標
	float m_preY;

	/// 壁からのジャンプかどうか
	bool m_wallJump;

	/// スクロールの都合で加算するマップ描画のX座標
	float m_mapDrawAddX;


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
	float GetMapDrawAddX();
};

