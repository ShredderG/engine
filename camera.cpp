// Camera
namespace Camera {
	float x      = 0.0;
	float y      = 0.0;
	float z      = 0.0;
	float xAngle = 0.0;
	float yAngle = 0.0;
	float zAngle = 0.0;
	float zNear  = 0.01;
	float zFar   = 512.0;
	float fov    = 60.0;

	// Rotate world
	void update() {
		glLoadIdentity();
		glRotatef(-yAngle, 1.0, 0.0, 0.0);
		glRotatef(-zAngle, 0.0, 1.0, 0.0);
		glRotatef(-xAngle, 0.0, 0.0, 1.0);
		glTranslatef(-x, -y, -z);
	}

	// Set camera state
	void set(float _x, float _y, float _z, float _xAngle, float _yAngle = 90.0, float _zAngle = 0.0) {
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

	// 2d mode
	void set2d(int x1, int y1, int x2, int y2) {
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
}