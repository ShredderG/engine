// Keyboard
class Keyboard {
private:
	static const int KEYS_COUNT = 256;
	char lastPressedKey_;

	friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

public:
	bool isReleased[KEYS_COUNT];
	bool isPressed[KEYS_COUNT];
	bool isHeld[KEYS_COUNT];

	Keyboard() {
		memset(isHeld, false, KEYS_COUNT);
		reset();
	}

	// Reset keyboard status
	void reset() {
		memset(isReleased, false, KEYS_COUNT);
		memset(isPressed,  false, KEYS_COUNT);
		lastPressedKey_ = 0;
	}

	// Get last pressed key
	char getLastPressedKey() {
		lastPressedKey_ = isHeld[VK_SHIFT] ? toupper(lastPressedKey_) : tolower(lastPressedKey_);

		if (!isHeld[VK_SHIFT]) {
			switch (lastPressedKey_) {
			case -34: return '\'';
			case -35: return ']';
			case -36: return '\\';
			case -37: return '[';
			case -64: return '`';
			case -65: return '/';
			case -66: return '.';
			case -67: return '-';
			case -68: return ',';
			case -69: return '=';
			case -70: return ';';
			}
		} else {
			switch (lastPressedKey_) {
			case -34: return '"';
			case -35: return '}';
			case -36: return '|';
			case -37: return '{';
			case -64: return '~';
			case -65: return '?';
			case -66: return '>';
			case -67: return '_';
			case -68: return '<';
			case -69: return '+';
			case -70: return ':';
			case '1': return '!';
			case '2': return '@';
			case '3': return '#';
			case '4': return '$';
			case '5': return '%';
			case '6': return '^';
			case '7': return '&';
			case '8': return '*';
			case '9': return '(';
			case '0': return ')';
			}
		}

		return lastPressedKey_ > 31 && lastPressedKey_ < 127 ? lastPressedKey_ : 0;
	}
} keyboard;