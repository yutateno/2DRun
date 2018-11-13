#include "Map.hpp"

using namespace std;


// コンストラクタ
Map::Map()
{
	m_mapchipSize = 64.0f;

	ReadMap();
}

// デストラクタ
Map::~Map()
{
	m_vMapdata.clear();
	m_vMapdata.shrink_to_fit();
}


// 読み込み
void Map::ReadMap()
{
	int L_Type;						// 数字取得用
	int L_Count;					// 読み込み用（何行目を読み込んでいるか
	string L_Line;					// 読み込んだ行（1行）
	ifstream L_File;				// 読み込むファイル

	L_Count = 0;

	L_File.open("media\\map\\Map.txt");		// ファイルオープン

	if (L_File.fail()) {	// ファイル読み込み
	}
	else {
		// 読み込み成功
		while (getline(L_File, L_Line)) {	// 一行ずつ読み込み

			m_vMapdata.resize(L_Count + 1);
			for (int i = 0, n = L_Line.length(); i < n; i++) {
				// 数値取得
				L_Type = stoi(L_Line.substr(i, 1));	// 1文字ずつ取得
				// MapChipの実体作成
				m_vMapdata[L_Count].push_back(L_Type);
			}
			L_Count++;	// 次の行に
		}
	}
	// ファイルを閉じる
	L_File.close();
}


vector<vector<int>> Map::GetMapID()
{
	return m_vMapdata;
}

float Map::GetSpriteSize()
{
	return m_mapchipSize;
}
