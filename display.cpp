struct Display
{
	short width, height;

	Display() :
		width(GetSystemMetrics(SM_CXSCREEN)),
		height(GetSystemMetrics(SM_CYSCREEN))
	{
	}
} display;