#ifdef GM_EDITOR


// calculate the cofactor of element (row,col)
void GetMinor(float **src, float **dest, int row, int col, int order)
{
	int colCount = 0, rowCount = 0;

	for (int i = 0; i < order; i++)
	{
		if (i != row)
		{
			colCount = 0;
			for (int j = 0; j < order; j++)
			{
				if (j != col)
				{
					dest[rowCount][colCount] = src[i][j];
					colCount++;
				}
			}
			rowCount++;
		}
	}
}

// Calculate the determinant recursively.
double CalcDeterminant(float **mat, int order)
{
	if (order == 1)
		return mat[0][0];

	float det = 0;

	float **minor = new float*[order - 1];
	for (int i = 0; i < order - 1; i++)
		minor[i] = new float[order - 1];

	for (int i = 0; i < order; i++)
	{
		GetMinor(mat, minor, 0, i, order);
		det += pow(-1.0, i) * mat[0][i] * CalcDeterminant(minor, order - 1);
	}

	for (int i = 0; i < order - 1; i++)
		delete[] minor[i];
	delete[] minor;

	return det;
}

// matrix inversioon
// the result is put in Y
void MatrixInversion(float **A, int order, float **Y)
{
	// get the determinant of a
	double det = 1.0 / CalcDeterminant(A, order);

	// memory allocation
	float *temp = new float[(order - 1)*(order - 1)];
	float **minor = new float*[order - 1];
	for (int i = 0; i < order - 1; i++)
		minor[i] = temp + (i*(order - 1));

	for (int j = 0; j < order; j++)
	{
		for (int i = 0; i < order; i++)
		{
			// get the co-factor (matrix) of A(j,i)
			GetMinor(A, minor, j, i, order);
			Y[i][j] = det * CalcDeterminant(minor, order - 1);
			if ((i + j) % 2 == 1) Y[i][j] = -Y[i][j];
		}
	}

	// release memory
	delete[] temp;
	delete[] minor;
}

void drawSprite(GM_texture &texture, ushort image, short x, short y)
{
	if (image > texture.count) return;
	texture.set();

	glBegin(GL_QUADS);
	glTexCoord2f(texture.x1[image], texture.y1[image]); glVertex2f(x, y);
	glTexCoord2f(texture.x1[image], texture.y2[image]); glVertex2f(x, y + texture.ySize);
	glTexCoord2f(texture.x2[image], texture.y2[image]); glVertex2f(x + texture.xSize, y + texture.ySize);
	glTexCoord2f(texture.x2[image], texture.y1[image]); glVertex2f(x + texture.xSize, y);
	glEnd();
}

void drawText(GM_texture &font, ushort x, ushort y, string text)
{
	font.set();
	ushort xStart = x,
		xEnd = x + font.xSize,
		yEnd = y + font.ySize;
	glBegin(GL_QUADS);
	for (int i = 0; i < text.length(); i++)
	{
		uchar image = text[i];
		glTexCoord2f(font.x1[image], font.y1[image]); glVertex2f(x, y);
		glTexCoord2f(font.x1[image], font.y2[image]); glVertex2f(x, yEnd);
		glTexCoord2f(font.x2[image], font.y2[image]); glVertex2f(xEnd, yEnd);
		glTexCoord2f(font.x2[image], font.y1[image]); glVertex2f(xEnd, y);

		if (text[i] == '\n')
		{
			y += font.ySize * 1.5;
			yEnd += font.ySize * 1.5;
			x = xStart;
			xEnd = x + font.xSize;
		}
		else
		{
			x = xEnd;
			xEnd += font.xSize;
		}
	}
	glEnd();
}

GM_editor::GM_editor()
{
	x = y = z = 16;
	dx = 225;
	dy = 45;

	zPlace = 0;
	size = 1024;
	grid = 8;

	loaded = false;
}

void GM_editor::step()
{
	if (!loaded)
	{
		//texture = new GM_texture(64, 64, 64, 64, &GM_data[0]);
		//texture->load();
		mouse.move(window.width / 2, window.height / 2);
		mx = mouse.x;
		my = mouse.y;
		loaded = true;
	}

	if (!mouse.visible)
	{
		// move camera
		float speed = keyboard.check[VK_SHIFT] ? grid : 1;
		dx -= (mouse.x - mx) / 2.0;
		dy -= (mouse.y - my) / 2.0;

		if (dy < 0) dy = 0;
		if (dx < 0) dx += 360;
		if (dy >= 180) dy = 180;
		if (dx >= 360) dx -= 360;

		mouse.move(window.width / 2, window.height / 2);
		mx = mouse.x;
		my = mouse.y;

		float move = -999;
		if (keyboard.check['W'] || keyboard.check[VK_UP])
		{
			move = dx;
			if (keyboard.check['A'] || keyboard.check[VK_RIGHT]) move += 45;
			if (keyboard.check['D'] || keyboard.check[VK_LEFT]) move -= 45;
		}
		else if (keyboard.check['S'] || keyboard.check[VK_DOWN])
		{
			move = dx - 180;
			if (keyboard.check['A'] || keyboard.check[VK_RIGHT]) move -= 45;
			if (keyboard.check['D'] || keyboard.check[VK_LEFT]) move += 45;
		}
		else if (keyboard.check['A'] || keyboard.check[VK_RIGHT]) move = dx + 90;
		else if (keyboard.check['D'] || keyboard.check[VK_LEFT]) move = dx - 90;

		if (move != -999)
		{
			x += stepX(speed, move);
			y += stepY(speed, move);
		}

		if (keyboard.check[VK_SPACE]) z += speed;
		if (keyboard.check[VK_CONTROL]) z -= speed;

		if (mouse.check[MOUSE_WHEEL_UP]) zPlace += grid;
		if (mouse.check[MOUSE_WHEEL_DOWN])
		{
			zPlace -= grid;
			if (zPlace < 0) zPlace = 0;
		}

		// show cursor
		if (mouse.pressed[MOUSE_RIGHT])
			mouse.show();
	}
	else
	{
		// hide cursor
		if (mouse.pressed[MOUSE_RIGHT])
		{
			mouse.move(window.width / 2, window.height / 2);
			mx = mouse.x;
			my = mouse.y;
			mouse.hide();
		}
	}

	// ctrl + s
	if (keyboard.check[VK_CONTROL])
	{
		if (keyboard.pressed['S'])
		{
			int count = 0;
			FILE *f = fopen("file.txt", "w");
			for (GM_object *ptr = GM_list; ptr; ptr = ptr->GM_right)
			{
				count++;
				fprintf(f, "x%d=%f\n", count, ptr->x);
				fprintf(f, "y%d=%f\n", count, ptr->y);
				fprintf(f, "z%d=%f\n", count, ptr->z);
				fprintf(f, "o%d=%f\n", count, ptr->GM_id());
			}
			fprintf(f, "count=%d", count);
			fclose(f);
		}
	}
}

void GM_editor::draw()
{
	view.set(x, y, z, dx, dy, 0);

	notex.set();
	{
		//grid
		glBegin(GL_LINES);
		glColor4f(1, 1, 1, 0.5);
		for (int i = 0; i < size / grid; i++)
		{
			float pos = i * grid;
			glVertex3f(pos, 0, zPlace);
			glVertex3f(pos, size, zPlace);
			glVertex3f(0, pos, zPlace);
			glVertex3f(size, pos, zPlace);
		}
		glEnd();

		// coordinate xyz
		glBegin(GL_LINES);
		// x
		glColor4f(0, 1, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(size, 0, 0);
		// y
		glColor4f(1, 0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, size, 0);
		// z
		glColor4f(0, 0, 1, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, size);
		glEnd();
	}

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x0101);
	{
		// coordinate xyz
		glBegin(GL_LINES);
		// -x
		glColor4f(0, 1, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(-size, 0, 0);
		// -y
		glColor4f(1, 0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, -size, 0);
		// -z
		glColor4f(0, 0, 1, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, -size);
		glEnd();
	}
	glDisable(GL_LINE_STIPPLE);

	// RAY
	GLdouble projection_matrix[16];
	GLdouble model_matrix[16];

	glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, model_matrix);

	float
		rx = ((2.0 * mouse.x) / window.width - 1.0) / projection_matrix[0],
		ry = (1.0 - (2.0 * mouse.y) / window.height) / projection_matrix[5];

	float **_model_matrix = new float*[4];
	float **_model_matrix_new = new float*[4];
	for (int i = 0; i < 4; i++)
	{
		_model_matrix[i] = new float[4];
		_model_matrix_new[i] = new float[4];
	}

	for (int index = 0, j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			_model_matrix[i][j] = model_matrix[index++];

	MatrixInversion(_model_matrix, 4, _model_matrix_new);

	for (int index = 0, j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			model_matrix[index++] = _model_matrix_new[i][j];

	for (int i = 0; i < 4; i++)
	{
		delete[] _model_matrix[i];
		delete[] _model_matrix_new[i];
	}
	delete[] _model_matrix;
	delete[] _model_matrix_new;

	float xx = model_matrix[0] * rx + model_matrix[4] * ry - model_matrix[8];
	float yy = model_matrix[1] * rx + model_matrix[5] * ry - model_matrix[9];
	float zz = model_matrix[2] * rx + model_matrix[6] * ry - model_matrix[10];

	float vx = pointDirection(0, 0, xx, yy);
	float vy = pointDirection(0, 0, 0, xx, yy, zz);

	float
		distance = (zPlace - z) / stepZ(1, vx, vy),
		xPlace = x + stepX(distance, vx, vy),
		yPlace = y + stepY(distance, vx, vy);

	if (!keyboard.check[VK_SHIFT])
	{
		if (xPlace < 0) xPlace -= grid;
		if (yPlace < 0) yPlace -= grid;
		xPlace = int(xPlace / grid) * grid;
		yPlace = int(yPlace / grid) * grid;
	}
	else
	{
		xPlace -= grid / 2;
		yPlace -= grid / 2;
	}

	glColor4f(1, 1, 1, 1);

	if (mouse.check[MOUSE_LEFT])
	{
		objectCreate(xPlace, yPlace, zPlace, q1w2e3);
	}
	
	glColor4f(1, 1, 1, 1);
	drawBox(xPlace, yPlace, zPlace, grid, grid, grid);

	for (GM_object *ptr = GM_list; ptr; ptr = ptr->GM_right)
		if (ptr->GM_active)
		{
			//ptr->drawBBox();
		}

	//with(q1w2e3)
	//{
	//	V.add(q1w2e3->x,q1w2e3->y,q1w2e3->z, true);
	///}
	//V.make();
	V.draw();

	// 2d
	view.set2d(0, 0, window.width, window.height);
	{
		glColor4f(1, 1, 1, 1);
		string text =
			"x=" + strf(xx) +
			"\ny=" + strf(yy) +
			"\nz=" + strf(dy) +
			"\ncount=" + str(objectCount(q1w2e3));
		drawText(font, 16, 16, text);
	}
	view.set3d();
	// 3d

	

}

#endif