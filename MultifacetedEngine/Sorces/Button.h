#pragma once
#include "SquarePolygon.h"



class Button:public SquarePolygon {
private:
protected:
public:
	bool PushKey(MOUSE_KEYTYPE m_key);
	bool OnCursor();
};

