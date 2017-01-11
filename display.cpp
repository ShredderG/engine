// Display
class Display {
public:
	int width;
	int height;

	Display() :
		width(GetSystemMetrics(SM_CXSCREEN)),
		height(GetSystemMetrics(SM_CYSCREEN)) {
		// nothing here
	}
} display;