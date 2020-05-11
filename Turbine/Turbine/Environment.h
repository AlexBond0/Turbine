#pragma once

#include "RenderingContext.h"
#include "Color.h"

struct EnviromentFog {

	bool active = true;

	color4 color = color4(0.8, 0.8, 0.8, 1.0);
	float density = 0.02;
};

class Environment
{
public:
	Environment() {};
	~Environment() {};

	EnviromentFog fog;


	void Render(RenderingContext& rcontext);
};

