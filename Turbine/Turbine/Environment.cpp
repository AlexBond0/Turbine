#include "Environment.h"


void Environment::Render(RenderingContext& rcontext) {

	if (fog.active) {

		rcontext.SetLiveShader("fog");
		rcontext.liveShader->SetColor("fog.color", fog.color.rgba);
		rcontext.liveShader->SetFloat("fog.density", fog.density);
		rcontext.liveShader->SetFloat("fog.focus", fog.density);
	}
	else {

		rcontext.SetLiveShader("smooth");
	}
}

json Environment::Serialize() {

	json me;

	json fg;
	fg["active"] = fog.active;
	fg["color"] = fog.color.rgba;
	fg["density"] = fog.density;
	fg["focus"] = fog.focus;

	me["fog"] = fg;

	// pack and send json
	return me;
}

Environment::Environment(json& data) {

	fog.active	= data["fog"]["active"];
	fog.color	= color4(data["fog"]["color"]);
	fog.density = data["fog"]["density"];
	fog.focus	= data["fog"]["focus"];
}