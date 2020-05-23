#include "ScriptRunner.h"

sol::state ScriptRunner::_lua;

void ScriptRunner::SetupTurbine() {

	// expose libraries
	_lua.open_libraries(sol::lib::math);
	_lua.open_libraries(sol::lib::base);
	_lua.open_libraries(sol::lib::string);

	_lua["JSON"] = _lua.script_file("Scripts/JSON.lua");

	TurbineUsertypeDefiner::Define(_lua);
	TurbineUsertypeDefiner::BuildTestOnes(_lua);

	_lua["DecodeJson"] = _lua.script("function DecodeJson (json) return JSON:decode(json) end return DecodeJson");
}

ScriptRunner::ScriptRunner(std::string name)
	: _name(name) {

	_namespace = _lua[_name].get_or_create<sol::table>();

}

// Load a script from a file into the script runner
bool ScriptRunner::AttachScript(char* scriptFile) {

	//sol::protected_function_result result;
	//sol::state test;

	//// run file in the test lua state, and return if the script was valid
	//result = test.script_file(
	//	scriptFile,
	//	[](lua_State*, sol::protected_function_result pfr) {

	//	return pfr;
	//});

	//// load the script into the static lua state
	//if (result.valid()) {

	//	_namespace["script"] = _lua.script_file(scriptFile);
	//	return true;
	//}

	//else

	//_lua[_name][scriptsLoaded].get_or_create<sol::table>();
	//scriptsLoaded++;

	_namespace[scriptsLoaded] = _lua.script_file(scriptFile);
	scriptsLoaded++;
	return false;
}


// Called immediately when an entity is created in the scene
void ScriptRunner::OnLoad() {

	_OnEventBuilder("OnLoad");
}

// Called when the full world has been loaded
void ScriptRunner::OnWorldLoad() {

	_OnEventBuilder("OnWorldLoad");
}


// Called when a message is received by the entity
void ScriptRunner::OnMessage(Message msg) {

	// convert JSON message into Lua table
	std::string rawJson = msg.message.dump();
	if (_lua["JSON"]["decode"].valid()) {

		// _lua[_name]["rawJson"] = rawJson;
		_namespace["rawJson"] = rawJson;

		std::string localMessage = _name + "." + "script";

		_lua.script(_name +".messageTbl = JSON:decode("+ _name +".rawJson)");
		// _namespace["messageTbl"] = _lua["DecodeJson"](_namespace["rawJson"]);
	}

	for (int scriptID = 0; scriptID < scriptsLoaded; scriptID++) {

		// run the OnMessage function
		if (_namespace[scriptID]["OnMessage"].valid()) {

			// std::string hehe = _namespace["script"]["OnMessage"]("Echo"); //(_namespace["script"]["messageTbl"]);
			std::string hehe = _namespace[scriptID]["OnMessage"]((_namespace["messageTbl"]));
		}
	}
}


void ScriptRunner::OnTest() {

	for (int scriptID = 0; scriptID < scriptsLoaded; scriptID++) {

		// run the OnTest function
		if (_namespace[scriptID]["OnTest"].valid()) {

			Entity* frongle = _namespace[scriptID]["OnTest"]();

			Entity* dog_pointer = _namespace[scriptID]["entity"];
		}
	}
}

void ScriptRunner::_OnEventBuilder(std::string tag) {

	for (int scriptID = 0; scriptID < scriptsLoaded; scriptID++) {

		// run the OnMessage function
		if (_namespace[scriptID][tag].valid()) {

			_namespace[scriptID][tag]();
		}
	}
}