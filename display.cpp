// Display
namespace Display {
	int width;
	int height;

	void initialize() {
		width  = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
	}
}