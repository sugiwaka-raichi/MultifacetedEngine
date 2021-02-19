#pragma once
#include "Win.h"
class ErrorMessage
{
private:
	HWND hWnd;
public:
	ErrorMessage();
	//~ErrorMessage();
	static int ErrorMessageBox(LPCWSTR message,LPCWSTR title,UINT uint);
	static int ErrorMessageBox(wstring message,wstring title,UINT uint);
};