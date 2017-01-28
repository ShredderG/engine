// Libraries
#pragma comment (lib, "winmm.lib") // TODO: find another lib for *.wav
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "wsock32.lib")
#define _CRT_SECURE_NO_WARNINGS

// Includes
#include <winsock.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#include <string>
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
using namespace std;

// Extra types
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

// Engine
#include "files/defines.cpp"
#include "functions.cpp"
#include "sfx.cpp"
#include "winsock.cpp"
#include "keyboard.cpp" // TODO: check numpad keys
#include "mouse.cpp"
#include "camera.cpp"
#include "display.cpp"
#include "window.cpp"
#include "shader.cpp"
#include "target.cpp"
#include "texture.cpp"
#include "files/textures.cpp"
#include "files/objects.cpp"

// Main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// Initialize everything
	Display::initialize();
	Keyboard::initialize();
	Mouse::initialize();
	Winsock::initialize();
	
	// Create window
	if (Window::create(GAME_TITLE, GAME_FULL,
		(Display::width  - GAME_WIDTH)  / 2,
		(Display::height - GAME_HEIGHT) / 2,
		GAME_WIDTH, GAME_HEIGHT)) {
		// Seed random generator
		srand((unsigned)time(nullptr));
		
		// Load textures
		Engine::loadTextures();
		
		// Load room
		room = GAME_ROOM;
		
		// Variables
		constexpr float timeForFrame = (float)1000.0 / GAME_FPS;
		int frame       = 0;
		int timeCurrent = clock();
		int timeStart   = timeCurrent;
		int timeEnd     = timeStart + 1000;
		int timeSleep;
		MSG msg;

		// Main cycle
		while (Engine::game) {
			// Retrieve message
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// Do frame tick
			Engine::step();
			if (!room.doTransition()) {
				Engine::draw();
			}

			// Wait some mseconds if needed
			if ((timeSleep = timeStart + timeForFrame * ++frame - (timeCurrent = clock())) > 0) {
				timeCurrent += timeSleep;
				Sleep(timeSleep);
			}

			// Check FPS
			if (timeEnd <= timeCurrent) {
				Engine::fps = frame;
				frame       = 0;
				timeStart   = timeCurrent;
				timeEnd     = timeCurrent + 1000;
			}
		}

		// App end
		Engine::deleteObjects();
		Engine::unloadTextures();
		Window::destroy();
	}

	WSACleanup();
	return 0;
}