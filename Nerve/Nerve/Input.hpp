#pragma once
//#ifndef _MY_INPUTPAD_H
//#define _MY_INPUTPAD_H

#include <Windows.h>
#include <math.h>

#include <Xinput.h>

#pragma comment(lib, "xinput.lib")

namespace XINPUT_PAD
{
	// 番号
	const unsigned __int8 NUM01 = 0;
	const unsigned __int8 NUM02 = 1;
	const unsigned __int8 NUM03 = 2;
	const unsigned __int8 NUM04 = 3;


	// ボタン
	const unsigned __int8 BUTTON_UP = 0;
	const unsigned __int8 BUTTON_DOWN = 1;
	const unsigned __int8 BUTTON_LEFT = 2;
	const unsigned __int8 BUTTON_RIGHT = 3;

	const unsigned __int8 BUTTON_START = 4;
	const unsigned __int8 BUTTON_BACK = 5;

	const unsigned __int8 BUTTON_STICK_LEFT = 6;
	const unsigned __int8 BUTTON_STICK_RIGHT = 7;

	const unsigned __int8 BUTTON_A = 12;
	const unsigned __int8 BUTTON_B = 13;
	const unsigned __int8 BUTTON_X = 14;
	const unsigned __int8 BUTTON_Y = 15;


	// トリガ
	const bool TRIGGER_RT = 0;
	const bool TRIGGER_LT = 1;

	const unsigned __int8 SHOULDER_LB = 8;
	const unsigned __int8 SHOULDER_RB = 9;


	// スティック
	const unsigned __int8 STICK_RIGHT_X = 0;
	const unsigned __int8 STICK_RIGHT_Y = 1;
	const unsigned __int8 STICK_LEFT_X = 2;
	const unsigned __int8 STICK_LEFT_Y = 3;


	// バイブレーション
	const unsigned __int16 VIB_MAX = 65535;
}

struct STICK_DEADZONE
{
	// 左スティックの左右
	short LEFT_RIGHT = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	short LEFT_LEFT = -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	// 左スティックの上下
	short LEFT_UP = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	short LEFT_DOWN = -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	// 右スティックの左右
	short RIGHT_RIGHT = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	short RIGHT_LEFT = -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

	// 右スティックの上下
	short RIGHT_UP = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	short RIGHT_DOWN = -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
};

class InputPad
{
private:
	static unsigned __int8 controllerNum;		// 接続している最大の個数
	static bool setControll[4];

	static unsigned __int8 playerPadNum;		// プレイヤーが使うコントローラー

	static int button[4][16];					// wButtonの対応
	static int stick[4][4];						// stickの対応(公式だとthumb)

	static XINPUT_STATE state[4];				// xinputの中身

	static STICK_DEADZONE stickDeadZone;		// スティックのデッドゾーン値

	static XINPUT_VIBRATION vibration;			// バイブレーションの構造体

public:
	InputPad();		// コンストラクタ
	~InputPad();		// デストラクタ

	static void FirstUpdate();		// ゲーム開始前操作更新
	static void EverUpdate();			// ゲーム開始後操作更新

	static void Vibration(unsigned __int8 use_padnum, int time = 0
		, unsigned __int16 rightVib = XINPUT_PAD::VIB_MAX, unsigned __int16 leftVib = XINPUT_PAD::VIB_MAX);		// バイブレーションを行う

	static void VibrationStop(unsigned __int8 use_padnum);						// バーブレーションを止める

	static void SetPlayerPadNum(unsigned __int8 playerPadNum);					// プレイヤーの番号を設定

	static void SetPadDeadZone(short leftPad_right = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, short leftPad_left = -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
		, short leftPad_up = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, short leftPad_down = -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
		, short rightPad_right = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, short rightPad_left = -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
		, short rightPad_up = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, short rightPad_down = -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);		// デッドゾーンの設定値変更  // (ある程度楽したいので一応デフォルト引数乱用)

	static int GetPadNum();																// コントローラの数
	static int GetPadButtonData(unsigned __int8 use_padnum, unsigned __int8 use_button);			// コントローラのボタン操作
	static int GetPadTriggerData(unsigned __int8 use_padnum, bool use_Is_triggerLeft);				// コントローラのトリガー操作
	static int GetPadThumbData(unsigned __int8 use_padnum, unsigned __int8 use_stick);			// コントローラのスティック操作
};

//#endif // !_MY_INPUTPAD_H