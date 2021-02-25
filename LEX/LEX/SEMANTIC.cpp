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
		ORDER_PRIORITY tmp;			//操車場アルゴリズムで入れ替えを行うデータ
		ORDER_DATA orderData;		//出力に直接入れるデータ
		bool stackFlag = false;		//スタック入れるものかどうか
		bool wOperator = false;		//二つ演算子が存在する演算子

		switch (_in[i].token) {
		case ORDER_TOKEN::STRING:
			orderData.code = OP_CODE::TEXT;		//文字列系
			orderData.token = _in[i];
			if (argument > 0) {
				//引数であればセリフであることはない
				orderData.code |= TEXT_TYPE::STRING << 4;		//文字列
			}
			else {
				//水平タブがあればその位置、なければ-1を入れる
				int find = _in[i].script.find(0x09);
				if (find != wstring::npos) {
					//あればセリフ系
					if (find == 0) {
						//先頭であれば名前無し
						orderData.code |= TEXT_TYPE::TEXT_ONLY << 4;
					}
					else {
						//先頭でなければ名前付き
						orderData.code |= TEXT_TYPE::NAME_TEXT << 4;
					}
				}
				else {
					//なければ文字列
					orderData.code |= TEXT_TYPE::STRING << 4;		//文字列
				}
			}
			out.push_back(orderData);
			break;
		case ORDER_TOKEN::FUNCTION:		//関数
			orderData.token = _in[i];			//トークン情報を入れる
			orderData.code = OP_CODE::FUNC;		//命令は関数
			if (i + 1< _in.size()) {
				if (_in[i + 1].token == ORDER_TOKEN::ARGUMENT) {
					orderData.code |= 0x10;
				}
			}
			out.push_back(orderData);			//命令情報を出力に入れる
			break;
		case ORDER_TOKEN::LABEL:		//ラベル
			orderData.token = _in[i];
			orderData.code = OP_CODE::LABEL;
			out.push_back(orderData);
			break;
		case ORDER_TOKEN::VALUE:		//数値
			orderData.code = OP_CODE::VAL;
			orderData.token = _in[i];
			//小数点が含まれるかどうかを調べる
			if (_in[i].script.find(L'.') != wstring::npos) {
				//含まれていれば
				orderData.code |= VAL_TYPE::VAL_FLOAT << 4;		//追記情報に実数であるということを記す
			}
			else {
				//含まれていなければ
				orderData.code |= VAL_TYPE::VAL_INT << 4;		//整数であることを記す
			}
			out.push_back(orderData);
			break;
		case ORDER_TOKEN::GVARIABLE:	//グローバル変数
			orderData.token = _in[i];
			orderData.code = OP_CODE::VAR | (VAR_TYPE::GLOBAL << 4);
			out.push_back(orderData);
			break;
		case ORDER_TOKEN::LVARIABLE:	//ローカル変数
			orderData.token = _in[i];
			orderData.code = OP_CODE::VAR | (VAR_TYPE::LOCAL << 4);
			out.push_back(orderData);
			break;
		case ORDER_TOKEN::OPERATION:	//演算子
			tmp.order.code = orderData.code = OP_CODE::OP;		//演算子であるということ
			tmp.order.token = orderData.token = _in[i];
			//演算子が二つ使われるか
			if (_in[i].script.size() > 1) {
				if (_in[i].script[1] == L'=') {
					//右辺が=であること
					wOperator = true;
				}
				else {
					//違えばエラーとして返す
					wcout << L"ERROR\n" << _in[i].script << L"は演算子として正しくない\n";
					return;
				}
			}
			//詳細な内容
			switch (_in[i].script.front()) {
			case L'+':
				//加算
				tmp.order.code |= OPERATOR::ADD << 4;
				tmp.priority = 2;
				if (wOperator) {
					tmp.order.code |= 0x80;		//後ろに=が付く
					tmp.priority = 1;
				}
				stackFlag = true;
				break;
			case L'-':
				//減算
				tmp.order.code |= OPERATOR::SUBT << 4;
				tmp.priority = 2;
				if (wOperator) {
					tmp.order.code |= 0x80;
					tmp.priority = 1;
				}
				stackFlag = true;
				break;
			case L'*':
				//乗算
				tmp.order.code |= OPERATOR::MUL << 4;
				tmp.priority = 3;
				if (wOperator) {
					tmp.order.code |= 0x80;
					tmp.priority = 1;
				}
				stackFlag = true;
				break;
			case L'/':
				//除算
				tmp.order.code |= OPERATOR::DIV << 4;
				tmp.priority = 3;
				if (wOperator) {
					tmp.order.code |= 0x80;
					tmp.priority = 1;
				}
				stackFlag = true;
				break;
			case L'=':
				//代入
				tmp.order.code |= OPERATOR::SUBS << 4;
				tmp.priority = 1;
				if (wOperator) {
					orderData.code = tmp.order.code | 0x80;
				}
				else {
					stackFlag = true;
				}
				break;
			case L'<':
				//小なり
				orderData.code = OPERATOR::SMALL << 4;
				if (wOperator) {
					orderData.code |= 0x80;
				}
				break;
			case L'>':
				//大なり
				orderData.code = OPERATOR::GREATER << 4;
				if (wOperator) {
					orderData.code |= 0x80;
				}
				break;
			case L'!':
				//否定
				if (wOperator) {
					orderData.code = OPERATOR::NOT << 4;
				}
				else {
					cout << "ERROR\n";
					return;
				}
				break;
			default:
				//エラー
				break;
			}
			if (!stackFlag) {
				//スタックに入れる内容でなければ出力に渡す
				orderData.token = _in[i];
				out.push_back(orderData);
			}
			wOperator = false;
			break;
		case ORDER_TOKEN::ARGUMENT:		//引数位置
			//優先度無し 直前の関数の引数
			orderData.token = _in[i];
			orderData.code = OP_CODE::ARG;
			out.push_back(orderData);
			argument++;
			break;
		case ORDER_TOKEN::PAR:			//()
			tmp.order.token = _in[i];
			stackFlag = true;
			break;
		case ORDER_TOKEN::END:			//終了(引数の終了含む)
			if (argument > 0 && _in[i].script.find(L')') != wstring::npos) {
				orderData.token = _in[i];
				orderData.code = OP_CODE::ARG | 0x10;		//引数終了
				out.push_back(orderData);
				argument--;
			}
			else {
				for (int j = stack.size() - 1; stack[j].order.token.token != ORDER_TOKEN::PAR; j--) {
					if (j >= stack.size()) {
						//error
						wcout << L"()の数が合いません" << endl;
						return;
					}
					out.push_back(stack.back().order);
					stack.pop_back();//(を取り出す
				}
				stack.pop_back();//)を取り出す
			}
			break;
		default:
			break;
		}
/*
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
				for (int j = 0; j < orderData.token.script.size(); j++) {
					//.があれば実数
					if (orderData.token.script[j] == L'.') {
						orderData.code |= VAL_TYPE::FLOAT << 4;		//命令概要のオプション 実数
						break;		//あれば抜ける
					}
					else {
						orderData.code |= VAL_TYPE::INT << 4;		//命令概要のオプション 整数
					}
				}
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
				orderData.code = OP_CODE::TEXT;
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
				orderData.code = OP_CODE::TEXT;
				orderData.token = _in[i];
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::FUNCTION:
				orderData.code = OP_CODE::FUNC;
				orderData.token = _in[i];
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::ARGUMENT:
				argument++;
				orderData.code = OP_CODE::ARG;
				orderData.token = _in[i];
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::VALUE:
				orderData.code = OP_CODE::VAL;
				orderData.token = _in[i];
				for (int j = 0; j < orderData.token.script.size(); j++) {
					//.があれば実数
					if (orderData.token.script[j] == L'.') {
						orderData.code |= VAL_TYPE::FLOAT << 4;		//命令概要のオプション 実数
						break;		//あれば抜ける
					}
					else {
						orderData.code |= VAL_TYPE::INT << 4;		//命令概要のオプション 整数
					}
				}
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::GVARIABLE:
				//out.push_back(_in[i]);
				orderData.code = OP_CODE::VAR | (VAR_TYPE::GLOBAL << 4);
				orderData.token = _in[i];
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::LVARIABLE:
				orderData.code = OP_CODE::VAR | (VAR_TYPE::LOCAL << 4);
				orderData.token = _in[i];
				out.push_back(orderData);
				break;
			case ORDER_TOKEN::OPERATION:
				//演算子以下に分けて処理を行う
				if (_in[i].script.size() > 1) {
					//二文字で構成される演算子
					switch (_in[i].script.front()) {
					case '=':
						orderData.code = OPERATOR::EQUAL << 4;
						break;
					case '<':
						orderData.code = OPERATOR::GEQUAL << 4;
						break;
					case '>':
						orderData.code = OPERATOR::SEQUAL << 4;
						break;
					case '!':
						orderData.code = OPERATOR::NOT << 4;
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
						orderData.code = OPERATOR::SMALL << 4;
						break;
					case '>':
						orderData.code = OPERATOR::GREATER << 4;
						break;
					case '=':
					case '+':
					case '-':
					case '*':
					case '/':
						break;
					default:
						break;
					}
				}
				//tmp.order.code |= OP_CODE::OP;			//演算子
				orderData.code |= OP_CODE::OP;			//演算子
				stackFlag = true;
				break;
			case ORDER_TOKEN::END:
				if (_in[i].script == L")") {
					argument--;		//引数を減らす
					orderData.code = 1 << 4;
					orderData.code |= OP_CODE::ARG;
					orderData.token = _in[i];
					out.push_back(orderData);
				}
				break;
			}

			//out.push_back(_in[i]);
		}
		*/
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
			//)でなければ入れる
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
			wcout << out[j].token.script;
		}
		wcout << endl;
		wcout << "stack:";
		for (int j = stack.size() - 1; j >= 0; j--) {
			wcout << stack[j].order.token.script;
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
	cout << "---------------------------------------------" << endl;
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
		//vector<char> dataSize = ToByte(i + 1);		//記録する行を取り出す
		//byte.insert(byte.end(), dataSize.begin(), dataSize.end());		//行を記録
		for (int j = 0; j < result[i].size(); j++) {
			vector<char> temp;
			vector<char> dataSize = ToByte(i + 1);		//記録する行を取り出す
			byte.insert(byte.end(), dataSize.begin(), dataSize.end());		//行を記録
			//i+1がスクリプトファイルの現在読んでいる行
			//バイト変換
			switch (result[i][j].code & 0x0F) {		//上位4bitは考慮しない
			case OP_CODE::TEXT:
				//文字列
				temp = ToByte(1 + sizeof(wchar_t) * result[i][j].token.script.size());		//命令のバイト数
				byte.insert(byte.end(), temp.begin(), temp.end());
				byte.push_back(result[i][j].code);
				for (int k = 0; k < result[i][j].token.script.size(); k++) {
					temp = ToByte(result[i][j].token.script[k]);			//文字を1バイトデータに分ける
					byte.insert(byte.end(), temp.begin(), temp.end());		//文字データを記録
				}
				break;
			case OP_CODE::VAL:
				//数値
				temp = ToByte(1 + sizeof(int));
				byte.insert(byte.end(), temp.begin(), temp.end());
				byte.push_back(result[i][j].code);		//命令概要書出し
				temp = ToByte(_wtoi(result[i][j].token.script.c_str()));		//数値データに直して1バイト単位に切り分ける
				//for (int k = 0; k < result[i][j].token.script.size(); k++) {
				//	temp = ToByte(result[i][j].token.script[k]);			//文字を1バイトデータに分ける
				//	byte.insert(byte.end(), temp.begin(), temp.end());		//文字データを記録
				//}
				byte.insert(byte.end(), temp.begin(), temp.end());
				break;
			case OP_CODE::VAR:
				//変数
				temp = ToByte(1 + sizeof(wchar_t) * result[i][j].token.script.size());
				byte.insert(byte.end(), temp.begin(), temp.end());
				byte.push_back(result[i][j].code);
				for (int k = 0; k < result[i][j].token.script.size(); k++) {
					temp = ToByte(result[i][j].token.script[k]);			//文字を1バイトデータに分ける
					byte.insert(byte.end(), temp.begin(), temp.end());		//文字データを記録
				}
				break;
			case OP_CODE::FUNC:
				//関数
				temp = ToByte(1 + sizeof(wchar_t) * result[i][j].token.script.size());
				byte.insert(byte.end(), temp.begin(), temp.end());
				byte.push_back(result[i][j].code);
				for (int k = 0; k < result[i][j].token.script.size(); k++) {
					temp = ToByte(result[i][j].token.script[k]);			//文字を1バイトデータに分ける
					byte.insert(byte.end(), temp.begin(), temp.end());		//文字データを記録
				}
				break;
			case OP_CODE::ARG:
				//引数
				temp = ToByte(1);		//引数は引数である識別子があればいい
				byte.insert(byte.end(), temp.begin(), temp.end());
				byte.push_back(result[i][j].code);
				break;
			case OP_CODE::OP:
				//演算子
				temp = ToByte(1);		//演算子は命令概要の1バイトで完結する
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
				break;
			default:
				break;
			}
		}
	}

#if SYSTEM_MESSAGE > 3
	for (int i = 0; i < byte.size(); i++) {
		cout << byte[i] << endl;
	}
#endif
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

	//データサイズをnバイトの範囲で記録
	for (int i = 0; i < dataSize; i++) {	//データサイズ分ループする6
		result.push_back(_data >> (dataSize - 1 - i) * 8);
	}
	reverse(result.begin(), result.end());
	return result;
}
