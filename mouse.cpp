// Mouse buttons
enum MouseButton : uchar {
	MOUSE_LEFT       = 0,
	MOUSE_RIGHT      = 1,
	MOUSE_WHEEL      = 2,
	MOUSE_MIDDLE     = 2,
	MOUSE_CENTER     = 2,
	MOUSE_WHEEL_UP   = 0,
	MOUSE_WHEEL_DOWN = 1
};

// Mouse
namespace Mouse {
	int  x;
	int  y;
	bool isReleased[3];
	bool isPressed[3];
	bool isHeld[3];
	bool isWheelRotated[2];
	bool isVisible;
	bool isOutside = false;

	// Reset buttons status
	void reset() {
		memset(isPressed, false, 3);
		memset(isReleased, false, 3);
		isWheelRotated[MOUSE_WHEEL_UP] = isWheelRotated[MOUSE_WHEEL_DOWN] = false;
	}
	
	// Move mouse
	void move(int x, int y) {
		if (GetActiveWindow()) {
			SetCursorPos(x, y);
		}
	}

	// Show mouse
	void show() {
		while (ShowCursor(true) < 0);
		isVisible = true;
	}

	// Hide mouse
	void hide() {
		while (ShowCursor(false) >= 0);
		isVisible = false;
	}

	// Initialize
	void initialize() {
		memset(isHeld, false, 3);
		reset();

#if GAME_MOUSE == true
		show();
#else
		hide();
#endif
	}
}