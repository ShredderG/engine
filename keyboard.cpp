// Keyboard
class Keyboard {
private:
	static const int KEYS_COUNT = 256;
	uchar lastPressedKey_;

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
	uchar getLastPressedKey() {
		if (lastPressedKey_ >= 'A' && lastPressedKey_ <= 'Z') {
			return isHeld[VK_SHIFT] ? lastPressedKey_ : tolower(lastPressedKey_);
		}

		if (!isHeld[VK_SHIFT]) {
			switch (lastPressedKey_) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				return lastPressedKey_;

			case VK_OEM_1:      return ';';
			case VK_OEM_PLUS:   return '=';
			case VK_OEM_COMMA:  return ',';
			case VK_OEM_MINUS:  return '-';
			case VK_OEM_PERIOD: return '.';
			case VK_OEM_2:      return '/';
			case VK_OEM_3:      return '`';

			case VK_OEM_4: return '[';
			case VK_OEM_5: return '\\';
			case VK_OEM_6: return ']';
			case VK_OEM_7: return '\'';			
			}
		} else {
			switch (lastPressedKey_) {
			case '0': return ')';
			case '1': return '!';
			case '2': return '@';
			case '3': return '#';
			case '4': return '$';
			case '5': return '%';
			case '6': return '^';
			case '7': return '&';
			case '8': return '*';
			case '9': return '(';

			case VK_OEM_1:      return ':';
			case VK_OEM_PLUS:   return '+';
			case VK_OEM_COMMA:  return '<';
			case VK_OEM_MINUS:  return '_';
			case VK_OEM_PERIOD: return '>';
			case VK_OEM_2:      return '?';
			case VK_OEM_3:      return '~';

			case VK_OEM_4: return '{';
			case VK_OEM_5: return '|';
			case VK_OEM_6: return '}';
			case VK_OEM_7: return '\"';
			}
		}

		return (lastPressedKey_ == ' ') ? ' ' : 0;
	}
} keyboard;