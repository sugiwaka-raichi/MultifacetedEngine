#include "ERROR.h"

ErrorMessage::ErrorMessage() {
	//hWnd = GetHandle();
}

int ErrorMessage::ErrorMessageBox(LPCWSTR message,LPCWSTR title,UINT uint) {
	bool sts = MessageBox(GetHandle(), message, title, uint);
	return sts;
}

int ErrorMessage::ErrorMessageBox(wstring message, wstring title, UINT uint) {
	bool sts = MessageBox(GetHandle(), (LPCWSTR)message.c_str(), (LPCWSTR)title.c_str(), uint);
	return sts;
}
