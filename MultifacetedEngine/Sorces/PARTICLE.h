#pragma once
#include "SquarePolygon.h"

class PARTICLE :public SquarePolygon {
private:
public:
	float g;		//重力
	float math;		//質量
	float maxTime;	//再生する時間
	float speed;	//速度
	float dis;		//方向

	void Update();	//更新処理

};

