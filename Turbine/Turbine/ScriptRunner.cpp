#include "ScriptRunner.h"


ScriptRunner::ScriptRunner() {

	// expose libraries
	_lua.open_libraries(sol::lib::math);
	_lua.open_libraries(sol::lib::base);
	_lua.open_libraries(sol::lib::string);

	_lua["JSON"] = _lua.script_file("Scripts/JSON.lua");

	TurbineUsertypeDefiner::Define(_lua);
	TurbineUsertypeDefiner::BuildTestOnes(_lua);
}

// Load a script from a file into the script runner
bool ScriptRunner::AttachScript(char* scriptFile) {

	sol::protected_function_result result;

	// run file in the lua state, and return if the script was valid
	result = _lua.script_file(
		scriptFile,
		[](lua_State*, sol::protected_function_result pfr) {

		return pfr;
	});

	return result.valid();
}


// Called immediately when an entity is created in the scene
void ScriptRunner::OnLoad() {

	if (_lua["OnLoad"].valid()) {
		_lua["OnLoad"]();
	}
}

// Called when the full world has been loaded
void ScriptRunner::OnWorldLoad() {

	if (_lua["OnWorldLoad"].valid()) {
		_lua["OnWorldLoad"]();
	}
}


// Called when a message is received by the entity
void ScriptRunner::OnMessage(Message msg) {

	// convert JSON message into Lua table
	std::string rawJson = msg.message.dump();
	if (_lua["JSON"]["decode"].valid()) {

		_lua["rawJson"] = rawJson;
		_lua.script("messageTbl = JSON:decode(rawJson)");
	}

	// run the OnMessage function
	if (_lua["OnMessage"].valid()) {
		
		std::string hehe = _lua["OnMessage"](_lua["messageTbl"]);
	}
}


void ScriptRunner::OnTest() {


	_lua["penguin"] = glm::vec3(3.0f);

	// run the OnTest function
	if (_lua["OnTest"].valid()) {

		std::string hehe = _lua["OnTest"]();

		Entity* dog_pointer = _lua["entity"];

		int x = 1;
	}
}

//void LoadTurbineDefinitons() {
//
//	TurbineUsertypeDefiner::Define(_lua);
//	TurbineUsertypeDefiner::BuildTestOnes(_lua);
//}