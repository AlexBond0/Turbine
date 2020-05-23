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
}

ScriptRunner::ScriptRunner(std::string name)
	: _name(name) {

	_namespace = _lua[_name].get_or_create<sol::table>();

	_namespace["messageTbl"] = "";
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


	_namespace["script"] = _lua.script_file(scriptFile);
	return false;
}


// Called immediately when an entity is created in the scene
void ScriptRunner::OnLoad() {

	if (_namespace["OnLoad"].valid()) {
		_namespace["OnLoad"]();
	}
}

// Called when the full world has been loaded
void ScriptRunner::OnWorldLoad() {

	if (_namespace["OnWorldLoad"].valid()) {
		_namespace["OnWorldLoad"]();
	}
}


// Called when a message is received by the entity
void ScriptRunner::OnMessage(Message msg) {

	// convert JSON message into Lua table
	std::string rawJson = msg.message.dump();
	if (_lua["JSON"]["decode"].valid()) {

		// _lua[_name]["rawJson"] = rawJson;
		_namespace["rawJson"] = rawJson;

		_lua.script(_name+".messageTbl = JSON:decode("+_name+".rawJson)");
	}

	// run the OnMessage function
	if (_namespace["script"].valid()) {
		
		std::string hehe = _namespace["script"](_namespace["messageTbl"]);
	}
}


void ScriptRunner::OnTest() {


	_namespace["penguin"] = glm::vec3(3.0f);

	// run the OnTest function
	if (_namespace["OnTest"].valid()) {

		std::string hehe = _namespace["OnTest"]();

		Entity* dog_pointer = _namespace["entity"];

		int x = 1;
	}
}

//void LoadTurbineDefinitons() {
//
//	TurbineUsertypeDefiner::Define(_lua);
//	TurbineUsertypeDefiner::BuildTestOnes(_lua);
//}