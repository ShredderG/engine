// Camera
class Camera {
public:
	float x;
	float y;
	float z;
	float xAngle;
	float yAngle;
	float zAngle;
	float zNear;
	float zFar;
	float fov;

	Camera() :
		x(0.0),
		y(0.0),
		z(0.0),
		xAngle(0.0),
		yAngle(0.0),
		zAngle(0.0),
		zNear(0.5),
		zFar(512.0),
		fov(60.0) {
		// nothing here
	}

	// Set camera state
	void set(float _x, float _y, float _z, float _xAngle, float _yAngle, float _zAngle) {
		while (_xAngle >= 360.0) _xAngle -= 360.0;
		while (_yAngle >= 360.0) _yAngle -= 360.0;
		while (_zAngle >= 360.0) _zAngle -= 360.0;
		while (_xAngle < 0.0) _xAngle += 360.0;
		while (_yAngle < 0.0) _yAngle += 360.0;
		while (_zAngle < 0.0) _zAngle += 360.0;

		x = _x;
		y = _y;
		z = _z;
		xAngle = _xAngle - (float)90.0;
		yAngle = _yAngle;
		zAngle = _zAngle;

		update();
	}

	// Rotate world
	void update() {
		glLoadIdentity();
		glRotatef(-yAngle, 1.0, 0.0, 0.0);
		glRotatef(-zAngle, 0.0, 1.0, 0.0);
		glRotatef(-xAngle, 0.0, 0.0, 1.0);
		glTranslatef(-x, -y, -z);
	}

	// 2d mode
	void set2d(short x1, short y1, short x2, short y2) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glOrtho(x1, x2, y2, y1, 0.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	// 3d mode
	void set3d() {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		update();
	}
} camera;