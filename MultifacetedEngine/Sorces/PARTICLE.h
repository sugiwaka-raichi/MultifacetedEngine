#pragma once
#include "SquarePolygon.h"

class PARTICLE :public SquarePolygon {
private:
public:
	float g;		//�d��
	float math;		//����
	float maxTime;	//�Đ����鎞��
	float speed;	//���x
	float dis;		//����

	void Update();	//�X�V����

};

