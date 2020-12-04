#pragma once
#include "SquarePolygon.h"



class Button:public SquarePolygon {
private:
protected:
public:
	bool OnClick(MOUSE_KEYTYPE m_key);
	bool OnCursor();
};

