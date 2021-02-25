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
	int strSeek = 0;
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
	ORDER_DATA data;		//読込んだ命令情報
	ifstream file;			//ファイル
	wstring str;			//読込んだ文字列
	bool loadFlg = true;	//ロードフラグ
	int strIndex = 0;		//特定文字の位置
	int readBytes = 0;		//1命令の読込むバイト数
	int readLines = 0;		//読込んでいる行
	float tempValue = 0;	//一時数値格納変数

	file.open(fileName, ios::in | ios::binary);
	if (!file) {
		ErrorMessage::ErrorMessageBox(L"スクリプトファイルを開くのに失敗しました。", L"Script FileOpen Error", MB_OK);
		return;
	}

	file.seekg(_seek);		//読込位置を設定
	while (loadFlg) {
		file.read((char*)&data.lines, sizeof(int));		//n行目の命令か
		file.read((char*)&data.byte, sizeof(int));		//命令のバイト数
		file.read(&data.op_code, sizeof(char));			//命令の概要
		readBytes = data.byte - 1;						//読込む内容のバイト数

		if (readLines == 0) {
			readLines = data.lines;				//読込んでいる行
		}
		else if(readLines != data.lines) {		//読んでいた行と一致しないときループから抜ける
			seek = file.tellg();				//読込んでいる位置を取得
			seek -= sizeof(ORDER_DATA) * 8;		//読込んだ情報分位置を戻す
			break;
		}
		
		switch (data.op_code & 0x0F) {		//下位4bitで判定
		case 0:		//0は読み込みエラーまたは読み込み終了している
			loadFlg = false;
			break;
		case OP_CODE::LABEL:
			str = ReadString(&file, readBytes);
			label[str] = file.tellg();		//ラベル管理(シーク位置)
			loadFlg = false;
			break;
		case OP_CODE::TEXT:
			str = ReadString(&file, readBytes);		//文字列の読み込み
			strIndex = str.find(0x09);
			if (strIndex != wstring::npos) {		//TABで改行
				str[strIndex] = L'\n';
			}
			if ((data.op_code & 0xF0) >> 4 != TEXT_TYPE::STRING) {
				if (backLog < dialogue.size()) {
					dialogue.erase(dialogue.begin());		//先頭データを消す
				}
				dialogue.push_back(str);		//セリフ一覧に入れる
				loadFlg = false;
			}
			else {
				arg.push_back(str);		//引数として扱うの
			}
			break;
		case OP_CODE::FUNC:
			str = ReadString(&file, readBytes);		//関数名の取得
			switch (data.op_code & 0xF0) {
			case 0x00:		//引数無
				//無しならそのまま処理スタックに積む
				func.push_back(str);
				break;
			case 0x10:		//引数有
				//有りなら引数の終了が来るまで処理待ち
				
				break;
			}
			break;
		case OP_CODE::ARG:
			switch (data.op_code & 0xF0)
			{
			case 0x00:			//引数の始まり
				break;
			case 0x10:			//引数の終わり
				break;
			}
			break;
		case OP_CODE::VAR:
			//ToDo:LOCALとGLOBAL分ける
			str = ReadString(&file, readBytes);		//変数名の読み込み
			variable[str] = 0;		//変数名の登録と初期化
			break;
		case OP_CODE::VAL:
			//数値取得
			switch ((data.op_code & 0xF0) >> 4) {
			case VAL_TYPE::VAL_INT:
				file.read((char*)&tempValue, sizeof(int));
				break;
			case VAL_TYPE::VAL_FLOAT:
				file.read((char*)&tempValue, sizeof(float));
				break;
			}
			value.push_back(tempValue);
			break;
		case OP_CODE::OP:
			switch ((data.op_code & 0xF0) >> 4)	{
			case OPERATOR::ADD:
			case OPERATOR::SUBT:
			case OPERATOR::MUL:
			case OPERATOR::DIV:
				break;
			}
		}
		lastOPCode = data.op_code;		//
		str.clear();
	}
	seek = file.tellg();
	file.close();
	return;
}

wstring Script::GetNowDialogue()
{
	return dialogue.back();
}

int Script::GetSeek(wstring _label) {
	return label[_label];
}

char Script::GetLastOperation()
{
	return lastOPCode;
}

void Script::SetSeek(int _seek) {
	seek = _seek;
	return;
}
