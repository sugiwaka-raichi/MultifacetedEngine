#include "Script.h"
#include <fstream>

Script* Script::instance = nullptr;

Script::Script() {
	
}

Script::~Script()
{
}

//==================================================================
//文字列の読み込み
//ifstream:ファイルポインタ
//int:読込むバイト数
//==================================================================
wstring Script::ReadString(ifstream * _file, int _readByte)
{
	wstring str;
	//読込むバイト数 / 読込む型のサイズ分ループ
	for (int i = 0; i < _readByte / sizeof(wchar_t); i++) {
		wchar_t tempData = '\0';		//null文字で初期化
		_file->read((char*)&tempData, sizeof(wchar_t));		//ファイル読み込み
		str.push_back(tempData);		//読込んだ文字を文字列型に追加
	}
	return str;		//読込み終えたデータを渡す
}

Script & Script::GetScriptInstance()
{
	if (instance == nullptr) {
		instance = new Script;
	}
	return *instance;
}

void Script::LoadHeader() {
	wstring str;			//一時データ
	ifstream file;			//ファイル
	
	//file.open(fileName, ios::in | ios::binary);
	file.open("SCRIPT.ric", ios::in | ios::binary);
	if (!file) {
		ErrorMessage::ErrorMessageBox(L"スクリプトファイルを開くのに失敗しました。", L"Script FileOpen Error", MB_OK);
		return;
	}

	file.read((char*)&fileLine, sizeof(int));

	seek = file.tellg();
	file.close();
	return;
}

void Script::LoadScript() {
	//ORDER_DATA data;
	//ifstream file;
	//int fileLine = 0;

	LoadScript(seek);		//現在のポイントを読込む
}

void Script::LoadScript(int _seek) {
	ORDER_DATA data;
	ifstream file;
	wstring str;
	
	//file.open(fileName, ios::in | ios::binary);
	file.open("SCRIPT.ric", ios::in | ios::binary);
	if (!file) {
		ErrorMessage::ErrorMessageBox(L"スクリプトファイルを開くのに失敗しました。", L"Script FileOpen Error", MB_OK);
		return;
	}

	int readBytes = 0;
	int tempValue = 0;
	file.seekg(_seek);
	file.read((char*)&data.lines, sizeof(int));		//n行目の命令か
	file.read((char*)&data.byte, sizeof(int));		//命令のバイト数
	file.read(&data.op_code, sizeof(char));			//命令の概要
	readBytes = data.byte - 1;						//読込む内容のバイト数

	switch (data.op_code & 0x0F) {		//下位4bitで判定
	case OP_CODE::LABEL:
		str = ReadString(&file, readBytes);
		label[str] = data.lines;		//ラベル管理
		break;
	case OP_CODE::TEXT:
		str = ReadString(&file, readBytes);
		if (backLog < dialogue.size()) {
			dialogue.erase(dialogue.begin());		//先頭データを消す
		}
		dialogue.push_back(str);		//セリフ一覧に入れる
		break;
	case OP_CODE::FUNC:
		break;
	case OP_CODE::ARG:
		break;
	}

	str.clear();
	seek = file.tellg();
	lastOPCode = data.op_code;		//
	file.close();
	return;
}

wstring Script::GetNowDialogue()
{
	return dialogue.back();
}

char Script::GetLastOperation()
{
	return lastOPCode;
}
