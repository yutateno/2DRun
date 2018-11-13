#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

enum class MAPID { wall = 8, wallJump, wallDead };

class Map
{
private:
	/// マップ関連
	void ReadMap();

	/// マップデータ本体
	std::vector<std::vector<int>>m_vMapdata;

	/// マップチップのサイズ
	float m_mapchipSize;

public:
	/// コンストラクタ
	Map();
	/// デストラクタ
	~Map();

	/// MapID取得
	std::vector<std::vector<int>>GetMapID();

	/// スプライトサイズ
	float GetSpriteSize();
};