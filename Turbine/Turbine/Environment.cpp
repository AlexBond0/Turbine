#include "Environment.h"


void Environment::Render(RenderingContext& rcontext) {

	if (fog.active) {

		rcontext.liveShader->SetColor("fog.color", fog.color.rgba);
		rcontext.liveShader->SetFloat("fog.density", fog.density);
	}
}