#include "Button.h"
#include "input.h"

bool Button::OnClick(MOUSE_KEYTYPE m_key) {
	if (OnCursor()) {
		switch (m_key) {
		case MOUSE_KEYTYPE::LEFT:
			if (GetMousePress(MOUSE_KEYTYPE::LEFT)){
				return true;
			}
			break;
		case MOUSE_KEYTYPE::RIGHT:
			if (GetMousePress(MOUSE_KEYTYPE::RIGHT)) {
				return true;
			}
			break;
		case MOUSE_KEYTYPE::CENTER:
			if (GetMousePress(MOUSE_KEYTYPE::CENTER)) {
				return true;
			}
			break;
		case MOUSE_KEYTYPE::FORWARD:
			if (GetMousePress(MOUSE_KEYTYPE::FORWARD)) {
				return true;
			}
			break;
		case MOUSE_KEYTYPE::BACK:
			if (GetMousePress(MOUSE_KEYTYPE::BACK)) {
				return true;
			}
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
	if (CursorX >= x - w / 2 && CursorX < x + w / 2) {
		if (CursorY >= y - h / 2 && CursorY < y + h / 2) {
			return true;
		}
	}
	return false;
}
