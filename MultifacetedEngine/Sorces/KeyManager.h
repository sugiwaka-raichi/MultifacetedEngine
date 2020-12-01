#pragma once
#include "game.h"
#include "input.h"
#include <map>
#include <vector>

//========================
//キーの状態管理構造体
//========================
typedef struct {
	BYTE key;		//入力されたキー
	bool press;		//押されている状態
	bool trigger;	//押された
	bool repeat;	//
	bool releases;	//離された
	
}KEY_STATE;

static class KeyManager {
private:
	map<string, vector<KEY_STATE>> keyList;		//処理ごとにキーの状態を保持する辞書
public:
	void KeyManagerUpdate();				//キーの入力状態をすべて取得
	bool GetKeyState(string _process);		//ある処理の入力状態を取得

};
