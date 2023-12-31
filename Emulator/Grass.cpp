#include "pch.h"
#include "Grass.h"
#include "helpers.h"
#include "EmulatorGameBoard.h"

Grass::Grass(CEmulatorGameBoard* gameboard, int row, int col)
	: GameObject(gameboard, row, col), cycle(0) { }

Grass::~Grass() { }

GameObjectType Grass::getType() { return GAMEOBJECT_TYPE_GRASS; }

GameObjectColors Grass::getColor() { return GAMEOBJECT_COLOR_GREEN; }

bool Grass::timeToMul()
{
	if (cycle < mulTimeout) {
		cycle++;
		return false;
	}
	cycle = 0;
	return true;
}

void Grass::mul(Coordinate coords)
{
	GameObject* obj = gameboard->GetBoardSpace(coords)->get();
	gameboard->ReplaceBoardItem(obj, new Grass(gameboard, coords.row, coords.col));
}

void Grass::update()
{
	if (!timeToMul()) return;

	Coordinate newCoord = chooseCell(1, GAMEOBJECT_TYPE_EMPTY);
	if (!gameboard->isCoordValid(newCoord)) return;
	mul(newCoord);
}
