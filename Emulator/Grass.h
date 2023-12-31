#pragma once
#include "GameObject.h"

class Grass : public GameObject
{
public:
	Grass(CEmulatorGameBoard* gameboard, int row, int col);
	~Grass();
	virtual void update();
	virtual GameObjectType getType(void);
	virtual GameObjectColors getColor(void);

private:
	bool timeToMul();
	void mul(Coordinate coords);

	int cycle;
	const int mulTimeout = 3;
};

