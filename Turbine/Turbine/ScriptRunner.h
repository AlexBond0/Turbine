#pragma once

#include "Sol2//sol.hpp"
#include "MessagePump.h"

class ScriptRunner
{

public:

	ScriptRunner() {};
	~ScriptRunner() {};

	void OnLoad();
	void OnWorldLoad();

	void OnMessage(Message msg);

protected:

private:
};

