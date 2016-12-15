struct Camera
{
	float 
		x, xAngle,
		y, yAngle,
		z, zAngle,
		zNear, zFar,
		fov;

	Camera() :
		x(0.0), xAngle(0.0),
		y(0.0), yAngle(0.0),
		z(0.0), zAngle(0.0),
		zNear(0.5), zFar(512.0),
		fov(60.0)
	{
	}

	void set(float xNew, float yNew, float zNew, float xAngleNew, float yAngleNew, float zAngleNew)
	{
		while (xAngleNew >= 360.0) xAngleNew -= 360.0;
		while (yAngleNew >= 360.0) yAngleNew -= 360.0;
		while (zAngleNew >= 360.0) zAngleNew -= 360.0;
		while (xAngleNew < 0.0) xAngleNew += 360.0;
		while (yAngleNew < 0.0) yAngleNew += 360.0;
		while (zAngleNew < 0.0) zAngleNew += 360.0;

		x = xNew;
		y = yNew;
		z = zNew;
		xAngle = xAngleNew - 90.0;
		yAngle = yAngleNew;
		zAngle = zAngleNew;

		update();
	}

	void update()
	{
		glLoadIdentity();
		glRotatef(-yAngle, 1.0, 0.0, 0.0); // вверх-вниз
		glRotatef(-zAngle, 0.0, 1.0, 0.0); // вбок-невбок
		glRotatef(-xAngle, 0.0, 0.0, 1.0); // влево-вправо
		glTranslatef(-x, -y, -z);
	}

	void set2d(short x1, short y1, short x2, short y2)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glOrtho(x1, x2, y2, y1, 0.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void set3d()
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		update();
	}
} camera;