#pragma once
#include <vector>
#include <map>
#include <string>
//#include <fstream>
#include "ERROR.h"
#include "OperationsStruct.h"

using namespace std;

//========================================================
//命令の構造体
//========================================================
typedef struct {
	int lines = 0;				//n行目
	int byte = 0;				//命令の総バイト数
	char op_code = 0x00;		//命令内容
}ORDER_DATA;

//==============================================
//ラベル管理用構造体
//==============================================
typedef struct {
	wstring labelName;		//ラベル名
	int	line;				//行
	//wstring fileName;		//ファイル名(仮)
}LABEL_DATA;

//==============================================
//スクリプトを管理するクラス
//==============================================
class Script {
private:
	const int backLog = 10;			//10文バックログとして表示する
	static Script* instance;
	
	int fileLine = 0;
	int seek;						//読込んでいる位置
	map<wstring, int> label;		//ラベル位置の記録
	vector<wstring> dialogue;			//セリフ一覧(現在のセリフ+バックログ)
	wstring fileName = L"SCRIPT.ric";
	char lastOPCode;			//最後の命令内容

	Script();

	wstring ReadString(ifstream* file, int _readByte);
public:
	~Script();
	static Script& GetScriptInstance();
	void LoadHeader();
	void LoadScript();
	void LoadScript(int _seek);
	wstring GetNowDialogue();
	char GetLastOperation();		//最後に取得した命令の概要を渡す関数
};

