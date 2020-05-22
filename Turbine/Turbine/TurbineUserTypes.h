#pragma once

#include "Sol2//sol.hpp"
#include "World.h"

class Thing1 {

public:

	int a = 1;
	int b = 2;

	int combineab() { return a + b; }

	virtual int gimmeX() { return 5; }

	virtual int gimmeY() { return 3; }
};

class Thing2 : public Thing1 {

public:

	int c = 4;
	int d = 8;

	int combinecd() { return c + d; }

	int gimmeX() { return 6; }

	int gimmeY() { return Thing1::gimmeY() + 7; }
};

class TurbineUsertypeDefiner {

public:

	static void BuildTestOnes(sol::state& luaState);

	static void Define(sol::state& luaState);

private:

	static void _MoveableOrientation(sol::state& luaState);
	static void _MoveablePoint(sol::state& luaState);
	static void _Moveable(sol::state& luaState);

	static void _LuaUsertypeBuilder(sol::state& luaState, json data);
};
