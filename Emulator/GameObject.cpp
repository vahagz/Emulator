#include "pch.h"
#include "GameObject.h"
#include "helpers.h"
#include "EmulatorGameBoard.h"
#include <iostream>

std::vector<GameBoardItem<GameObject*>*> GameObject::itemsToChoose;

GameObject::GameObject(CEmulatorGameBoard* gb, int row, int col)
{
	location.row = row;
	location.col = col;
	gameboard = gb;
}

GameObjectType GameObject::getType()
{
	return GAMEOBJECT_TYPE_EMPTY;
}

void GameObject::update() { }

bool GameObject::timeToUpdate(unsigned long long t)
{
	if (lastUpdate == t) {
		return false;
	}
	lastUpdate = t;
	return true;
}

GameObjectColors GameObject::getColor()
{
	return GAMEOBJECT_COLOR_GREY;
}

Coordinate GameObject::chooseCell(int radius, GameObjectType type)
{
	if (radius < 0) return Coordinate(-1, -1);

	itemsToChoose.clear();
	Coordinate leftTop(location.row - radius, location.col - radius);
	Coordinate rightBottom(location.row + radius, location.col + radius);

	//leftTop.row	= leftTop.row < 0 ? 0 : leftTop.row;
	//leftTop.col	= leftTop.col < 0 ? 0 : leftTop.col;
	//rightBottom.row = rightBottom.row >= gameboard->GetRows()    ? gameboard->GetRows() - 1    : rightBottom.row;
	//rightBottom.col = rightBottom.col >= gameboard->GetColumns() ? gameboard->GetColumns() - 1 : rightBottom.col;

	GameBoardItem<GameObject*>* item;
	for (int row = leftTop.row; row <= rightBottom.row; row++)
		for (int col = leftTop.col; col <= rightBottom.col; col++) {
			if (col == location.col && row == location.row) continue;

			item = gameboard->GetBoardSpace(row, col);
			if (item->get()->getType() == type) itemsToChoose.push_back(item);
		}

	if (!itemsToChoose.size()) return Coordinate(-1, -1);

	int randomIndex = random(0, itemsToChoose.size() - 1);
	return itemsToChoose[randomIndex]->get()->getLocation();
}

void GameObject::moveTo(Coordinate c)
{
	location = c;
}

Coordinate GameObject::getLocation()
{
	return location;
}
