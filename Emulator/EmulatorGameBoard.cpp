#pragma once

#include "pch.h"
#include "EmulatorGameBoard.h"
#include "helpers.h"
#include "Grass.h"
#include "Herbivore.h"

#include <chrono>
#include <iostream>

//int GameObjectTypeArr[2] = {
//    GAMEOBJECT_TYPE_EMPTY,
//    GAMEOBJECT_TYPE_GRASS,
//    /*GAMEOBJECT_TYPE_HERBIVORE,
//    GAMEOBJECT_TYPE_PREDATOR*/
//};
//
//COLORREF GameObjectColorArr[4] = {
//    GAMEOBJECT_COLOR_GREY,
//    GAMEOBJECT_COLOR_RED,
//    GAMEOBJECT_COLOR_GREEN,
//    GAMEOBJECT_COLOR_YELLOW
//};

CEmulatorGameBoard::CEmulatorGameBoard(int colCount, int rowCount, int squareHeight, int squareWidth) :
    m_arrBoard(NULL),
    m_nColumns(colCount), m_nRows(rowCount),
    m_nHeight(squareHeight), m_nWidth(squareWidth)
{ }

CEmulatorGameBoard::~CEmulatorGameBoard()
{
    DeleteBoard();
}

void CEmulatorGameBoard::SetupBoard()
{
    if (m_arrBoard) {
        LockBoard();
        DeleteBoard();
    }
    //  Create the board if needed
    else {
        CreateBoard();
        LockBoard();
    }


    GameObject* obj;
    int n;
    for (int row = 0; row < m_nRows; row++) {
        for (int col = 0; col < m_nColumns; col++) {
            n = random(1, 100);
            if (n >= 2 && n < 30) {
                obj = new Grass(this, row, col);
            }
            else if (n >= 1 && n < 2) {
                obj = new Herbivore(this, row, col);
            }
            else {
                obj = new GameObject(this, row, col);
            }

            SetBoardItem(obj);
            std::cout << m_arrBoard[row][col]->get()->getType() << ' ';
        }
        std::cout << std::endl;
    }

    UnlockBoard();
}

void CEmulatorGameBoard::CreateBoard()
{
    //  Create the array of rows
    m_arrBoard = new GameBoardItem<GameObject*> **[m_nRows];

    //  Create each row
    for (int row = 0; row < m_nRows; row++) {
        m_arrBoard[row] = new GameBoardItem<GameObject*>*[m_nColumns];
        for (int col = 0; col < m_nColumns; col++)
            m_arrBoard[row][col] = new GameBoardItem<GameObject*>(NULL);
    }
}

void CEmulatorGameBoard::DeleteBoard()
{
    if (!m_arrBoard) return;

    for (int row = 0; row < m_nRows; row++)
        for (int col = 0; col < m_nColumns; col++)
            delete m_arrBoard[row][col]->get();
}

void CEmulatorGameBoard::LockBoard()
{
    if (!m_arrBoard) return;

    for (int row = 0; row < m_nRows; row++)
        for (int col = 0; col < m_nColumns; col++)
            m_arrBoard[row][col]->lock();
}

void CEmulatorGameBoard::UnlockBoard()
{
    if (!m_arrBoard) return;

    for (int row = 0; row < m_nRows; row++)
        for (int col = 0; col < m_nColumns; col++)
            m_arrBoard[row][col]->unlock();
}

GameBoardItem<GameObject*>* CEmulatorGameBoard::GetBoardSpace(Coordinate coord)
{
    if (coord.row < 0) coord.row += (coord.row / m_nRows + 1) * m_nRows;
    else if (coord.row >= m_nRows) coord.row %= m_nRows;
    if (coord.col < 0) coord.col += (coord.col / m_nColumns + 1) * m_nColumns;
    else if (coord.col >= m_nColumns) coord.col %= m_nColumns;
    //if (coord.row < 0 || coord.row >= m_nRows || coord.col < 0 || coord.col >= m_nColumns)
    //    return NULL;
    return m_arrBoard[coord.row][coord.col];
}

GameBoardItem<GameObject*>* CEmulatorGameBoard::GetBoardSpace(int row, int col)
{
    Coordinate c(row, col);
    return GetBoardSpace(c);
}

void CEmulatorGameBoard::SetBoardItem(GameObject* obj)
{
    Coordinate c = obj->getLocation();
    m_arrBoard[c.row][c.col]->set(obj);
}

void CEmulatorGameBoard::SwapItems(GameObject* obj1, GameObject* obj2)
{
    Coordinate c = obj1->getLocation();
    obj1->moveTo(obj2->getLocation());
    obj2->moveTo(c);
    SetBoardItem(obj1);
    SetBoardItem(obj2);
}

void CEmulatorGameBoard::ReplaceBoardItem(GameObject* obj, GameObject* replace)
{
    Coordinate c = obj->getLocation();
    m_arrBoard[c.row][c.col]->lock();
    delete m_arrBoard[c.row][c.col]->get();
    replace->moveTo(c);
    SetBoardItem(replace);
    m_arrBoard[c.row][c.col]->unlock();
}

COLORREF CEmulatorGameBoard::BackgroundColor()
{
    return GAMEOBJECT_COLOR_GREY;
}

bool CEmulatorGameBoard::isCoordValid(Coordinate coord)
{
    return coord.row >= 0 && coord.col >= 0 &&
        coord.row < m_nRows && coord.col < m_nColumns;
}

void CEmulatorGameBoard::Update()
{
    unsigned long long t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    for (int row = 0; row < m_nRows; row++)
        for (int col = 0; col < m_nColumns; col++)
            if (m_arrBoard[row][col]->get()->timeToUpdate(t))
                m_arrBoard[row][col]->get()->update();
}