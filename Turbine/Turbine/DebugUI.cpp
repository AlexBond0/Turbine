#include "DebugUI.h"



DebugUI::DebugUI(const char* glsl_version, GLFWwindow* newWindow) {

	window = newWindow;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();

	// log ImGui version
	std::string versionStr(IMGUI_VERSION);
	std::string message = "\nImGui Version : " + versionStr;
	OutputDebugStringA(message.c_str());

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
}


DebugUI::~DebugUI() {

	for (DebugUIContainer* component : components) {

		if (component)
			delete component;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

// add generated DebugUIContainers, DebugUI will cleanup the components on deletion
void DebugUI::AddComponent(DebugUIContainer* newComponent) {

	components.push_back(newComponent);
}

// render the stored components
void DebugUI::Render() {

	// create new frames for UI to render to
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	for (DebugUIContainer* component : components) {

		component->Render();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}