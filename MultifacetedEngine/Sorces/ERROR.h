#pragma once
#include "Win.h"
class ErrorMessage
{
private:
	HWND hWnd;
public:
	ErrorMessage();
	//~ErrorMessage();
	static void ErrorMessageBox(LPCSTR message,LPCSTR title,UINT uint);
};