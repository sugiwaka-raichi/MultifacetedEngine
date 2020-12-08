#include "OBJECT.h"

void OBJECT::SetLayernum(int _layer) {
	layer = _layer;
}

void OBJECT::SetDispFlag(bool _flag) {
	dispFlag = _flag;
}

int OBJECT::GetLayernum() {
	return layer;
}

bool OBJECT::GetDispFlag()
{
	return dispFlag;
}
