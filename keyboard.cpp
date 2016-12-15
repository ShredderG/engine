struct Keyboard
{
	static const int KEYS_COUNT = 256;
	uchar lastKeyPressed;
	bool
		isReleased[KEYS_COUNT],
		isPressed[KEYS_COUNT],
		isHeld[KEYS_COUNT];

	Keyboard()
	{
		memset(isHeld, false, KEYS_COUNT);
		reset();
	}

	// Reset keyboard status
	void reset()
	{
		memset(isReleased, false, KEYS_COUNT);
		memset(isPressed,  false, KEYS_COUNT);
		lastKeyPressed = 0;
	}
} keyboard;