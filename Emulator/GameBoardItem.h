#pragma once
#include <mutex>
#include "GameObject.h"

template<class T = int>
class GameBoardItem
{
public:
	GameBoardItem(T _obj) : obj(_obj) {};

	void lock() { mutex.lock(); };
	void unlock() { mutex.unlock(); };

	T get() { return obj; };
	void set(T _obj) { obj = _obj; };
private:
	std::mutex mutex;
	T obj;
};
