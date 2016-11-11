// Mouse buttons
enum MouseButton
{
	MOUSE_LEFT       = 0,
	MOUSE_RIGHT      = 1,
	MOUSE_WHEEL      = 3,
	MOUSE_MIDDLE     = 3,
	MOUSE_WHEEL_UP   = 0,
	MOUSE_WHEEL_DOWN = 1
};

// Mouse
struct Mouse
{
private:
	POINT point_;

public:
	short x, y;
	bool
		isReleased[4],
		isPressed[4],
		isHeld[4],
		isWheelRotated[2],
		isVisible,
		isOutside;
	
	Mouse() : isOutside(false)
	{
		//inirt nouse
		for (uchar i = 0; i < 3; i++) {
			isHeld[i] = false;
		}
	}

	// Move mouse
	void move(short GM_x, short GM_y)
	{
		GetCursorPos(&point_);
		x = point_.x - x;
		y = point_.y - y;
		if (bool(GetActiveWindow())) {
			SetCursorPos(x + GM_x, y + GM_y);
		}
	}

	// Show mouse
	void show()
	{
		while (ShowCursor(true) < 0);
		isVisible = true;
	}

	// Hide mouse
	void hide()
	{
		while (ShowCursor(false) >= 0);
		isVisible = false;
	}

	// Update mouse buttons states
	inline void update(HWND hWnd, short width, short height)
	{
		// Show or hide cursor
		if (!isVisible) {
			if (isOutside) {
				if (x >= 0 && y >= 0 && x < width && y < height) {
					ShowCursor(false);
					isOutside = false;
				}
			}
			else {
				if (x < 0 || y < 0 || x >= width || y >= height) {
					ShowCursor(true);
					isOutside = true;
				}
			}
		}
	}

	// Unset mouse wheel status
	inline void reset()
	{
		isWheelRotated[MOUSE_WHEEL_UP] = isWheelRotated[MOUSE_WHEEL_DOWN] = false;
		memset(isPressed,  false, 4);
		memset(isReleased, false, 4);
	}
} mouse;