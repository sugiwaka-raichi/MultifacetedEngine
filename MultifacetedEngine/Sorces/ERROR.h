#pragma once
#include "Win.h"
class ErrorMessage
{
private:
	HWND hWnd;
public:
	ErrorMessage();
	//~ErrorMessage();
	static int ErrorMessageBox(LPCSTR message,LPCSTR title,UINT uint);
	static int ErrorMessageBox(string message,string title,UINT uint);
};