#include <Windows.h>  // for access to the windows APIs
#include <crtdbg.h>   // for debug stuff
#include <math.h>
#include "GLSetup.h"
#include "Utils.h"
#include "RenderingContext.h"
#include "scene.h"

static HWND hwnd;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

RenderingContext rcontext;
Scene* scene = new Scene();

const static int REDRAW_TIMER = 10;

int rot = 0;
int dX = 0;
int dY = 0;
bool isLDragging = false;
bool isMDragging = false;
bool isRDragging = false;
bool isClicking = false;
bool hasBeenDragged = false;

void OnCreate();
void OnTimer(UINT nIDEvent);
void OnSize(DWORD type, UINT cx, UINT cy);

void OnLButtonDown(UINT nFlags, int x, int y);
void OnLButtonUp(UINT nFlags, int x, int y);
void OnMButtonDown(UINT nFlags, int x, int y);
void OnMButtonUp(UINT nFlags, int x, int y);
void OnRButtonDown(UINT nFlags, int x, int y);
void OnRButtonUp(UINT nFlags, int x, int y);

void OnMouseMove(UINT nFlags, int x, int y);
void OnMouseClick(UINT nFlags, int x, int y);
void OnKey(UINT uMsg, WPARAM wParam);

void CleanUp();


// ================================================================================================

// Win32 entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  // This mini section is really useful to find memory leaks
#ifdef _DEBUG   // only include this section of code in the DEBUG build
	// _CrtSetBreakAlloc(3613);  // really useful line of code to help find memory leaks
	_onexit(_CrtDumpMemoryLeaks); // check for memory leaks when the program exits
#endif

  // To create a window, we must first define various attributes about it
	WNDCLASSEX classname;
  classname.cbSize=sizeof(WNDCLASSEX); 
  classname.style=CS_HREDRAW|CS_VREDRAW;
  classname.lpfnWndProc=(WNDPROC)WndProc;   // This is the name of the event-based callback method
  classname.cbClsExtra=0;
  classname.cbWndExtra=0;
  classname.hInstance=hInstance;
  classname.hIcon=LoadIcon(NULL, IDI_WINLOGO);
  classname.hCursor=LoadCursor(NULL, IDC_ARROW);
  classname.hbrBackground=NULL;//   ::GetSysColorBrush(COLOR_3DFACE); - note, setting this to null prevents flickering when resizing the window
  classname.lpszMenuName=NULL;
  classname.lpszClassName=L"SpinOut";
  classname.hIconSm=LoadIcon(NULL, IDI_WINLOGO);
  RegisterClassEx(&classname);

  // Default size will be 1024x768 and we'll centre it on the screen - this include the caption and window borders so might not be the canvas size (which will be smaller)
  int width=1024;
  int height=768;
  int offx=(::GetSystemMetrics(SM_CXSCREEN)-width)/2;
  int offy=(::GetSystemMetrics(SM_CYSCREEN)-height)/2;

  // Create the window using the definition provided above
  CreateWindowEx(NULL, L"SpinOut", L"SpinOut - OpenGL Ride Assignment", WS_OVERLAPPEDWINDOW|WS_VISIBLE, offx, offy, width, height, NULL, NULL, hInstance, NULL);

  // Set the event-based message system up
  MSG msg;
  PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
  while (msg.message!=WM_QUIT)  // keep looping until we get the quit message
  {
    if (GetMessage(&msg, NULL, 0, 0)) // cause this thread to wait until there is a message to process
    {
      // These two lines of code take the MSG structure, mess with it, and correctly dispatch it to the WndProc defined during the window creation
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  CleanUp();

}

// This is our message handling method and is called by the system (via the above while-loop) when we have a message to process
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message) {

  case WM_CREATE:
    hwnd=hWnd;
    OnCreate();
    return 0;
    break;

  case WM_SIZE:
    OnSize((DWORD) wParam, (UINT) (lParam&0xFFFF), (UINT) (lParam>>16));
    break;

  case WM_PAINT:
	{
	PAINTSTRUCT paint;
	BeginPaint(hwnd, &paint);
	scene->Render(rcontext);
	EndPaint(hwnd, &paint);
	}
	break;

	case WM_LBUTTONDOWN:
		OnLButtonDown(wParam, lParam & 0xFFFF, (lParam>>16) & 0xFFFF);
		break;

	case WM_LBUTTONUP:
		OnLButtonUp(wParam, lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);
		break;

	case WM_MBUTTONDOWN:
		OnMButtonDown(wParam, lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);
		break;

	case WM_MBUTTONUP: 
		OnMButtonUp(wParam, lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);
		break;

	case WM_RBUTTONDOWN:
		OnRButtonDown(wParam, lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);
		break;

	case WM_RBUTTONUP:
		OnRButtonUp(wParam, lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);
		break;

	case WM_MOUSEMOVE:
		OnMouseMove(wParam, lParam&0xFFFF, (lParam>>16)&0xFFFF);
		break;

	case WM_MOUSEWHEEL:
		if (!scene->pov)
			scene->camera.FocusCam(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
  
	case WM_DESTROY: 
		PostQuitMessage(0);
		break;
  
	case WM_KEYDOWN: 
		OnKey(message, wParam);
		break;

	case WM_KEYUP:
		OnKey(message, wParam);
		break;
	

	case WM_TIMER:
		OnTimer(wParam);
		break;

	case WM_CLOSE:
		delete scene;
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		break;
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}

// ================================================================================================



// This is called then the window is first created and useful to get things ready (e.g. load or create pens, brushes, images, etc)
void OnCreate()
{
  InitGL(hwnd);

  GLenum err=glewInit();
  if (err!=GLEW_OK)
    DisplayMessage((char*) glewGetErrorString(err));

  scene->Setup();

  int prog 
	= rcontext.glprogram 
	= LoadShaders(L"vertshader.txt", L"fragshader.txt");

  // Light
  LinkUniformHandle(rcontext.lighthandles[0], "u_l_direction", prog);
  LinkUniformHandle(rcontext.lighthandles[1], "u_l_halfplane", prog);
  LinkUniformHandle(rcontext.lighthandles[2], "u_l_ambient", prog);
  LinkUniformHandle(rcontext.lighthandles[3], "u_l_diffuse", prog);
  LinkUniformHandle(rcontext.lighthandles[4], "u_l_specular", prog);

  // Material
  LinkUniformHandle(rcontext.mathandles[0], "u_m_ambient", prog);
  LinkUniformHandle(rcontext.mathandles[1], "u_m_diffuse", prog);
  LinkUniformHandle(rcontext.mathandles[2], "u_m_specular", prog);
  LinkUniformHandle(rcontext.mathandles[3], "u_m_shininess", prog);

  // texture
  LinkUniformHandle(rcontext.texturehandle, "u_texture", prog);
        
  // Matrices
  LinkUniformHandle(rcontext.nmathandle, "u_normalmatrix", prog);
  LinkUniformHandle(rcontext.mvphandle, "u_mvpmatrix", prog);
  LinkUniformHandle(rcontext.vphandle, "u_vpmatrix", prog);

  // Flags
  LinkUniformHandle(rcontext.useslighthandle, "u_usesLight", prog);
  LinkUniformHandle(rcontext.usestexturehandle, "u_usesTexture", prog);
        
  // Attributes
  LinkAttribHandle(rcontext.verthandles[0], "a_position", prog);
  LinkAttribHandle(rcontext.verthandles[1], "a_normal", prog);
  LinkAttribHandle(rcontext.verthandles[2], "a_uvcoord", prog);

  // instancing
  LinkUniformHandle(rcontext.usesinstancing, "u_instancing", prog);
  LinkAttribHandle(rcontext.instancehandle, "a_offset", prog);

  // billboarding
  LinkUniformHandle(rcontext.usesbillboarding, "u_billboarding", prog);
  LinkUniformHandle(rcontext.billboardhandles[0], "a_b_up", prog);
  LinkUniformHandle(rcontext.billboardhandles[1], "a_b_right", prog);


  // setting the timer 
  ::SetTimer(hwnd, REDRAW_TIMER, 30, NULL);
  
  glUseProgram(prog);

  glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
}

void OnTimer(UINT nIDEvent)
{
	switch (nIDEvent) {

		case REDRAW_TIMER: {

			// do rotations
			rcontext.RotateX(dY);
			rcontext.RotateY(dX);
			rot++;

			scene->OnTimer(rcontext, 30);
			break;
		}
	}
}

// Called when the window is resized
void OnSize(DWORD type, UINT cx, UINT cy)
{
  if (cx>0 && cy>0)
  {
    glViewport(0, 0, cx, cy);


    // our FOV is 60 degrees 
    const float FOVY=(60.0f*(float) M_PI/180.0f); 
    const float NEAR_CLIP=0.01f;  // for example
    const float FAR_CLIP=100.0f;  // for example

    float fAspect=(float) cx/cy;
    float top=(float) (tan(FOVY*0.5)*NEAR_CLIP);
    float bottom=-top;
    float left=fAspect*bottom;
    float right=fAspect*top;
  
    MyMatrix::SetFrustum(
		rcontext.projectionmatrix, 
		left, 
		right, 
		bottom, 
		top, 
		NEAR_CLIP, 
		FAR_CLIP
	);
  }
}

// =================================================

void OnLButtonDown(UINT nFlags, int x, int y) {

	isLDragging = true;
}

void OnLButtonUp(UINT nFlags, int x, int y) {

	isLDragging = false;
	isClicking = false;
	scene->camera.FinishMovement();

	if (!hasBeenDragged)
		OnMouseClick(nFlags, x, y);

	hasBeenDragged = false;
}

// =================================================

void OnMButtonDown(UINT nFlags, int x, int y) {

	isMDragging = true;
}

void OnMButtonUp(UINT nFlags, int x, int y) {

	isMDragging = false;
	isClicking = false;
	scene->camera.FinishMovement();
	hasBeenDragged = false;
}

// =================================================

void OnRButtonDown(UINT nFlags, int x, int y) {

	isRDragging = true;
}

void OnRButtonUp(UINT nFlags, int x, int y) {

	isRDragging = false;
	isClicking = false;
	scene->camera.FinishMovement();

	if (!hasBeenDragged)
		OnMouseClick(nFlags, x, y);

	hasBeenDragged = false;
}

// =================================================

void OnMouseMove(UINT nFlags, int x, int y) {

	if (isMDragging) {

		hasBeenDragged = true;

		scene->camera.MoveCam(x, y);
	}
	else if (isLDragging || isRDragging) {

		hasBeenDragged = true;

		scene->RotateCamera(x, y, !(isRDragging || scene->pov));
	}
}

void OnMouseClick(UINT nFlags, int x, int y) {

	RECT rect;
	::GetClientRect(hwnd, &rect);

	// float* pickRay = camera.CalculatePickRay( x, y, rect.right, rect.bottom );

}

void OnKey(UINT keyEvent, WPARAM keyID) {

	bool keyDown = (keyEvent == WM_KEYDOWN);

	switch (keyID) {

		// swap cameras | SPACE
		case VK_SPACE: {

			if (!keyDown)
				scene->ToggleCamera();
			
			break;
		}

		// toggle standard animation | ENTER
		case 0x0D: {

			if (!keyDown) {

				scene->ToggleAnimation();
			}

			break;
		}

		// toggle day / night | D
		case 0x44: {

			if (!keyDown)
				scene->ToggleDayNight();

			break;
		}

		// start standard ride script | R
		case 0x52: {

			if (!keyDown)
				scene->animator.QueueStandardRide();

			break;
		}

		// start long ride script | L
		case 0x4C: {

			if (!keyDown)
				scene->animator.QueueLongRide();

			break;
		}

		// toggle dry ice | G
		case 0x47: {

			(dynamic_cast<Particle*>(scene->objects["DryIce"]))->active = keyDown;
			(dynamic_cast<Particle*>(scene->objects["DryIce2"]))->active = keyDown;

			break;
		}

		// toggle fire | F
		case 0x46: {

			(dynamic_cast<Particle*>(scene->objects["Fire"]))->active = keyDown;
			(dynamic_cast<Particle*>(scene->objects["FireSmoke"]))->active = keyDown;

			break;
		}

		// ==================================================
		// manual ride controlls

		// platter clockwise | LEFT KEY OR NUM 1
		case 0x25: 
		case 0x31:
		case 0x61:
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

void CleanUp() {

  glDeleteProgram(rcontext.glprogram);
}
