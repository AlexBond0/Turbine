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
}

// Load a script from a file into the script runner
bool ScriptRunner::AttachScript(std::string scriptFile) {

	// is script a .lua file (not super clean but some protection)
	if (scriptFile.find(".lua") != std::string::npos) {

		std::string scriptName = scriptFile.substr(0, scriptFile.find(".lua"));
		_namespace[scriptName] = _lua.script_file(scriptFile);

		return true;
	}
	else
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

	if (_lua["JSON"]["decode"].valid()) {

		// convert JSON message into Lua table
		std::string rawJson = msg.message.dump();
		_namespace["rawJson"] = rawJson;
		std::string localMessage = _name + "." + "script";
		_lua.script(_name + ".messageTbl = JSON:decode(" + _name + ".rawJson)");

		// pass message to all message receiving functions
		for (std::string& scriptID : _scripts) {

			// run the OnMessage function
			if (_namespace[scriptID]["OnMessage"].valid()) {

				_namespace[scriptID]["OnMessage"]((_namespace["messageTbl"]));
			}
		}
	}
}


void ScriptRunner::OnTest() {

	for(std::string& scriptID : _scripts) {

		// run the OnTest function
		if (_namespace[scriptID]["OnTest"].valid()) {

			Entity* frongle = _namespace[scriptID]["OnTest"]();

			Entity* dog_pointer = _namespace[scriptID]["entity"];
		}
	}
}


void ScriptRunner::_OnEventBuilder(std::string tag) {

	// for each script
	for (std::string& scriptID : _scripts) {

		// run the function
		if (_namespace[scriptID][tag].valid()) {

			_namespace[scriptID][tag]();
		}
	}
}