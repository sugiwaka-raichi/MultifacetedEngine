#include "SEMANTIC.h"
#include <iostream>

template<typename T>
vector<char> ToByte(T _data);

void SEMANTIC::Soushajo(vector<ORDER> _in) {
	vector<ORDER_PRIORITY> stack;		//演算子スタック
	vector<ORDER_DATA> out;		//出力
	int argument = 0;		//引数の数
	//-------------------------------------------------------
	//逆ポーランド記法に置き換える
	//-------------------------------------------------------
	for (int i = 0; i < _in.size(); i++) {
		ORDER_PRIORITY tmp;
		ORDER_DATA orderData;
		bool stackFlag = false;		//スタック入れるものかどうか

		if (argument == 0) {
			switch (_in[i].token)
			{
			case ORDER_TOKEN::LABEL:
				//※ラベルは地点記録なので処理なし、他命令は認めず終了
				orderData.token = _in[i];
				orderData.code = OP_CODE::LABEL;
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::FUNCTION:
				//優先度無し 出力へ直接投げる
				//tmp.code = OP_CODE::FUNC;
				//tmp.yuusendo = 4;
				orderData.token = _in[i];
				orderData.code = OP_CODE::FUNC;
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::ARGUMENT:
				//優先度無し 直前の関数の引数
				orderData.token = _in[i];
				orderData.code = OP_CODE::ARG;
				out.push_back(orderData);
				argument++;
				break;
			case ORDER_TOKEN::VALUE:
				//優先度無し 出力へ直接プッシュ
				orderData.token = _in[i];
				orderData.code = OP_CODE::VAL;
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::GVARIABLE:
				orderData.token = _in[i];
				orderData.code = OP_CODE::VAR | (VAR_TYPE::GLOBAL << 4);
				out.push_back(orderData);
				break; 
			case ORDER_TOKEN::LVARIABLE:
				//優先度無し 出力へ直接プッシュ 実行時宣言と参照を行う
				orderData.token = _in[i];
				orderData.code = OP_CODE::VAR | (VAR_TYPE::LOCAL << 4);
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::OPERATION:
				switch (_in[i].script.front()) {		//演算子の内容を調べる
				case '=':
					tmp.priority = 1;
					tmp.order.code = OPERATOR::SUBS << 4;			//代入処理
					break;
				case '+':
					tmp.priority = 2;
					tmp.order.code = OPERATOR::ADD << 4;			//加算処理
					break;
				case '-':
					tmp.priority = 2;
					tmp.order.code = OPERATOR::SUBT << 4;
					break;
				case '*':
					tmp.priority = 4;
					tmp.order.code = OPERATOR::MUL;
					break;
				case '/':
					tmp.priority = 4;
					tmp.order.code = OPERATOR::DIV;
					break;
				default:
					break;
				}
				tmp.order.code = tmp.order.code | OP_CODE::OP;
				tmp.order.token = _in[i];

				stackFlag = true;

				break;
			case ORDER_TOKEN::STRING:
				orderData.token = _in[i];
				orderData.code = OP_CODE::VAR;
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::PAR:
				tmp.order.token = _in[i];
				stackFlag = true;
				break;
			case ORDER_TOKEN::END:
				//区切り文字扱い

				//)は特殊処理
				if (_in[i].script == L")") {
					//(が来るまでスタックを取り出す
					for (int j = stack.size() - 1; j >= 0; j--) {
						if (stack[j].order.token.token != ORDER_TOKEN::PAR) {
							//(でなければ出力へスタックの内容を入れる
							out.push_back(stack[j].order);
							stack.pop_back();
						}
						else {
							//(であれば取り除いて終了
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
				tmp.order.code = OP_CODE::TEXT;
				break;
			case ORDER_TOKEN::FUNCTION:
				tmp.order.code = OP_CODE::FUNC;
				break;
			case ORDER_TOKEN::ARGUMENT:
				argument++;
				break;
			case ORDER_TOKEN::VALUE:
			case ORDER_TOKEN::GVARIABLE:
				//out.push_back(_in[i]);
				break;
			case ORDER_TOKEN::OPERATION:
				//演算子以下に分けて処理を行う*/
				if (_in[i].script.size() > 1) {
					//二文字で構成される演算子
					switch (_in[i].script.front()) {
					case '=':
						tmp.order.code = OPERATOR::EQUAL << 4;
						break;
					case '<':
						tmp.order.code = OPERATOR::GEQUAL << 4;
						break;
					case '>':
						tmp.order.code = OPERATOR::SEQUAL << 4;
						break;
					case '!':
						tmp.order.code = OPERATOR::NOT << 4;
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
						tmp.order.code = OPERATOR::SMALL << 4;
						break;
					case '>':
						tmp.order.code = OPERATOR::GREATER << 4;
						break;
					default:
						break;
					}
				}
				tmp.order.code = tmp.order.code | OP_CODE::OP;			//比較演算子
				break;
			case ORDER_TOKEN::END:
				if (_in[i].script == L")") {
					argument--;		//引数を減らす
				}
				break;
			}

			//out.push_back(_in[i]);
		}

		if (stackFlag) {
			//スタックから取り出す処理
			if (stack.size() > 0 && stack.back().priority >= tmp.priority) {
				//取り出すものがあれば
				for (int j = stack.size() - 1; j >= 0; j--) {
					//()は処理しない
					if (tmp.order.token.token == ORDER_TOKEN::PAR) {
						break;
					}
					if (stack[j].order.token.token == ORDER_TOKEN::PAR) {
						break;
					}

					//スタックの優先度より現在の優先度が低いもしくは等しければ
					if (stack[j].priority >= tmp.priority) {
						out.push_back(stack.back().order);
						stack.pop_back();
					}
				}
			}
			if (tmp.order.token.token != ORDER_TOKEN::END) {
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
		wcout << L"引数:" << argument << endl;
#endif // SYSTEM_MESSAGE

		//wcout << endl;
		//出力をファイルとして書き出す処理を入れていく
	}

	//スタックの残りを入れる
	for (int i = stack.size() - 1; i >= 0; i--) {
		out.push_back(stack[i].order);
		stack.pop_back();
	}

	/*for (int i = 0; i < _in.size(); i++) {
		wcout << _in[i].script;
	}*/
#if SYSTEM_MESSAGE >= 1
	wcout << L"結果:";
	for (int i = 0; i < out.size(); i++) {
		wcout << out[i].token.script << ' ';
	}
	wcout << endl;
#endif
	result.push_back(out);
}

//================================================
//命令の書出し
//================================================
void SEMANTIC::Compile() {
	vector<char> byte;		//書き出す内容を1バイトデータずつ格納する
	//ヘッダー情報を作成
	{
		//ToDo: スクリプトファイル自体の情報
		//スクリプトファイル名の長さ
		//スクリプトファイル名自体
		//※スクリプトファイルの識別子だけあればいいかも?
		//※最適化は余裕出来たころにでも
		//ToDo: end
		vector<char> Lines = ToByte(result.size());		//行数をバイト単位に直したデータ
		byte.insert(byte.end(), Lines.begin(), Lines.end());
	}
	//書出し用の内容へ
	for (int i = 0; i < result.size(); i++) {
		vector<char> dataSize = ToByte(i+1);		//記録する行を取り出す
		byte.insert(byte.end(), dataSize.begin(), dataSize.end());		//行を記録
		for (int j = 0; j < result[i].size(); j++) {
			vector<char> temp;
			//i+1がスクリプトファイルの現在読んでいる行
			//バイト変換
			switch (result[i][j].code & 0x0F) {		//上位4bitは考慮しない
			case OP_CODE::TEXT:
				//文字列
				temp = ToByte(1 + sizeof(wchar_t) * result[i][j].token.script.size());
				byte.insert(byte.end(), temp.begin(), temp.end());
				byte.push_back(result[i][j].code);
				for (int k = 0; k < result[i][j].token.script.size(); k++) {
					temp = ToByte(result[i][j].token.script[k]);			//文字を1バイトデータに分ける
					byte.insert(byte.end(), temp.begin(), temp.end());		//文字データを記録
				}
				break;
			case OP_CODE::VAL:
				//数値
				temp = ToByte(1 + sizeof(wchar_t) * result[i][j].token.script.size());
				byte.insert(byte.end(), temp.begin(), temp.end());
				byte.push_back(result[i][j].code);
				for (int k = 0; k < result[i][j].token.script.size(); k++) {
					temp = ToByte(result[i][j].token.script[k]);			//文字を1バイトデータに分ける
					byte.insert(byte.end(), temp.begin(), temp.end());		//文字データを記録
				}
			case OP_CODE::FUNC:
				//関数
				break;
			case OP_CODE::ARG:
				//引数
				break;
			case OP_CODE::OP:
				//演算子
				byte.push_back(1);		//演算子は命令概要の1バイトで完結する
				byte.insert(byte.end(), temp.begin(), temp.end());		//1命令のデータサイズ
				byte.push_back(result[i][j].code);		//命令概要
				break;
			case OP_CODE::LABEL:
				//ラベルであれば
				//一命令のデータサイズ 命令概要1バイト ラベル名nバイト
				temp = ToByte(1 + sizeof(wchar_t) * result[i][j].token.script.size());
				byte.insert(byte.end(), temp.begin(), temp.end());
				byte.push_back(result[i][j].code);		//命令概要:ラベルを記録
				for (int k = 0; k < result[i][j].token.script.size(); k++) {
					temp = ToByte(result[i][j].token.script[k]);			//文字を1バイトデータに分ける
					byte.insert(byte.end(), temp.begin(), temp.end());		//文字データを記録
				}
			default:
				break;
			}
		}
	}

	for (int i = 0; i < byte.size(); i++) {
		cout << byte[i] << endl;
	}

	//ファイルの作成
	ofstream file;
	file.open("SCRIPT.ric", ios::out | ios::binary | ios::trunc);		//ファイルの新規作成

	if (!file) {
		cout << "スクリプトファイルが開けません" << endl;
		return;
	}
	//for (int i = 0; i < orderList.size(); i++) {
	//	for (int j = 0; j < orderList[i].size(); j++) {
	//		//file.write((char*)&orderList[i], sizeof(char));
	//	}
	//}
	for(int i = 0;i<byte.size();i++){
		file.write(&byte[i] ,sizeof(char));
	}
	file.close();
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
template <typename T>
vector<char> ToByte(T _data) {
	int dataSize = sizeof(_data);			//データのバイト数を調べる
	vector<char> result;					//結果のデータ

	//データサイズを4バイトの範囲で記録
	for (int i = 0; i < dataSize; i++) {	//データサイズ分ループする6
		result.push_back(_data >> (dataSize - 1 - i) * 8);
	}
	return result;
}
