#pragma once
#include <string>
#include <vector>

using namespace std;

class StringOperation {
private:
public:
	static vector<wstring> Split(wstring str, wchar_t ragex);		//�w�蕶������	wchar_t
	static vector<string> Split(string str, char ragex);		//�w�蕶������	char

};

