// �������� WndProc
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

struct Window
{
	//friend void Engine::draw();
//private:
	HGLRC     hRC;       // ���������� �������� ����������
	HDC       hDC;       // ��������� �������� ���������� GDI
	HWND      hWnd;      // ����� ����� �������� ���������� ����
	HINSTANCE hInstance; // ����� ����� �������� ���������� ����������

public:
	ushort x, y, width, height;
	string title;
	bool hasFocus, isFullscreen;

	Window() :
		hRC(NULL),
		hDC(NULL),
		hWnd(NULL),
		hInstance(NULL)
	{
	}

	void move(ushort xNew, ushort yNew)
	{
		SetWindowPos(hWnd, hWnd, x = xNew, y = yNew, width, height, SWP_NOZORDER | SWP_NOSIZE);
	}

	void resize(ushort widthNew, ushort heightNew)
	{
		RECT client, window;
		GetClientRect(hWnd, &client);
		GetWindowRect(hWnd, &window);

		SetWindowPos(hWnd, hWnd, x, y,
			widthNew  + window.right  - window.left - client.right,
			heightNew + window.bottom - window.top  - client.bottom,
			SWP_NOZORDER | SWP_NOMOVE);
	}

	void update()
	{
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// ���������� ����������� �������������� �������� ��� ����
		gluPerspective(camera.fov, (float)width / height, camera.zNear, camera.zFar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void initialize()
	{
		glClearColor(0, 0, 0, 0);

		glShadeModel(GL_SMOOTH);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_BACK, GL_LINE);

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER, 0.01);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearDepth(1.0);
		glEnable(GL_BLEND);
	}

	void changeFullscreen()
	{
		glDisable(GL_TEXTURE_2D);
		destroy();
		create(title, !isFullscreen, x, y, width, height);
	}

	void setTitle(string titleNew)
	{
		title = titleNew;
		SetWindowText(hWnd, title.c_str());
	}

	void destroy()
	{
		if (isFullscreen) {
			ChangeDisplaySettings(NULL, 0);
		}

		if (hRC) {
			if (!wglMakeCurrent(NULL, NULL)) {
				showMessage("Release Of DC And RC Failed.");
			}
			if (!wglDeleteContext(hRC)) {
				showMessage("Release Rendering Context Failed.");
			}
		}
		if (hDC && !ReleaseDC(hWnd, hDC)) {
			showMessage("Release Device Context Failed.");
		}
		if (hWnd && !DestroyWindow(hWnd)) {
			showMessage("Could Not Release hWnd.");
		}
		if (!UnregisterClass("OpenGL", hInstance)) {
			showMessage("Could Not Unregister Class.");
		}

		hRC       = NULL;
		hDC       = NULL;
		hWnd      = NULL;
		hInstance = NULL;
	}

	bool create(string GAME_title, bool GAME_fullscreen, short GAME_x, short GAME_y, short GAME_width, short GAME_height)
	{
		title = GAME_title;
		isFullscreen = GAME_fullscreen;
		x = GAME_x;
		y = GAME_y;
		width  = GAME_width;
		height = GAME_height;
		if (isFullscreen) {
			x = y = 0;
		}

		GLuint PixelFormat; // ������ ��������� ����� ������
		WNDCLASS wc;        // ��������� ������ ����
		DWORD dwExStyle;    // ����������� ����� ����
		DWORD dwStyle;      // ������� ����� ����

		RECT WindowRect = { 0, 0, width, height }; // Grabs Rectangle Upper Left / Lower Right Values

		hInstance = GetModuleHandle(NULL);                          // ������� ���������� ������ ����������
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;              // ���������� � ������ ������� DC
		wc.lpfnWndProc = (WNDPROC)WndProc;                          // ��������� ��������� ���������
		wc.cbClsExtra = 0;                                          // ��� �������������� ���������� ��� ����
		wc.cbWndExtra = 0;                                          // ��� �������������� ���������� ��� ����
		wc.hInstance = hInstance;                                   // ������������� ����������
		wc.hIcon = LoadIcon(hInstance, IDI_WINLOGO);                // ��������� ������ �� ���������
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);                   // ��������� ��������� �����
		wc.hbrBackground = NULL;                                    // ��� �� ��������� ��� GL
		wc.lpszMenuName = NULL;                                     // ���� � ���� �� �����
		wc.lpszClassName = "OpenGL";                                // ������������� ��� ������

		// �������� ���������������� ����� ����
		if (!RegisterClass(&wc)) {
			showMessage("Failed To Register The Window Class.");
			return false;
		}
		
		// ������������� �����?
		if (isFullscreen) {
			DEVMODE dmScreenSettings;                                                 // ����� ����������
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));                   // ������� ��� �������� ���������
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);                       // ������ ��������� Devmode
			dmScreenSettings.dmPelsWidth = width;                                     // ������ ������
			dmScreenSettings.dmPelsHeight = height;                                   // ������ ������
			dmScreenSettings.dmBitsPerPel = 32;                                       // ������� �����
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; // ����� �������

			// �������� ���������� ��������� ����� � �������� ���������.
			if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
				isFullscreen = false;
			}
		}

		if (isFullscreen || GAME_NOBORDER) {
			dwExStyle = WS_EX_APPWINDOW; // ����������� ����� ����
			dwStyle   = WS_POPUP;        // ������� ����� ����
		}
		else {
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // ����������� ����� ����
			dwStyle   = WS_OVERLAPPEDWINDOW;
			if (!GAME_MINIMIZE) dwStyle &= ~WS_MINIMIZEBOX;
			if (!GAME_MAXIMIZE) dwStyle &= ~WS_MAXIMIZEBOX;
		}

		// ��������� ���� ���������� �������
		AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);

		if (!(hWnd = CreateWindowEx(
			dwExStyle,                          // ����������� ����� ��� ����
			"OpenGL",                           // ��� ������
			title.c_str(),                      // ��������� ����
			WS_CLIPSIBLINGS |                   // ��������� ����� ��� ����
			WS_CLIPCHILDREN |                   // ��������� ����� ��� ����
			dwStyle,                            // ���������� ����� ��� ����
			x, y,                               // ������� ����
			WindowRect.right - WindowRect.left, // ���������� ���������� ������
			WindowRect.bottom - WindowRect.top, // ���������� ���������� ������
			NULL,                               // ��� �������������
			NULL,                               // ��� ����
			hInstance,                          // ���������� ����������
			NULL)))                             // �� ������� ������ �� WM_CREATE (???)
		{
			destroy();                               // ������������ �����
			showMessage("������ ��� �������� ����"); // ������
			return false;                            // ������� false
		}

		static PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;

		// ��������� �� ������
		if (hDC = GetDC(hWnd)) {
			if (PixelFormat = ChoosePixelFormat(hDC, &pfd)) {
				if (SetPixelFormat(hDC, PixelFormat, &pfd)) {
					if (hRC = wglCreateContext(hDC)) {
						if (wglMakeCurrent(hDC, hRC)) {
							ShowWindow(hWnd, SW_SHOW);    // �������� ����
							SetForegroundWindow(hWnd);    // ������ ������� ���������
							SetFocus(hWnd);               // ���������� ����� ���������� �� ���� ����
							update();                     // �����������
							initialize();                 // ������

							return true;                  // ���� �������
						}
						else showMessage("Can't Activate The GL Rendering Context.");
					}
					else showMessage("Can't Create A GL Rendering Context.");
				}
				else showMessage("Can't Set The PixelFormat.");
			}
			else showMessage("Can't Find A Suitable PixelFormat.");
		}
		else showMessage("Can't Create A GL Device Context.");

		// ������
		destroy();
		return false;
	}
} window;

// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Check message
	switch (uMsg)
	{
		// Key pressed
	case WM_KEYDOWN:
		keyboard.isHeld[keyboard.lastKeyPressed = wParam] = keyboard.isPressed[wParam] = true;
		return 0;

		// Key released
	case WM_KEYUP:
		keyboard.isHeld[wParam] = false;
		keyboard.isReleased[wParam] = true;
		return 0;

		// Mouse moved
	case WM_MOUSEMOVE:
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);
		return 0;

		// Mouse wheel rotated
	case WM_MOUSEWHEEL:
		mouse.isWheelRotated[HIWORD(wParam) == WHEEL_DELTA ? MOUSE_WHEEL_UP : MOUSE_WHEEL_DOWN] = true;
		return 0;

		// Left mouse button pressed
	case WM_LBUTTONDOWN:
		mouse.isHeld[MOUSE_LEFT] = mouse.isPressed[MOUSE_LEFT] = true;
		return 0;

		// Right mouse button pressed
	case WM_RBUTTONDOWN:
		mouse.isHeld[MOUSE_RIGHT] = mouse.isPressed[MOUSE_RIGHT] = true;
		return 0;

		// Middle mouse button pressed
	case WM_MBUTTONDOWN:
		mouse.isHeld[MOUSE_MIDDLE] = mouse.isPressed[MOUSE_MIDDLE] = true;
		return 0;

		// Left mouse button released
	case WM_LBUTTONUP:
		mouse.isHeld[MOUSE_LEFT]     = false;
		mouse.isReleased[MOUSE_LEFT] = true;
		return 0;

		// Right mouse button released
	case WM_RBUTTONUP:
		mouse.isHeld[MOUSE_RIGHT]     = false;
		mouse.isReleased[MOUSE_RIGHT] = true;
		return 0;

		// Middle mouse button released
	case WM_MBUTTONUP:
		mouse.isHeld[MOUSE_MIDDLE]     = false;
		mouse.isReleased[MOUSE_MIDDLE] = true;
		return 0;

		// Window focus
	case WM_ACTIVATE:
		window.hasFocus = !HIWORD(wParam);
		return 0;

		// System command
	case WM_SYSCOMMAND:
		if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER)
			return 0;
		break;

		// Window resized
	case WM_SIZE:
		window.width  = LOWORD(lParam);
		window.height = HIWORD(lParam);
		window.update();
		return 0;

		// Window moved
	case WM_MOVE:
		window.x = LOWORD(lParam);
		window.y = HIWORD(lParam);
		return 0;

		// Window close
	case WM_CLOSE:
		Engine::game = false;
		return 0;
	}

	// ���������� ��� �������������� ���������
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}