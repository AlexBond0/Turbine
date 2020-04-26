#pragma once
#include <assert.h>
#include "Matrix.h"
#include "Shader.h"
#include "TranslationStack.h"

// Type of render currently being preformed
enum class RenderPass {
	OBJECT,	// solid objects rendering
	BLEND	// transparent objects rendering
};

class RenderingContext
 : public TranslationStack {

public:

	RenderingContext();
	~RenderingContext();

	Shader* liveShader;

	void AddShader(std::string name, Shader* shader);
	void SetLiveShader(std::string name);

	bool createdShaders = false;	// has the renderingcontext generated valid openGL shanders & handles
	RenderPass renderPass;			// current type of rendering pass being preformed

private: 

	std::map<std::string, Shader*> shaders;
};