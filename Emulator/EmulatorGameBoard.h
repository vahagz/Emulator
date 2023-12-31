#pragma once
#include "GameObject.h"
#include "GameBoardItem.h"


class CEmulatorGameBoard
{
public:
	CEmulatorGameBoard(
		int colCount, int rowCount,
		int squareHeight, int squareWidth
	);
	~CEmulatorGameBoard(void);
	GameBoardItem<GameObject*>* GetBoardSpace(Coordinate coord);
	GameBoardItem<GameObject*>* GetBoardSpace(int row, int col);
	void SetBoardItem(GameObject* obj);
	void ReplaceBoardItem(GameObject* obj, GameObject* replace);
	void SwapItems(GameObject* obj1, GameObject* obj2);
	COLORREF BackgroundColor();
	void SetupBoard(void);
	void Update(void);
	void DeleteBoard(void);
	bool isCoordValid(Coordinate coord);

	int GetWidth(void) const { return m_nWidth; }
	int GetHeight(void) const { return m_nHeight; }
	int GetColumns(void) const { return m_nColumns; }
	int GetRows(void) const { return m_nRows; }

	void Start(void) { pausedState.unlock(); }
	void Stop(void) { pausedState.lock(); }
	void WaitTillResume(void) { pausedState.lock(); }

private:
	void CreateBoard(void);
	void LockBoard(void);
	void UnlockBoard(void);
	GameBoardItem<GameObject*>*** m_arrBoard = NULL;
	std::mutex pausedState;
	int m_nColumns;
	int m_nRows;
	int m_nHeight;
	int m_nWidth;
};
