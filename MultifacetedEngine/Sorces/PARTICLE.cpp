#include "PARTICLE.h"

void PARTICLE::Update() {
	if (timer < maxTime) {
		timer++;
		DirectionMove(speed, dis);

		//if (0 < GetColor(COLOR::ALPHA)) {
		//	SetColor(COLOR::ALPHA, GetColor(COLOR::ALPHA) - 1);
		//}
		SetColor(D3DCOLOR_RGBA(Rand(), Rand(), Rand(), Rand()));

	}
	else {
		timer = 0;
		SetPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		dis = Rand() % 360;
		speed = Rand() % 3 + 1;
	}

}
