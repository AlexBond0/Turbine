#include "ScriptRunner.h"

// sol::state ScriptRunner::_jsonConverter;

ScriptRunner::ScriptRunner() {

	// expose libraries

	//_jsonConverter.open_libraries(sol::lib::math);
	//_jsonConverter.open_libraries(sol::lib::base);
	//_jsonConverter.open_libraries(sol::lib::string);

	_lua.open_libraries(sol::lib::math);
	_lua.open_libraries(sol::lib::base);
	_lua.open_libraries(sol::lib::string);

	// load the JSON converter script
	/*sol::protected_function_result result;
	result = _jsonConverter.script_file(
		"Scripts/JSON.lua",
		[](lua_State*, sol::protected_function_result pfr) {

		return pfr;
	});

	assert(result.valid());
	_jsonConverter["JSON"] = result;*/

	_lua["JSON"] = _lua.script_file("Scripts/JSON.lua");
	// _lua["message"] = _lua.create_table();
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
	// sol::table message;

	if (_lua["JSON"]["decode"].valid()) {

		_lua["rawJson"] = rawJson;
		_lua.script("messageTbl = JSON:decode(rawJson)");
	}

	// std::string gotone = message["hello"];

	// message[3] = "three";
	// message["friend"] = "foe";

	if (_lua["OnMessage"].valid()) {
		
		std::string hehe = _lua["OnMessage"](_lua["messageTbl"]); // send json as string to be reasembled as a lua table
	}
}
