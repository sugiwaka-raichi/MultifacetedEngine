#pragma once
#include <string>
#include <vector>

using namespace std;

class StringOperation {
private:
public:
	static vector<wstring> Split(wstring str, wchar_t ragex);		//指定文字分割	wchar_t
	static vector<string> Split(string str, char ragex);		//指定文字分割	char

};

