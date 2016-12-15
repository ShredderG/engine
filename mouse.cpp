// Mouse buttons
enum MouseButton : uchar
{
	MOUSE_LEFT       = 0,
	MOUSE_RIGHT      = 1,
	MOUSE_WHEEL      = 2,
	MOUSE_MIDDLE     = 2,
	MOUSE_CENTER     = 2,
	MOUSE_WHEEL_UP   = 0,
	MOUSE_WHEEL_DOWN = 1
};

// Mouse
struct Mouse
{
	short x, y;
	bool
		isReleased[3],
		isPressed[3],
		isHeld[3],
		isWheelRotated[2],
		isVisible,
		isOutside;
	
	Mouse() : isOutside(false)
	{
		memset(isHeld, false, 3);
		reset();

#if GAME_MOUSE == true
		show();
#else
		hide();
#endif
	}

	// Move mouse
	void move(short xNew, short yNew)
	{
		if (bool(GetActiveWindow())) {
			POINT point;
			GetCursorPos(&point);
			SetCursorPos(point.x - x + xNew, point.y - y + yNew);
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

	// Reset buttons status
	void reset()
	{
		isWheelRotated[MOUSE_WHEEL_UP] = isWheelRotated[MOUSE_WHEEL_DOWN] = false;
		memset(isPressed,  false, 3);
		memset(isReleased, false, 3);
	}
} mouse;