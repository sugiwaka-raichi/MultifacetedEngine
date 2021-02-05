#include "StringOperation.h"

//指定文字分割
vector<wstring> StringOperation::Split(wstring str, wchar_t delimiter) {
	//find関数を用いて処理を行う
	int first = 0;		//検索している文字列の先頭
	int last = str.find_first_of(delimiter);	//区切り文字が出た位置

	vector<wstring> result;		//処理結果
	
	if (last != wstring::npos) {
		//firstが文字列よりも小さい間ループ
		while (first < str.size()) {
			wstring subStr(str, first, last - first);
			result.push_back(subStr);
			first = last + 1;		//指定文字の次の文字を指定
			last = str.find_first_of(delimiter, first);		//次の文字位置からみて次の区切り文字の位置を返す

			//区切り文字が見つからなければ
			if (last == wstring::npos) {
				last = str.size();		//最後の文字位置へ
			}
		}
	}
	else {
		result.push_back(str);		//区切り文字がないので結果に入れて返す
	}
	return result;		//取得した結果を返す
}

//指定文字分割
vector<string> StringOperation::Split(string str, char delimiter) {
	//find関数を用いて処理を行う
	int first = 0;		//検索している文字列の先頭
	int last = str.find_first_of(delimiter);	//区切り文字が出た位置

	vector<string> result;		//処理結果

	//firstが文字列よりも小さい間ループ
	while (first < str.size()) {
		string subStr(str, first, last - first);
		result.push_back(subStr);
		first = last + 1;		//指定文字の次の文字を指定
		last = str.find_first_of(delimiter, first);		//次の文字位置からみて次の区切り文字の位置を返す

		//区切り文字が見つからなければ
		if (last == string::npos) {
			last = str.size();		//最後の文字位置へ
		}
	}
	return result;		//取得した結果を返す
}