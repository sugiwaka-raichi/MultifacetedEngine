#pragma once
#include <vector>
#include "SquarePolygon.h"

class Manager {
private:
	vector<SquarePolygon*> PolygonList;		//ƒ|ƒŠƒSƒ“ƒŠƒXƒg

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();
	void CreatePolygon();
	void CreatePolygon(float get_x, float get_y, float get_w, float get_h);
	void CreatePolygon(float get_x, float get_y, float get_w, float get_h, float get_angle);
};

