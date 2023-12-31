#pragma once
#include <vector>

class CEmulatorGameBoard;

template<class T = int>
class GameBoardItem;

enum GameObjectType {
	GAMEOBJECT_TYPE_EMPTY,
	GAMEOBJECT_TYPE_GRASS,
	GAMEOBJECT_TYPE_HERBIVORE
};

enum GameObjectColors {
	GAMEOBJECT_COLOR_GREY = RGB(100, 100, 100),
	GAMEOBJECT_COLOR_GREEN = RGB(0, 255, 0),
	GAMEOBJECT_COLOR_YELLOW = RGB(255, 255, 0)
};

struct Coordinate {
	int row, col;

	Coordinate() {}
	Coordinate(int _row, int _col) : row(_row), col(_col) {}
};

class GameObject
{
public:
	GameObject(CEmulatorGameBoard* gameboard, int row, int col);

	virtual void update();
	virtual GameObjectType getType(void);
	virtual GameObjectColors getColor(void);

	Coordinate chooseCell(int radius, GameObjectType type);
	Coordinate getLocation(void);
	bool timeToUpdate(unsigned long long);
	void moveTo(Coordinate coord);

protected:
	CEmulatorGameBoard* gameboard;
	Coordinate location;

private:
	static std::vector<GameBoardItem<GameObject*>*> itemsToChoose;
	unsigned long long lastUpdate = 0;
};
