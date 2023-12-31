#pragma once
#include "GameObject.h"
#include "Grass.h";

class Herbivore : public GameObject
{
public:
	Herbivore(CEmulatorGameBoard* gameboard, int row, int col);
	~Herbivore();

	virtual void update();
	virtual GameObjectType getType(void);
	virtual GameObjectColors getColor(void);

private:
	void mul(void);
	void eat(Grass* grass);
	void die(void);

	bool timeToStay(void);
	bool timeToDie(void);

	int stayCycle = 0;
	int dieCycle = 0;
	int eaten = 0;
	const int stayTimeout = 1;
	const int dieTimeout = 5;
	const int food2mul = 15;
};

