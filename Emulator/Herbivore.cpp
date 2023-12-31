#include "pch.h"
#include "Herbivore.h"
#include "helpers.h"
#include "EmulatorGameBoard.h"

Herbivore::Herbivore(CEmulatorGameBoard* gameboard, int row, int col)
	: GameObject(gameboard, row, col) { }

Herbivore::~Herbivore() { }

GameObjectType Herbivore::getType() { return GAMEOBJECT_TYPE_HERBIVORE; }

GameObjectColors Herbivore::getColor() { return GAMEOBJECT_COLOR_YELLOW; }

void Herbivore::eat(Grass* grass)
{
	eaten++;
	dieCycle--;
	if (dieCycle < 0) dieCycle = 0;

	gameboard->SetBoardItem(new GameObject(gameboard, location.row, location.col));
	gameboard->ReplaceBoardItem(grass, this);
}

void Herbivore::mul()
{
	Coordinate newCoord = chooseCell(1, GAMEOBJECT_TYPE_EMPTY);
	if (!gameboard->isCoordValid(newCoord)) return;

	eaten = 0;
	gameboard->ReplaceBoardItem(
		gameboard->GetBoardSpace(newCoord)->get(),
		new Herbivore(gameboard, newCoord.row, newCoord.col)
	);
}

void Herbivore::die()
{
	Coordinate l = location;
	gameboard->ReplaceBoardItem(this, new GameObject(gameboard, l.row, l.col));
}

bool Herbivore::timeToStay(void)
{
	if (stayCycle < stayTimeout) {
		stayCycle++;
		return true;
	}
	stayCycle = 0;
	return false;
}

bool Herbivore::timeToDie(void)
{
	if (dieCycle >= dieTimeout) {
		return true;
	}
	dieCycle++;
	return false;
}

void Herbivore::update()
{
	if (timeToStay()) return;
	if (timeToDie()) return die();

	Coordinate newCoord;
	GameObject* obj;

	newCoord = chooseCell(1, GAMEOBJECT_TYPE_GRASS);
	if (gameboard->isCoordValid(newCoord)) {
		obj = gameboard->GetBoardSpace(newCoord)->get();
		eat((Grass*)obj);
		if (eaten >= food2mul) mul();
		return;
	}

	newCoord = chooseCell(1, GAMEOBJECT_TYPE_EMPTY);
	if (gameboard->isCoordValid(newCoord)) {
		obj = gameboard->GetBoardSpace(newCoord)->get();
		gameboard->SwapItems(this, obj);
		return;
	}
}
