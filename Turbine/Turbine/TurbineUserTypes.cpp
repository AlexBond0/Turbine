#include "TurbineUserTypes.h"

std::map<std::string, std::string> TurbineUsertypeDefiner::descriptions;


void TurbineUsertypeDefiner::BuildTestOnes(sol::state& luaState) {

	auto turbine = luaState["Turbine"].get_or_create<sol::table>();

	_SetupGLM(luaState);

	//turbine.new_usertype<Thing1>(
	//	"Thing1",
	//	"a", &Thing1::a,
	//	"b", &Thing1::b,
	//	"combineab", &Thing1::combineab
	//);
	//turbine["Thing1"]["gimmeY"] = &Thing1::gimmeY;

	//turbine.new_usertype<Thing2>(
	//	"Thing2",
	//	sol::base_classes, sol::bases<Thing1>(),
	//	"c", &Thing2::c,
	//	"d", &Thing2::d,
	//	"combinecd", &Thing2::combinecd
	//);
	//turbine["Thing2"]["gimmeY"] = &Thing2::gimmeY;

	/*BUILD_NEW_USERTYPE(turbine, glm::vec3, "glm vector");
	BUILD_USERTYPE_MEMBER(turbine, glm::vec3, x, "x");
	BUILD_USERTYPE_MEMBER(turbine, glm::vec3, y, "y");
	BUILD_USERTYPE_MEMBER(turbine, glm::vec3, z, "z");*/

	// BUILD_NEW_USERTYPE(turbine, Thing1, "TestType class");

	//turbine.new_usertype<Thing1>(
	//	"Thing1",
	//	"a", &Thing1::a,
	//	"b", &Thing1::b,
	//	"slammer", &Thing1::slammer
	//);

	BUILD_NEW_USERTYPE_START(turbine, Thing1, "TestType class")
		BUILD_USERTYPE_VARIABLE(Thing1, a),
		BUILD_USERTYPE_VARIABLE(Thing1, b),
		BUILD_USERTYPE_VARIABLE(Thing1, slammer)
	BUILD_NEW_USERTYPE_END()

	// BUILD_USERTYPE_MEMBER(turbine, Thing1, a, "TestType memeber");
	// BUILD_USERTYPE_MEMBER(turbine, Thing1, b, "TestType memeber");
	// BUILD_USERTYPE_MEMBER(turbine, Thing1, slammer, "Vector thingie");
	BUILD_USERTYPE_METHOD(turbine, Thing1, combineab, "TestType function");
	BUILD_USERTYPE_METHOD(turbine, Thing1, setupVec, "TestType function");
	BUILD_USERTYPE_METHOD(turbine, Thing1, gimmeY, "TestType function");

	BUILD_OVERLOADED_METHOD_START(turbine, Thing1, gimmeX, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(Thing1, gimmeX, int()),
		BUILD_OVERLOADED_METHOD_FORMAT(Thing1, gimmeX, int(int))
	BUILD_OVERLOADED_METHOD_END();
	

	// BUILD_NEW_CHILD_USERTYPE(turbine, Thing2, Thing1, "TestType class");
	// BUILD_USERTYPE_MEMBER(turbine, Thing2, c, "TestType memeber");
	// BUILD_USERTYPE_MEMBER(turbine, Thing2, d, "TestType memeber");

	BUILD_NEW_USERTYPE_START(turbine, Thing2, "TestType class")
		BUILD_USERTYPE_BASE_CLASS(Thing1),
		BUILD_USERTYPE_VARIABLE(Thing2, c),
		BUILD_USERTYPE_VARIABLE(Thing2, d)
	BUILD_NEW_USERTYPE_END()

	BUILD_USERTYPE_METHOD(turbine, Thing2, combinecd, "TestType function");
	BUILD_USERTYPE_METHOD(turbine, Thing2, gimmeY, "TestType function");

	BUILD_OVERLOADED_METHOD_START(turbine, Thing2, gimmeX, "Overloaded type method")
		BUILD_OVERLOADED_METHOD_FORMAT(Thing2, gimmeX, int()),
		BUILD_OVERLOADED_METHOD_FORMAT(Thing2, gimmeX, int(int))
	BUILD_OVERLOADED_METHOD_END();
}

void TurbineUsertypeDefiner::Define(sol::state& luaState) {

	_MoveableOrientation(luaState);
	_MoveablePoint(luaState);
	_Moveable(luaState);
}

void TurbineUsertypeDefiner::_MoveableOrientation(sol::state& luaState) {

	// BUILD_NEW_USERTYPE(luaState, MoveableOrientation, "Manages the orientation of an Entity");

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
	// BUILD_NEW_USERTYPE(luaState, Thing1, "Test thingie class");

	// BUILD_USERTYPE_MEMBER(luaState, Thing1, a, "This is a thing");
}

void TurbineUsertypeDefiner::_SetupGLM(sol::state& luaState) {

	auto glm = luaState["glm"].get_or_create<sol::table>();

	auto mult_overloads = sol::overload(
		[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 * v2; },
		[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 * f; },
		[](float f, const glm::vec3& v1) -> glm::vec3 { return f * v1; }
	);

	glm.new_usertype<glm::vec3>("vec3",
		sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z,
		sol::meta_function::multiplication, mult_overloads
		);
}