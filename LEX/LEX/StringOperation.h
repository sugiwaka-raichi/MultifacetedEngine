#pragma once
#include <string>
#include <vector>

using namespace std;

class StringOperation {
private:
public:
	static vector<wstring> Split(wstring str, wchar_t ragex);		//Žw’è•¶Žš•ªŠ„	wchar_t
	static vector<string> Split(string str, char ragex);		//Žw’è•¶Žš•ªŠ„	char

};

