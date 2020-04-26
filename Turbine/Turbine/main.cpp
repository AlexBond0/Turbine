#include <iostream>
#include <Windows.h>
#include <crtdbg.h>   // for debug stuff
#include <math.h>
#include "GLSetup.h"
#include "Utils.h"
#include "RenderingContext.h"
#include "scene.h"
#include <thread>         // std::threa

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// ImGui
#include "DebugUI.h";

// ===================================================================================
// Prototyping

int OnCreate(const char* glsl_version);
void OnMouseClickL(int winX, int winY, int mouseX, int mouseY);


void window_size_callback(GLFWwindow* window, int width, int height);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void SetupDebugUI();

GLuint shaderProgram;
GLuint VBO, VAO;

// ===================================================================================
// Global Variables

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

RenderingContext* rcontext;
Scene* scene;

PickObject pickedObj;

const static int REDRAW_TIMER = 10;

int rot = 0;
int dX = 0;
int dY = 0;
bool isLDragging = false;
bool isMDragging = false;
bool isRDragging = false;
bool isClicking = false;
bool hasBeenDragged = false;

int windowX, windowY;

GLFWwindow* window;

DebugUI* debugUI;
Object3DUI* moveUI;
CameraUI* camUI;
WorldUI* worldUI;

// The MAIN function, from here we start the application and run the game loop
int main()
{

// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// hide console lol
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, 0);

	// create the scene
	if (OnCreate(glsl_version) == EXIT_SUCCESS) {

		// setup debug ui
		SetupDebugUI();

		double timepassed = glfwGetTime();
		double timeDiff;

		// render loop
		while (!glfwWindowShouldClose(window)) {

			// get time passed
			timeDiff = glfwGetTime() - timepassed;
			camUI->timePassed = timeDiff;
			timepassed = glfwGetTime();

			// Render scene
			scene->Render(*rcontext);

			// Render UI
			debugUI->Render();

			// Swap the screen buffers
			glfwSwapBuffers(window);

			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();

			// if time passed, apply time change to scene
			if (timeDiff > 0.0)
				scene->OnTimer(*rcontext, timeDiff);
		}

		// Terminate GLFW, clearing any resources allocated by GLFW.
		glfwTerminate();

		// cleanup ImGui components
		delete debugUI;
		delete rcontext;

		return EXIT_SUCCESS;
	}
	else {

		return EXIT_FAILURE;
	}
}

// ============================================================================================================

// This is called then the window is first created and useful to get things ready (e.g. load or create pens, brushes, images, etc)
int OnCreate(const char* glsl_version) {


	// ======================================================================================
	// Init GLFW

	glfwInit();

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(WIDTH, HEIGHT, "Turbine", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window) {

		OutputDebugStringA("\nGLFW window creation FAILED");
		glfwTerminate();

		return EXIT_FAILURE;
	}
	else {

		OutputDebugStringA("\nGLFW window creation OK");
	}

	glfwMakeContextCurrent(window);

	// setup event callbakcs with GLFW
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// ======================================================================================
	// Init GLEW

	// Set this to true so GLEW knows to use a modern approach to retrieving function 
	// pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit()) {

		OutputDebugStringA("\nGLEW initialisation FAILED");
		return EXIT_FAILURE;
	}
	else {

		OutputDebugStringA("\nGLEW initialisation OK");
	}

	// ======================================================================================
	// Check OpenGL version
	const GLubyte* version = glGetString(GL_VERSION);
	std::string versionStr((char*)version);
	std::string message = "\nOpenGL Version : " + versionStr;
	OutputDebugStringA(message.c_str());

	// ======================================================================================
	// Setup Dear ImGui context
	debugUI = new DebugUI(glsl_version, window);

	// setup shaders in renderingcontext;
	rcontext = new RenderingContext();

	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	// Create Turbine scene
	scene = new Scene();
	scene->Setup();

	// Setup camera frustrum based on window size 
	window_size_callback(window, screenWidth, screenHeight);

	return EXIT_SUCCESS;
}

void SetupDebugUI() {

	moveUI = new Object3DUI();
	moveUI->object = scene->world.GetObject3D("Seats");
	debugUI->AddComponent(moveUI);

	camUI = new CameraUI();
	camUI->camera = dynamic_cast<Camera*>(scene->world.GetEntity("POV Camera"));
	debugUI->AddComponent(camUI);

	worldUI = new WorldUI();
	worldUI->world = &scene->world;
	worldUI->rcontext = rcontext;
	debugUI->AddComponent(worldUI);
}

// ============================================================================================================

void window_size_callback(GLFWwindow* window, int width, int height) {

	if (width > 0 && height > 0 && scene) {

		windowX = width;
		windowY = height;


		glViewport(0, 0, width, height);

		Camera* c = dynamic_cast<Camera*>(scene->world.GetEntity("Default Camera"));

		c->fAspect = (float)width / height;

		float top = (float)(tan(c->fFovy * 0.5) * c->fZNear);

		float bottom = -top;
		float left = c->fAspect * bottom;
		float right = c->fAspect * top;

		rcontext->projectionMatrix = glm::frustum(
			left,
			right,
			bottom,
			top,
			c->fZNear,
			c->fZFar
		);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	//if (key == GLFW_KEY_E && action == GLFW_PRESS)
	//	OutputDebugStringA("\nE");

	bool keydown = (action == GLFW_PRESS);
	switch (key) {

		// swap cameras | SPACE
		case GLFW_KEY_SPACE: {

			if (action == GLFW_RELEASE)
				scene->ToggleCamera();
			
			break;
		}

		// toggle standard animation | ENTER
		case GLFW_KEY_ENTER: {

			if (action == GLFW_RELEASE) {

				scene->ToggleAnimation();
			}

			break;
		}

		// toggle day / night | D
		case GLFW_KEY_D: {

			if (action == GLFW_RELEASE)
				scene->ToggleDayNight();

			break;
		}

		// start standard ride script | R
		case GLFW_KEY_R: {

			if (action == GLFW_RELEASE)
				scene->animator.QueueStandardRide();

			break;
		}

		// start long ride script | L
		case GLFW_KEY_L: {

			if (action == GLFW_RELEASE)
				scene->animator.QueueLongRide();

			break;
		}

		// toggle dry ice | G
		case GLFW_KEY_G: {

			(dynamic_cast<Particle*>(scene->world.GetEntity("DryIce")))->active = keydown;
			(dynamic_cast<Particle*>(scene->world.GetEntity("DryIce2")))->active = keydown;

			break;
		}

		// toggle fire | F
		case GLFW_KEY_F: {

			(dynamic_cast<Particle*>(scene->world.GetEntity("Fire")))->active = keydown;
			(dynamic_cast<Particle*>(scene->world.GetEntity("FireSmoke")))->active = keydown;

			break;
		}

		// ==================================================
		// manual ride controlls

		// platter clockwise | LEFT KEY OR NUM 1
		case GLFW_KEY_LEFT:
		case GLFW_KEY_1:
			scene->animator.MovePart(Part::PLATTER, Change::ROT_C);
			break;

		// platter anti-clockwise | RIGHT KEY OR NUM 3
		case 0x27:
		case 0x33:
		case 0x63:
			scene->animator.MovePart(Part::PLATTER, Change::ROT_AC);
			break;

		// arm clockwise | NUM 7
		case 0x37:
		case 0x67:
			scene->animator.MovePart(Part::ARM, Change::ROT_C);
			break;

		// arm anti-clockwise | NUM 9
		case 0x39:
		case 0x69:
			scene->animator.MovePart(Part::ARM, Change::ROT_AC);
			break;

		// seats clockwise | NUM 4
		case 0x34:
		case 0x64:
			scene->animator.MovePart(Part::SEATS, Change::ROT_C);
			break;

		// seats anti-clockwise | NUM 6
		case 0x36:
		case 0x66:
			scene->animator.MovePart(Part::SEATS, Change::ROT_AC);
			break;

		// tilt up | UP KEY OR NUM 5
		case 0x26:
		case 0x35:
		case 0x65:
			scene->animator.MovePart(Part::TILT, Change::TILT_UP);
			break;

		// tilt down | DOWN KEY OR NUM 2
		case 0x28:
		case 0x32:
		case 0x62:
			scene->animator.MovePart(Part::TILT, Change::TILT_DOWN);
			break;

		// reset all rotations | NUM 8
		case 0x38:
		case 0x68:
			scene->animator.ResetRotations();
			break;
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

	if (isMDragging) {
		
		hasBeenDragged = true;
		
		scene->world.GetActiveCamera()->MoveCam(xpos, ypos);
	}
	else if (isLDragging || isRDragging) {
		
		hasBeenDragged = true;
		
		scene->RotateCamera(xpos, ypos, !(isRDragging || scene->pov));
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	// don't click things behind ImGui windows
	ImGuiIO& io = ImGui::GetIO();
	bool validClick = (action == GLFW_PRESS && !io.WantCaptureMouse);
	bool validRelease = (action == GLFW_RELEASE && !io.WantCaptureMouse);

	double xpos, ypos;
	int winx, winy;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwGetWindowSize(window, &winx, &winy);

	switch (button) {

		case GLFW_MOUSE_BUTTON_LEFT: {

			if (validClick) {
	
				isLDragging = true;
			}
			else {

				isLDragging = false;
				isClicking = false;
				scene->world.GetActiveCamera()->FinishMovement();
				
				if (!hasBeenDragged && validRelease) {

					OnMouseClickL(winx, winy, xpos, ypos);
				}
				
				hasBeenDragged = false;
			}
			break;
		}

		case GLFW_MOUSE_BUTTON_MIDDLE: {

			if (validClick) {

				isMDragging = true;
			}
			else {

				isMDragging = false;
				isClicking = false;
				scene->world.GetActiveCamera()->FinishMovement();
				hasBeenDragged = false;
			}
			break;
		}

		case GLFW_MOUSE_BUTTON_RIGHT: {

			if (validClick) {

				isRDragging = true;
			}
			else {

				isRDragging = false;
				isClicking = false;
				scene->world.GetActiveCamera()->FinishMovement();
				
				//if (!hasBeenDragged)
				//	OnMouseClick(nFlags, x, y);
				
				hasBeenDragged = false;
			}
			break;
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

	// check mouse scroll isnt in UI
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse) {

		scene->world.GetActiveCamera()->FocusCam(yoffset);
	}
}

void OnMouseClickL(int winX, int winY, int mouseX, int mouseY) {

	if (pickedObj.hasBeenPicked)
		pickedObj.object->isHighlighted = false;

	// calculate picking ray
	glm::vec3 pickRay = scene->world.GetActiveCamera()->CalculatePickRay(mouseX, mouseY, winX, winY);

	// get picked object from scene
	pickedObj = scene->world.GetActiveCamera()->GetPickedObject(&scene->world, pickRay);

	if (pickedObj.hasBeenPicked) {
		
		moveUI->object = pickedObj.object;
		pickedObj.object->isHighlighted = true;
	}
}