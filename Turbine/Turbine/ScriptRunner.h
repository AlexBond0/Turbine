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

	bool AttachScript(std::string scriptFile);

	void OnLoad();
	void OnWorldLoad();

	void OnTest();

	void OnMessage(Message msg);

protected:

	const std::string _name;

private:

	static sol::state _lua;		// global lua state
	sol::table _namespace;		// local namespace for ScriptRunner

	std::vector<std::string> _scripts; // scripts loaded

	void _OnEventBuilder(std::string tag);
};

