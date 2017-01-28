// Prototype WndProc
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// glew Functions
PFNGLCREATEPROGRAMPROC           gl_CreateProgram;
PFNGLDELETEPROGRAMPROC           gl_DeleteProgram;
PFNGLUSEPROGRAMPROC              gl_UseProgram;
PFNGLATTACHSHADERPROC            gl_AttachShader;
PFNGLDETACHSHADERPROC            gl_DetachShader;
PFNGLLINKPROGRAMPROC             gl_LinkProgram;
PFNGLGETPROGRAMIVPROC            gl_GetProgramiv;
PFNGLGETSHADERINFOLOGPROC        gl_GetShaderInfoLog;
PFNGLGETUNIFORMLOCATIONPROC      gl_GetUniformLocation;
PFNGLUNIFORM1IPROC               gl_Uniform1i;
PFNGLUNIFORM2IPROC               gl_Uniform2i;
PFNGLUNIFORM3IPROC               gl_Uniform3i;
PFNGLUNIFORM4IPROC               gl_Uniform4i;
PFNGLUNIFORM1FPROC               gl_Uniform1f;
PFNGLUNIFORM2FPROC               gl_Uniform2f;
PFNGLUNIFORM3FPROC               gl_Uniform3f;
PFNGLUNIFORM4FPROC               gl_Uniform4f;
PFNGLUNIFORMMATRIX4FVPROC        gl_UniformMatrix4fv;
PFNGLGETATTRIBLOCATIONPROC       gl_GetAttribLocation;
PFNGLVERTEXATTRIB1FPROC          gl_VertexAttrib1f;
PFNGLVERTEXATTRIB1FVPROC         gl_VertexAttrib1fv;
PFNGLVERTEXATTRIB2FVPROC         gl_VertexAttrib2fv;
PFNGLVERTEXATTRIB3FVPROC         gl_VertexAttrib3fv;
PFNGLVERTEXATTRIB4FVPROC         gl_VertexAttrib4fv;
PFNGLENABLEVERTEXATTRIBARRAYPROC gl_EnableVertexAttribArray;
PFNGLBINDATTRIBLOCATIONPROC      gl_BindAttribLocation;
PFNGLACTIVETEXTUREPROC           gl_ActiveTexture;
PFNGLCREATESHADERPROC            gl_CreateShader;
PFNGLDELETESHADERPROC            gl_DeleteShader;
PFNGLSHADERSOURCEPROC            gl_ShaderSource;
PFNGLCOMPILESHADERPROC           gl_CompileShader;
PFNGLGETSHADERIVPROC             gl_GetShaderiv;
PFNGLGENFRAMEBUFFERSPROC         gl_GenFramebuffers;
PFNGLBINDFRAMEBUFFERPROC         gl_BindFramebuffer;
PFNGLFRAMEBUFFERTEXTURE2DPROC    gl_FramebufferTexture2D;
PFNGLCHECKFRAMEBUFFERSTATUSPROC  gl_CheckFramebufferStatus;
PFNGLDELETEFRAMEBUFFERSPROC      gl_DeleteFramebuffers;

namespace Window {
	HGLRC     hRC       = nullptr; // Постоянный контекст рендеринга
	HDC       hDC       = nullptr; // Приватный контекст устройства GDI
	HWND      hWnd      = nullptr; // Здесь будет хранится дескриптор окна
	HINSTANCE hInstance = nullptr; // Здесь будет хранится дескриптор приложения
	
	int x;
	int y;
	int width;
	int height;
	string title;
	bool hasFocus;
	bool isFullscreen;

	// Move
	void move(int x, int y) {
		SetWindowPos(hWnd, hWnd, x, y, width, height, SWP_NOZORDER | SWP_NOSIZE);
	}

	// Resize
	void resize(int width, int height) {
		RECT client, window;
		GetClientRect(hWnd, &client);
		GetWindowRect(hWnd, &window);

		SetWindowPos(hWnd, hWnd, x, y,
			width  + window.right  - window.left - client.right,
			height + window.bottom - window.top  - client.bottom,
			SWP_NOZORDER | SWP_NOMOVE);
	}

	// Update
	void update(int width, int height) {
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Calculate window aspect ratio
		gluPerspective(Camera::fov, (float)width / height, Camera::zNear, Camera::zFar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	// Initialization
	void initialize() {
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClearDepth(1.0);

		// vsync
		//typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
		//((PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT"))(1);

		glShadeModel(GL_SMOOTH);
		glEnable(GL_TEXTURE_2D);

		glPolygonMode(GL_BACK, GL_LINE);
		glEnable(GL_CULL_FACE);

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glAlphaFunc(GL_GREATER, (float)0.01);
		glEnable(GL_ALPHA_TEST);

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);

		// glew functions
		gl_CreateProgram           = (PFNGLCREATEPROGRAMPROC)           wglGetProcAddress("glCreateProgram");
		gl_DeleteProgram           = (PFNGLDELETEPROGRAMPROC)           wglGetProcAddress("glDeleteProgram");
		gl_UseProgram              = (PFNGLUSEPROGRAMPROC)              wglGetProcAddress("glUseProgram");
		gl_AttachShader            = (PFNGLATTACHSHADERPROC)            wglGetProcAddress("glAttachShader");
		gl_DetachShader            = (PFNGLDETACHSHADERPROC)            wglGetProcAddress("glDetachShader");
		gl_LinkProgram             = (PFNGLLINKPROGRAMPROC)             wglGetProcAddress("glLinkProgram");
		gl_GetProgramiv            = (PFNGLGETPROGRAMIVPROC)            wglGetProcAddress("glGetProgramiv");
		gl_GetShaderInfoLog        = (PFNGLGETSHADERINFOLOGPROC)        wglGetProcAddress("glGetShaderInfoLog");
		gl_GetUniformLocation      = (PFNGLGETUNIFORMLOCATIONPROC)      wglGetProcAddress("glGetUniformLocation");
		gl_Uniform1i               = (PFNGLUNIFORM1IPROC)               wglGetProcAddress("glUniform1i");
		gl_Uniform2i               = (PFNGLUNIFORM2IPROC)               wglGetProcAddress("glUniform2i");
		gl_Uniform3i               = (PFNGLUNIFORM3IPROC)               wglGetProcAddress("glUniform3i");
		gl_Uniform4i               = (PFNGLUNIFORM4IPROC)               wglGetProcAddress("glUniform4i");
		gl_Uniform1f               = (PFNGLUNIFORM1FPROC)               wglGetProcAddress("glUniform1f");
		gl_Uniform2f               = (PFNGLUNIFORM2FPROC)               wglGetProcAddress("glUniform2f");
		gl_Uniform3f               = (PFNGLUNIFORM3FPROC)               wglGetProcAddress("glUniform3f");
		gl_Uniform4f               = (PFNGLUNIFORM4FPROC)               wglGetProcAddress("glUniform4f");
		gl_UniformMatrix4fv        = (PFNGLUNIFORMMATRIX4FVPROC)        wglGetProcAddress("glUniformMatrix4fv");
		gl_GetAttribLocation       = (PFNGLGETATTRIBLOCATIONPROC)       wglGetProcAddress("glGetAttribLocation");
		gl_VertexAttrib1f          = (PFNGLVERTEXATTRIB1FPROC)          wglGetProcAddress("glVertexAttrib1f");
		gl_VertexAttrib1fv         = (PFNGLVERTEXATTRIB1FVPROC)         wglGetProcAddress("glVertexAttrib1fv");
		gl_VertexAttrib2fv         = (PFNGLVERTEXATTRIB2FVPROC)         wglGetProcAddress("glVertexAttrib2fv");
		gl_VertexAttrib3fv         = (PFNGLVERTEXATTRIB3FVPROC)         wglGetProcAddress("glVertexAttrib3fv");
		gl_VertexAttrib4fv         = (PFNGLVERTEXATTRIB4FVPROC)         wglGetProcAddress("glVertexAttrib4fv");
		gl_EnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glEnableVertexAttribArray");
		gl_BindAttribLocation      = (PFNGLBINDATTRIBLOCATIONPROC)      wglGetProcAddress("glBindAttribLocation");
		gl_ActiveTexture           = (PFNGLACTIVETEXTUREPROC)           wglGetProcAddress("glActiveTexture");
		gl_CreateShader            = (PFNGLCREATESHADERPROC)            wglGetProcAddress("glCreateShader");
		gl_DeleteShader            = (PFNGLDELETESHADERPROC)            wglGetProcAddress("glDeleteShader");
		gl_ShaderSource            = (PFNGLSHADERSOURCEPROC)            wglGetProcAddress("glShaderSource");
		gl_CompileShader           = (PFNGLCOMPILESHADERPROC)           wglGetProcAddress("glCompileShader");
		gl_GetShaderiv             = (PFNGLGETSHADERIVPROC)             wglGetProcAddress("glGetShaderiv");
		gl_GenFramebuffers         = (PFNGLGENFRAMEBUFFERSPROC)         wglGetProcAddress("glGenFramebuffers");
		gl_BindFramebuffer         = (PFNGLBINDFRAMEBUFFERPROC)         wglGetProcAddress("glBindFramebuffer");
		gl_FramebufferTexture2D    = (PFNGLFRAMEBUFFERTEXTURE2DPROC)    wglGetProcAddress("glFramebufferTexture2D");
		gl_CheckFramebufferStatus  = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)  wglGetProcAddress("glCheckFramebufferStatus");
		gl_DeleteFramebuffers      = (PFNGLDELETEFRAMEBUFFERSPROC)      wglGetProcAddress("glDeleteFramebuffers");
	}

	// Set title
	void setTitle(string title) {
		Window::title = title;
		SetWindowText(hWnd, title.c_str());
	}

	// Destroy
	void destroy() {
		if (isFullscreen) {
			ChangeDisplaySettings(nullptr, 0);
		}

		if (hRC) {
			if (!wglMakeCurrent(nullptr, nullptr)) {
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

		hRC       = nullptr;
		hDC       = nullptr;
		hWnd      = nullptr;
		hInstance = nullptr;
	}

	// Create
	bool create(string title, bool fullscreen, int x, int y, int width, int height) {
		Window::title        = title;
		Window::isFullscreen = fullscreen;
		Window::x            = x;
		Window::y            = y;
		Window::width        = width;
		Window::height       = height;
		if (isFullscreen) {
			x = y = 0;
		}

		GLuint   pixelFormat; // Хранит результат после поиска
		WNDCLASS wc;          // Структура класса окна
		DWORD    dwExStyle;   // Расширенный стиль окна
		DWORD    dwStyle;     // Обычный стиль окна
		
		hInstance        = GetModuleHandle(nullptr);           // Считаем дескриптор нашего приложения
		wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Перерисуем и создаём скрытый DC
		wc.lpfnWndProc   = (WNDPROC)WndProc;                   // Процедура обработки сообщений
		wc.cbClsExtra    = 0;                                  // Нет дополнительной информации для окна
		wc.cbWndExtra    = 0;                                  // Нет дополнительной информации для окна
		wc.hInstance     = hInstance;                          // Устанавливаем дескриптор
		wc.hIcon         = LoadIcon(hInstance, IDI_WINLOGO);   // Загружаем иконку по умолчанию
		wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);     // Загружаем указатель мышки
		wc.hbrBackground = nullptr;                            // Фон не требуется для GL
		wc.lpszMenuName  = nullptr;                            // Меню в окне не будет
		wc.lpszClassName = "OpenGL";                           // Устанавливаем имя классу

		// Пытаемся зарегистрировать класс окна
		if (!RegisterClass(&wc)) {
			showMessage("Failed To Register The Window Class.");
			return false;
		}
		
		// Полноэкранный режим?
		if (isFullscreen) {
			DEVMODE dmScreenSettings;                                                     // Режим устройства
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));                       // Очистка для хранения установок
			dmScreenSettings.dmSize       = sizeof(dmScreenSettings);                     // Размер структуры Devmode
			dmScreenSettings.dmPelsWidth  = width;                                        // Ширина экрана
			dmScreenSettings.dmPelsHeight = height;                                       // Высота экрана
			dmScreenSettings.dmBitsPerPel = 32;                                           // Глубина цвета
			dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; // Режим Пикселя

			// Пытаемся установить выбранный режим и получить результат.
			if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
				isFullscreen = false;
			}
		}

		if (isFullscreen || GAME_NOBORDER) {
			dwExStyle = WS_EX_APPWINDOW; // Расширенный стиль окна
			dwStyle   = WS_POPUP;        // Обычный стиль окна
		} else {
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			dwStyle   = WS_OVERLAPPEDWINDOW;
			if (!GAME_MINIMIZE) dwStyle &= ~WS_MINIMIZEBOX;
			if (!GAME_MAXIMIZE) dwStyle &= ~WS_MAXIMIZEBOX;
		}

		// Подбирает окну подходящие размеры
		RECT WindowRect = { 0, 0, width, height };
		AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);

		if (!(hWnd = CreateWindowEx(
			dwExStyle,                           // Расширенный стиль для окна
			"OpenGL",                            // Имя класса
			title.c_str(),                       // Заголовок окна
			WS_CLIPSIBLINGS |                    // Требуемый стиль для окна
			WS_CLIPCHILDREN |                    // Требуемый стиль для окна
			dwStyle,                             // Выбираемые стили для окна
			x, y,                                // Позиция окна
			WindowRect.right  - WindowRect.left, // Вычисление подходящей ширины
			WindowRect.bottom - WindowRect.top,  // Вычисление подходящей высоты
			nullptr,                             // Нет родительского
			nullptr,                             // Нет меню
			hInstance,                           // Дескриптор приложения
			nullptr)))                           // Не передаём ничего до WM_CREATE (???)
		{
			destroy();                               // Восстановить экран
			showMessage("Ошибка при создании окна"); // Ошибка
			return false;                            // Вернуть false
		}

		static PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion   = 1;
		pfd.dwFlags    = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;

		// Проверяем на ошибки
		if (hDC = GetDC(hWnd)) {
			if (pixelFormat = ChoosePixelFormat(hDC, &pfd)) {
				if (SetPixelFormat(hDC, pixelFormat, &pfd)) {
					if (hRC = wglCreateContext(hDC)) {
						if (wglMakeCurrent(hDC, hRC)) {
							ShowWindow(hWnd, SW_SHOW);    // Показать окно
							SetForegroundWindow(hWnd);    // Слегка повысим приоритет
							SetFocus(hWnd);               // Установить фокус клавиатуры на наше окно
							update(width, height);        // Перспектива
							initialize();                 // Прочее
							return true;                  // Окно создано
						} else showMessage("Can't Activate The GL Rendering Context.");
					} else showMessage("Can't Create A GL Rendering Context.");
				} else showMessage("Can't Set The pixelFormat.");
			} else showMessage("Can't Find A Suitable pixelFormat.");
		} else showMessage("Can't Create A GL Device Context.");

		// Ошибка
		destroy();
		return false;
	}

	// Change window mode
	void changeFullscreen() {
		destroy();
		create(title, !isFullscreen, x, y, width, height);
	}
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// Check message
	switch (uMsg) {
		// Key pressed
	case WM_KEYDOWN:
		if (!Keyboard::isHeld[Keyboard::lastPressedKey = wParam]) {
			Keyboard::isHeld[wParam] = Keyboard::isPressed[wParam] = true;
		}
		return 0;

		// Key released
	case WM_KEYUP:
		Keyboard::isHeld[wParam]     = false;
		Keyboard::isReleased[wParam] = true;
		return 0;

		// Mouse moved
	case WM_MOUSEMOVE:
		Mouse::x = LOWORD(lParam);
		Mouse::y = HIWORD(lParam);
		return 0;

		// Mouse wheel rotated
	case WM_MOUSEWHEEL:
		Mouse::isWheelRotated[HIWORD(wParam) == WHEEL_DELTA ? MOUSE_WHEEL_UP : MOUSE_WHEEL_DOWN] = true;
		return 0;

		// Left mouse button pressed
	case WM_LBUTTONDOWN:
		Mouse::isHeld[MOUSE_LEFT] = Mouse::isPressed[MOUSE_LEFT] = true;
		return 0;

		// Right mouse button pressed
	case WM_RBUTTONDOWN:
		Mouse::isHeld[MOUSE_RIGHT] = Mouse::isPressed[MOUSE_RIGHT] = true;
		return 0;

		// Middle mouse button pressed
	case WM_MBUTTONDOWN:
		Mouse::isHeld[MOUSE_MIDDLE] = Mouse::isPressed[MOUSE_MIDDLE] = true;
		return 0;

		// Left mouse button released
	case WM_LBUTTONUP:
		Mouse::isHeld[MOUSE_LEFT]     = false;
		Mouse::isReleased[MOUSE_LEFT] = true;
		return 0;

		// Right mouse button released
	case WM_RBUTTONUP:
		Mouse::isHeld[MOUSE_RIGHT]     = false;
		Mouse::isReleased[MOUSE_RIGHT] = true;
		return 0;

		// Middle mouse button released
	case WM_MBUTTONUP:
		Mouse::isHeld[MOUSE_MIDDLE]     = false;
		Mouse::isReleased[MOUSE_MIDDLE] = true;
		return 0;

		// Window focus
	case WM_ACTIVATE:
		Window::hasFocus = !HIWORD(wParam);
		return 0;

		// System command
	case WM_SYSCOMMAND:
		if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER)
			return 0;
		break;

		// Window resized
	case WM_SIZE:
		Window::width  = LOWORD(lParam);
		Window::height = HIWORD(lParam);
		Window::update(Window::width, Window::height);
		return 0;

		// Window moved
	case WM_MOVE:
		Window::x = LOWORD(lParam);
		Window::y = HIWORD(lParam);
		return 0;

		// Window close
	case WM_CLOSE:
		Engine::game = false;
		return 0;
	}

	// Пересылаем все необработанные сообщения
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}