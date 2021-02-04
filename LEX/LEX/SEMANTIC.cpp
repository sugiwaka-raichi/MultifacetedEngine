#include "SEMANTIC.h"
#include <iostream>

template<typename T>
vector<char> ToChar(T data);

void SEMANTIC::Soushajo(vector<ORDER> _in) {
	vector<TOKEN_YUSEN> stack;		//演算子スタック
	vector<ORDER> out;		//出力
	bool argument = false;		//引数かどうか
	//-------------------------------------------------------
	//逆ポーランド記法に置き換える
	//-------------------------------------------------------
	for (int i = 0; i < _in.size(); i++) {
		TOKEN_YUSEN tmp;
		bool stackFlag = false;		//スタック入れるものかどうか

		if (!argument) {
			switch (_in[i].token)
			{
			case ORDER_TOKEN::LABEL:
				//※ラベルは地点記録なので処理なし、他命令は認めず終了
				out.push_back(_in[i]);
				break;
			case ORDER_TOKEN::FUNCTION:
				//優先度無し 出力へ直接投げる
				//tmp.code = OP_CODE::FUNC;
				//tmp.yuusendo = 4;
				out.push_back(_in[i]);
				break;
			case ORDER_TOKEN::ARGUMENT:
				//優先度無し 直前の関数の引数
				out.push_back(_in[i]);
				argument = true;
				break;
			case ORDER_TOKEN::VALUE:
				//優先度無し 出力へ直接プッシュ
				out.push_back(_in[i]);
				break;
			case ORDER_TOKEN::VARIABLE:
				//優先度無し 出力へ直接プッシュ 実行時宣言と参照を行う
				out.push_back(_in[i]);
				break;
			case ORDER_TOKEN::OPERATION:
				switch (_in[i].script.front()) {		//演算子の内容を調べる
				case '=':
					tmp.yuusendo = 1;
					tmp.code = OPERATOR::SUBS << 4;			//代入処理
					break;
				case '+':
					tmp.yuusendo = 2;
					tmp.code = OPERATOR::ADD << 4;			//加算処理
					break;
				case '-':
					tmp.yuusendo = 2;
					tmp.code = OPERATOR::SUBT << 4;
					break;
				case '*':
					tmp.yuusendo = 4;
					tmp.code = OPERATOR::MUL;
					break;
				case '/':
					tmp.yuusendo = 4;
					tmp.code = OPERATOR::DIV;
					break;
				default:
					break;
				}
				tmp.code = tmp.code | OP_CODE::OP;
				tmp.token = _in[i];

				stackFlag = true;

				break;
			case ORDER_TOKEN::STRING:
				out.push_back(_in[i]);
				break;
			case ORDER_TOKEN::PAR:
				tmp.token = _in[i];
				stackFlag = true;
				break;
			case ORDER_TOKEN::END:
				//区切り文字扱い

				//)は特殊処理
				if (_in[i].script == L")") {
					//(が来るまでスタックを取り出す
					for (int j = stack.size() - 1; j >= 0; j--) {
						if (stack[j].token.token != ORDER_TOKEN::PAR) {
							out.push_back(stack[j].token);
							stack.pop_back();
						}
						else {
							stack.pop_back();
							break;
						}
					}
				}
				else {
					continue;
				}
				break;
			default:
				break;
			}
		}
		else {		//引数であれば
			switch (_in[i].token) {
			case ORDER_TOKEN::STRING:
				tmp.code = OP_CODE::TEXT;
				break;
			case ORDER_TOKEN::FUNCTION:
				tmp.code = OP_CODE::FUNC;
				break;
			case ORDER_TOKEN::ARGUMENT:
				//エラー
				break;
			case ORDER_TOKEN::VALUE:
				tmp.code = OP_CODE::VAL;
				break;
			case ORDER_TOKEN::VARIABLE:
				tmp.code = OP_CODE::VAR;
				break;
			case ORDER_TOKEN::OPERATION:
				//演算子以下に分けて処理を行う*/
				if (_in[i].script.size() > 1) {
					//二文字で構成される演算子
					switch (_in[i].script.front()) {
					case '=':
						tmp.code = OPERATOR::EQUAL << 4;
						break;
					case '<':
						tmp.code = OPERATOR::GEQUAL << 4;
						break;
					case '>':
						tmp.code = OPERATOR::SEQUAL << 4;
						break;
					case '!':
						tmp.code = OPERATOR::NOT << 4;
						break;
					default:
						//エラー 許可されていない演算子
						wcout << L"エラー:" << _in[i].script << L"は演算子として許容されていません。" << endl;
						getchar();
						break;
					}
					//次の文字が=かどうか
					if (_in[i].script[1] == '=') {
						//問題なしなのでこれで確定
					}
					else {
						//エラー =以外現言語仕様上ありえない
						wcout << L"エラー:" << _in[i].script << L"は演算子として許容されていません。" << endl;
					}
				}//if_in[i].script.size() > 1)
				else {
					//演算子が一つの場合処理する
					switch (_in[i].script.front())
					{
					case '<':
						tmp.code = OPERATOR::SMALL << 4;
						break;
					case '>':
						tmp.code = OPERATOR::GREATER << 4;
						break;
					default:
						break;
					}
				}
				tmp.code = tmp.code | OP_CODE::OP;			//比較演算子
			case ORDER_TOKEN::END:
				if (_in[i].script == L")") {
					argument = false;
				}
				break;
			}

			out.push_back(_in[i]);
		}

		if (stackFlag) {
			//スタックから取り出す処理
			if (stack.size() > 0 && stack.back().yuusendo >= tmp.yuusendo) {
				//取り出すものがあれば
				for (int j = stack.size() - 1; j >= 0; j--) {
					//()は処理しない
					if (tmp.token.token == ORDER_TOKEN::PAR) {
						break;
					}

					//スタックの優先度より現在の優先度が低いもしくは等しければ
					if (stack[j].yuusendo >= tmp.yuusendo) {
						out.push_back(stack.back().token);
						stack.pop_back();
					}
				}
			}
			if (tmp.token.token != ORDER_TOKEN::END) {
				stack.push_back(tmp);
			}
		}

		//-----------------------------------------------------
		//出力の結果を表示
		//-----------------------------------------------------
#if SYSTEM_MESSAGE >= 2

		wcout << "out:";
		for (int j = 0; j < out.size(); j++) {
			wcout << out[j].script;
		}
		wcout << endl;
		wcout << "stack:";
		for (int j = stack.size() - 1; j >= 0; j--) {
			wcout << stack[j].token.script;
		}
		wcout << endl;
#endif // SYSTEM_MESSAGE

		//wcout << endl;
		//出力をファイルとして書き出す処理を入れていく
	}

	//スタックの残りを入れる
	for (int i = stack.size() - 1; i >= 0; i--) {
		out.push_back(stack[i].token);
		stack.pop_back();
	}

	/*for (int i = 0; i < _in.size(); i++) {
		wcout << _in[i].script;
	}*/
#if SYSTEM_MESSAGE >= 1
	wcout << L"結果:";
	for (int i = 0; i < out.size(); i++) {
		wcout << out[i].script << ' ';
	}
	wcout << endl;
#endif
	result.push_back(out);
}

//================================================
//命令の書出し
//================================================
void SEMANTIC::Compile() {
	vector<char> byte;		//1バイトデータずつ格納する
	//書出し用の内容へ
	for (int i = 0; i < result.size(); i++) {
		for (int j = 0; j < result[i].size(); j++) {
			//i+1がスクリプトファイルの現在読んでいる行
			//バイト変換
			switch (result[i][j].token) {
			case ORDER_TOKEN::LABEL:
				//ラベルであれば

			default:
				break;
			}
		}
	}

	//ファイルの作成
	ofstream file;
	file.open("SCRIPT.ric", ios::out | ios::binary | ios::trunc);		//ファイルの新規作成

	if (!file) {
		cout << "スクリプトファイルが開けません" << endl;
		return;
	}
	byte = ToChar(2);		//テスト
	//for (int i = 0; i < orderList.size(); i++) {
	//	for (int j = 0; j < orderList[i].size(); j++) {
	//		//file.write((char*)&orderList[i], sizeof(char));
	//	}
	//}
	for(int i = 0;i<result.size();i++){
	}
}

void SEMANTIC::SetScript(vector<vector<ORDER>> _order) {
	orderList = _order;
}

void SEMANTIC::Analysis() {
	for (int i = 0; i < orderList.size(); i++) {
		Soushajo(orderList[i]);
	}
	//実行結果をもとに命令データを作成
	Compile();
}

SEMANTIC& SEMANTIC::GetInstance()
{
	static SEMANTIC *instance = new SEMANTIC;
	return *instance;
}

SEMANTIC::SEMANTIC() {

}

//=========================================
//1バイトデータに変換する関数
//=========================================
template<typename T>
vector<char> ToChar(T data) {
	int dataSize = sizeof(data);		//何バイトのデータかを調べる
	vector<char> result;				//バイト分けされたデータ
	//データのバイト数に達するまでループする
	for (int i = 0; i < dataSize; i++) {
		char byteData = data & 0xFF;		//1バイトだけ取り出す
		data = data >> 8;		//処理済みビットは破棄
		//リザルトにデータを格納
		result.push_back(byteData);
	}
	return result;		//未完成
}
