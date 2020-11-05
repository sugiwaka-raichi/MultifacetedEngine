#include "Button.h"
#include "input.h"

bool Button::PushKey(int m_key) {
	if (OnCursor()) {
		switch (m_key) {
		case MOUSE_KEYTYPE::LEFT:
			if (GetMouseLeftPress) {
				return true;
			}
			break;
		case MOUSE_KEYTYPE::RIGHT:
			return true;
			break;
		case MOUSE_KEYTYPE::CENTER:
			return true;
			break;
		case MOUSE_KEYTYPE::FORWARD:
			return true;
			break;
		default:
			break;
		}
	}
	return false;
}

bool Button::OnCursor() {
	float CursorX, CursorY;
	CursorX = GetMouseX();
	CursorY = GetMouseY();
	if (CursorX >= x && CursorX < x + w) {
		if (CursorY >= y && CursorY < y + h) {
			return true;
		}
	}
	return false;
}
