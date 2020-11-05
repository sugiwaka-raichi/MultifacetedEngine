#include "ERROR.h"

ErrorMessage::ErrorMessage() {
	//hWnd = GetHandle();
}

int ErrorMessage::ErrorMessageBox(LPCSTR message,LPCSTR title,UINT uint) {
	bool sts = MessageBox(GetHandle(), message, title, uint);
	return sts;
}

int ErrorMessage::ErrorMessageBox(string message, string title, UINT uint) {
	bool sts = MessageBox(GetHandle(), (LPCSTR)message.c_str(), (LPCSTR)title.c_str(), uint);
	return sts;
}
