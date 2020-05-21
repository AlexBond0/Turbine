#pragma once

#include "Sol2//sol.hpp"
#include "MessagePump.h"

class ScriptRunner
{

public:

	ScriptRunner();
	~ScriptRunner() {};

	bool AttachScript(char* scriptFile);

	void OnLoad();
	void OnWorldLoad();

	void OnMessage(Message msg);

protected:

private:

	sol::state _lua; // lua state

	// static sol::state _jsonConverter;
};

