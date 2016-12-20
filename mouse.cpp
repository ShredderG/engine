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
class Mouse {
public:
	short x;
	short y;
	bool isReleased[3];
	bool isPressed[3];
	bool isHeld[3];
	bool isWheelRotated[2];
	bool isVisible;
	bool isOutside;
	
	Mouse() : isOutside(false) {
		memset(isHeld, false, 3);
		reset();

#if GAME_MOUSE == true
		show();
#else
		hide();
#endif
	}

	// Move mouse
	void move(short _x, short _y) {
		if (GetActiveWindow()) {
			POINT point;
			GetCursorPos(&point);
			SetCursorPos(point.x - x + _x, point.y - y + _y);
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

	// Reset buttons status
	void reset() {
		memset(isPressed,  false, 3);
		memset(isReleased, false, 3);
		isWheelRotated[MOUSE_WHEEL_UP] = isWheelRotated[MOUSE_WHEEL_DOWN] = false;
	}
} mouse;