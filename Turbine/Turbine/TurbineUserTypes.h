#pragma once

#include "Sol2//sol.hpp"
#include "World.h"
#include "TurbineTextEditor.h" // just for containers

// build an inline type decleration
#define BUILD_TYPE_REF(a, b) &a::b

// build an inline type decleration
#define BUILD_MEMBER_ACCESS(object, member) object:member

// Stringify results from other macros
#define BUILD_STR(a) BUILD_STR_X(a)
#define BUILD_STR_X(a) #a

#define BUILD_HOVER_DESCRIPTION(access, description) \
descriptions[access] = description

// ===============================================================

#define BUILD_METHOD_DESCRIPTION(type, member) \
dictionary[#type].methods.push_back(#member)

#define BUILD_VARIABLE_DESCRIPTION(type, member) \
dictionary[#type].variables.push_back(#member)


// ===============================================================

#define BUILD_NEW_USERTYPE(state, type, description) \
BUILD_HOVER_DESCRIPTION(#type, description); \
state.new_usertype<type>(#type);


// builds a base new userrtype
#define BUILD_NEW_USERTYPE_START(state, type, description) \
BUILD_HOVER_DESCRIPTION(#type, description); \
state.new_usertype<type>(#type,

#define BUILD_USERTYPE_VARIABLE(type, member) \
#member, &type::member

#define BUILD_USERTYPE_BASE_CLASS(parent) \
sol::base_classes, sol::bases<parent>()

#define BUILD_NEW_USERTYPE_END() \
); 

// ===============================================================

// builds a full decleration with description
#define BUILD_USERTYPE_METHOD(state, type, member, description) \
BUILD_HOVER_DESCRIPTION(#member, description); \
BUILD_METHOD_DESCRIPTION(type, member); \
state[#type][#member] = &type::member; 

// Builds the start of an overloaded function decleration
#define BUILD_OVERLOADED_METHOD_START(state, type, member, description) \
BUILD_HOVER_DESCRIPTION(#member, description); \
BUILD_METHOD_DESCRIPTION(type, member); \
state[#type][#member] =  sol::overload(

// Builds a member of an overloaded function, where the format of the parameters can be defined
#define BUILD_OVERLOADED_METHOD_FORMAT(type, member, format) \
sol::resolve<format>(&type::member)

// Builds the end of an overloaded function decleration
#define BUILD_OVERLOADED_METHOD_END() \
);

// ===============================================================

#define BUILD_NEW_ENUM_START(state, type) \
state.new_enum(#type,

#define BUILD_ENUM_VALUE(type, tag) \
#tag, type::tag

#define BUILD_NEW_ENUM_END() \
);


class Thing1 {

public:

	Thing1() {};
	Thing1(int p, int q) { a = p; b = q; };

	int a = 1;
	int b = 2;

	glm::vec3 slammer = glm::vec3(5.0f);

	int combineab() { return a + b; }

	virtual int gimmeX() { return 5; }
	virtual int gimmeX(int p) { return p + 5; }

	void setupVec() { slammer = glm::vec3(2.0, 2.0, 2.0); }

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

	static TurbineDictionary dictionary;

private:

	static void _Structs(sol::table& luaState);

	static void _MoveableOrientation(sol::table& luaState);
	static void _MoveablePoint(sol::table& luaState);
	static void _Moveable(sol::table& luaState);

	static void _Material(sol::table& luaState);

	static void _Entity(sol::table& luaState);

	static void _ObjectData(sol::table& luaState);
	static void _Object3D(sol::table& luaState);

	static void _LuaUsertypeBuilder(sol::table& luaState, json data);

	static void _SetupGLM(sol::state& luaState);

	static void MakeDictionary();
};
