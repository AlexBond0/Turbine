#include "Environment.h"


void Environment::Render(RenderingContext& rcontext) {

	if (fog.active) {

		rcontext.SetLiveShader("fog");
		rcontext.liveShader->SetColor("fog.color", fog.color.rgba);
		rcontext.liveShader->SetFloat("fog.density", fog.density);
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

	me["fog"] = fg;

	// pack and send json
	json ret;
	ret["Environment"] = me;
	return ret;
}

void Environment::Deserialize(json& data) {

}