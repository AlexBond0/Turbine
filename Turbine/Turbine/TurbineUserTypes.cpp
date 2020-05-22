#include "TurbineUserTypes.h"

std::map<std::string, std::string> TurbineUsertypeDefiner::descriptions;


void TurbineUsertypeDefiner::BuildTestOnes(sol::state& luaState) {

	auto turbine = luaState["Turbine"].get_or_create<sol::table>();

	/*turbine.new_usertype<Thing1>(
		"Thing1",
		"a", BUILD_TYPE_REF(Thing1, a),
		"b", &Thing1::b,
		"combineab", &Thing1::combineab,
		"gimmeX", &Thing1::gimmeX
	);
	turbine["Thing1"]["gimmeY"] = &Thing1::gimmeY;

	turbine.new_usertype<Thing2>(
		"Thing2",
		sol::base_classes, sol::bases<Thing1>(),
		"c", &Thing2::c,
		"d", &Thing2::d,
		"combinecd", &Thing2::combinecd,
		"gimmeX", &Thing2::gimmeX
	);
	turbine["Thing2"]["gimmeY"] = &Thing2::gimmeY;*/

	BUILD_NEW_USERTYPE(turbine, Thing1, "TestType class");
	BUILD_USERTYPE_MEMBER(turbine, Thing1, a, "TestType memeber");
	BUILD_USERTYPE_MEMBER(turbine, Thing1, b, "TestType memeber");
	BUILD_USERTYPE_MEMBER(turbine, Thing1, combineab, "TestType function");
	BUILD_USERTYPE_MEMBER(turbine, Thing1, gimmeY, "TestType function");

	BUILD_OVERLOADED_START(turbine, Thing1, gimmeX, "Overloaded type method")
		BUILD_OVERLOADED_MEMBER(Thing1, gimmeX, int()),
		BUILD_OVERLOADED_MEMBER(Thing1, gimmeX, int(int))
	BUILD_OVERLOADED_END();
	

	BUILD_NEW_CHILD_USERTYPE(turbine, Thing2, Thing1, "TestType class");
	BUILD_USERTYPE_MEMBER(turbine, Thing2, c, "TestType memeber");
	BUILD_USERTYPE_MEMBER(turbine, Thing2, d, "TestType memeber");
	BUILD_USERTYPE_MEMBER(turbine, Thing2, combinecd, "TestType function");
	BUILD_USERTYPE_MEMBER(turbine, Thing2, gimmeY, "TestType function");

	BUILD_OVERLOADED_START(turbine, Thing2, gimmeX, "Overloaded type method")
		BUILD_OVERLOADED_MEMBER(Thing2, gimmeX, int()),
		BUILD_OVERLOADED_MEMBER(Thing2, gimmeX, int(int))
	BUILD_OVERLOADED_END();
}

void TurbineUsertypeDefiner::Define(sol::state& luaState) {

	_MoveableOrientation(luaState);
	_MoveablePoint(luaState);
	_Moveable(luaState);
}

void TurbineUsertypeDefiner::_MoveableOrientation(sol::state& luaState) {

	BUILD_NEW_USERTYPE(luaState, MoveableOrientation, "Manages the orientation of an Entity");

	//BUILD_USERTYPE_MEMBER(luaState, MoveableOrientation, SetOrientation, "Set the rotation of the element");
	//BUILD_USERTYPE_MEMBER(luaState, MoveableOrientation, GetOrientationQuat, "Get the rotation of the element");

	//BUILD_USERTYPE_MEMBER(luaState, MoveableOrientation, SetRotation, "TestType function");
	//BUILD_USERTYPE_MEMBER(luaState, MoveableOrientation, SetRotation, "TestType function");
	//BUILD_USERTYPE_MEMBER(luaState, MoveableOrientation, ShiftRotation, "TestType function");
	//BUILD_USERTYPE_MEMBER(luaState, MoveableOrientation, GetRotationQuat, "TestType function");


}

void TurbineUsertypeDefiner::_MoveablePoint(sol::state& luaState) {

}

void TurbineUsertypeDefiner::_Moveable(sol::state& luaState) {


}

void TurbineUsertypeDefiner::_LuaUsertypeBuilder(sol::state& luaState, json data) {

	// luaState.new_usertype<Thing1>("Thing1");
	BUILD_NEW_USERTYPE(luaState, Thing1, "Test thingie class");

	BUILD_USERTYPE_MEMBER(luaState, Thing1, a, "This is a thing");
}