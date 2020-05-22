#pragma once

#include "Sol2//sol.hpp"
#include "World.h"

// build an inline type decleration
#define BUILD_TYPE_REF(a, b) &a::b

// build an inline type decleration
#define BUILD_MEMBER_ACCESS(object, member) object:member

// Stringify results from other macros
#define BUILD_STR(a) BUILD_STR_X(a)
#define BUILD_STR_X(a) #a

#define BUILD_DESCRIPTION(access, description) \
descriptions[access] = description

// builds a base new userrtype
#define BUILD_NEW_USERTYPE(state, type, description) \
state.new_usertype<type>(#type); \
BUILD_DESCRIPTION(#type, description);

// builds a base new userrtype with a parent
#define BUILD_NEW_CHILD_USERTYPE(state, type, parent, description) \
state.new_usertype<type>(#type, sol::base_classes, sol::bases<parent>()); \
BUILD_DESCRIPTION(#type, description);

// builds a full decleration with description
#define BUILD_USERTYPE_MEMBER(state, type, member, description) \
state[#type][#member] = &type::member; \
BUILD_DESCRIPTION(#member, description);

// Builds the start of an overloaded function decleration
#define BUILD_OVERLOADED_START(state, type, member, description) \
BUILD_DESCRIPTION(#member, description); \
state[#type][#member] =  sol::overload(

// Builds a member of an overloaded function, where the format of the parameters can be defined
#define BUILD_OVERLOADED_MEMBER(type, member, format) \
sol::resolve<format>(&type::member)

// Builds the end of an overloaded function decleration
#define BUILD_OVERLOADED_END() \
);

// doesn't work yet as editor needs some way of knowing types, a little complicated right now
//BUILD_DESCRIPTION(BUILD_STR(BUILD_MEMBER_ACCESS(type, member)), description);


class Thing1 {

public:

	int a = 1;
	int b = 2;

	int combineab() { return a + b; }

	virtual int gimmeX() { return 5; }
	virtual int gimmeX(int p) { return p + 5; }

	virtual int gimmeY() { return 3; }
};

class Thing2 : public Thing1 {

public:

	int c = 4;
	int d = 8;

	int combinecd() { return c + d; }

	int gimmeX() { return 6; }
	int gimmeX(int s) { return 6 + s; }

	int gimmeY() { return Thing1::gimmeY() + 7; }
};

class TurbineUsertypeDefiner {

public:

	static void BuildTestOnes(sol::state& luaState);

	static void Define(sol::state& luaState);

	static std::map<std::string, std::string> descriptions;

private:

	static void _MoveableOrientation(sol::state& luaState);
	static void _MoveablePoint(sol::state& luaState);
	static void _Moveable(sol::state& luaState);

	static void _LuaUsertypeBuilder(sol::state& luaState, json data);

};
