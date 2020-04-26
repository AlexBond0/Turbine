#include "RenderingContext.h"

RenderingContext::RenderingContext() {

	shaders["object"] = new Shader(L"shader.vert", L"shader.frag");
	shaders["object"]->SetupObjectShader();

	shaders["smooth"] = new Shader(L"SmoothLight.vert", L"SmoothLight.frag");
	shaders["smooth"]->SetupObjectShader();

	liveShader = shaders["object"];

	createdShaders = true;
}

RenderingContext::~RenderingContext() {

	if (createdShaders) {

		// delete all saved shaders in the hashmap
		for (auto const& shader : shaders)
			delete shader.second;
	}
}

void RenderingContext::AddShader(std::string name, Shader* shader) {

	shaders[name] = shader;

	if (shaders.size() == 1)
		SetLiveShader(name);
}

void RenderingContext::SetLiveShader(std::string name) {

	liveShader = shaders[name];
	liveShader->Use();
}