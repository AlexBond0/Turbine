#include "TurbineUserTypes.h"


void TurbineUsertypeDefiner::BuildTestOnes(sol::state& luaState) {

	luaState.new_usertype<Thing1>(
		"Thing1",
		"a", &Thing1::a,
		"b", &Thing1::b,
		"combineab", &Thing1::combineab,
		"gimmeX", &Thing1::gimmeX,
		"gimmeY", &Thing1::gimmeY
	);

	luaState.new_usertype<Thing2>(
		"Thing2",
		sol::base_classes, sol::bases<Thing1>(),
		"c", &Thing2::c,
		"d", &Thing2::d,
		"combinecd", &Thing2::combinecd,
		"gimmeX", &Thing2::gimmeX,
		"gimmeY", &Thing2::gimmeY
	);
}

void TurbineUsertypeDefiner::Define(sol::state& luaState) {

	_MoveableOrientation(luaState);
	_MoveablePoint(luaState);
	_Moveable(luaState);
}

void TurbineUsertypeDefiner::_MoveableOrientation(sol::state& luaState) {

	luaState.new_usertype<MoveableOrientation>("MoveableOrientation",
		"call", &MoveableOrientation::GetFront
	);
}

void TurbineUsertypeDefiner::_MoveablePoint(sol::state& luaState) {

}

void TurbineUsertypeDefiner::_Moveable(sol::state& luaState) {


}