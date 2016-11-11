// No warnings
#define _CRT_SECURE_NO_WARNINGS

// OpenGL
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

// Includes
#include <windows.h>
#include <math.h>
#include <time.h>
#include <string>
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
using namespace std;

// Engine
#include "files/defines.cpp"
#include "other.cpp"
#include "keyboard.cpp"
#include "mouse.cpp"
#include "view.cpp"
#include "display.cpp"
#include "texture.cpp"
#include "files/textures.cpp"
#include "window.cpp"
#include "shader.cpp"
#include "files/objects.cpp"
#include "room.cpp"
#include "files/rooms.cpp"

// Main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Create window
	if (!window.create(GM_TITLE, GM_FULL,
		(display.width  - GM_WIDTH)  / 2,
		(display.height - GM_HEIGHT) / 2,
		GM_WIDTH, GM_HEIGHT))
		return 0;

	// Load textures, shader, room
	GM_loadTextures();
	shader.create();
	room = r_start;

	// Variables
	const float timeFrame = 1000.0 / GM_FPS;
	int frame       = 0,
		timeCurrent = clock(),
		timeStart   = timeCurrent,
		timeEnd     = timeStart + 1000,
		timeSleep;
	MSG msg;

	// Main cycle
	while (GM_game) {
		// Retrieve message
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Do frame tick
		GM_step();
		if (!room.doTransition()) {
			GM_draw();
		}

		// Wait some mseconds if needed
		if ((timeSleep = timeStart + timeFrame * ++frame - (timeCurrent = clock())) > 0) {
			timeCurrent += timeSleep;
			Sleep(timeSleep);
		}

		// Check FPS
		if (timeEnd <= timeCurrent) {
			fps       = frame;
			frame     = 0;
			timeStart = timeCurrent;
			timeEnd   = timeCurrent + 1000;
		}
	}

	// App end
	GM_deleteObjects();
	GM_unloadTextures();
	window.destroy();
	return 0;
}