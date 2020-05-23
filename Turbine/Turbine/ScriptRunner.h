#pragma once

#include "Sol2//sol.hpp"
#include "MessagePump.h"
#include "TurbineUserTypes.h"

class ScriptRunner
{

public:

	ScriptRunner();
	~ScriptRunner() {};

	bool AttachScript(char* scriptFile);

	void OnLoad();
	void OnWorldLoad();

	void OnTest();

	void OnMessage(Message msg);

	// static void LoadTurbineDefinitons();

protected:

private:

	sol::state _lua; // lua state

	// static sol::state _jsonConverter;
};

