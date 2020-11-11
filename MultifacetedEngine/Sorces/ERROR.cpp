#include "ERROR.h"

ErrorMessage::ErrorMessage() {
	hWnd = GetHandle();
}

void ErrorMessage::ErrorMessageBox(LPCSTR message,LPCSTR title,UINT uint) {
	MessageBox(GetHandle(), message, title, uint);
	exit(1);		//ã≠êßèIóπ
}