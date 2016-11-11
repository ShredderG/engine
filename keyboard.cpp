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
		for (int i = 0; i < KEYS_COUNT; i++) {
			isReleased[i] = isPressed[i] = isHeld[i] = false;
		}
		lastKeyPressed = 0;
	}

	inline void reset()
	{
		memset(isReleased, false, KEYS_COUNT);
		memset(isPressed,  false, KEYS_COUNT);
		lastKeyPressed = 0;
	}
} keyboard;