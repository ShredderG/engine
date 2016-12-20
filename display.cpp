// Display
class Display {
public:
	short width;
	short height;

	Display() :
		width(GetSystemMetrics(SM_CXSCREEN)),
		height(GetSystemMetrics(SM_CYSCREEN)) {
		// nothing here
	}
} display;