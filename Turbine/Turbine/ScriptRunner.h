#pragma once

#include "Sol2//sol.hpp"
#include "MessagePump.h"
#include "TurbineUserTypes.h"

class ScriptRunner
{

public:

	ScriptRunner(std::string name);
	~ScriptRunner() {};

	static void SetupTurbine();

	bool AttachScript(char* scriptFile);

	void OnLoad();
	void OnWorldLoad();

	void OnTest();

	void OnMessage(Message msg);

	// static void LoadTurbineDefinitons();

protected:

	const std::string _name;

private:

	static sol::state _lua; // lua state

	sol::table _namespace;

	// static sol::state _jsonConverter;
};

