struct GM_OBJECT_o_world : public Object
{
const static int size = 2048;
const static int zero = 0;

float
xx[size][size],
xy[size][size],
yx[size][size],
yy[size][size];
uchar floor[size][size], wall[size][size];
/*
float getZ(float x, float y)
{
	if (x < 0 || y < 0) return 255;
	if (i >= size || j >= size) return 255;
	int i = xx, j = yy;
	
	uchar type = max(max(x[0][i][j], w[0][i][j]), max(y[0][i][j], h[0][i][j]));
	
	return type == w[0][i][j] || type == y[0][i][j] ? xx + yy < 8 ?
	y[0][i][j] + (w[0][i][j] - x[0][i][j]) * xx / 8 + (x[0][i][j] - y[0][i][j]) * (1 - yy / 8) :
	w[0][i][j] + (h[0][i][j] - w[0][i][j]) * yy / 8 + (y[0][i][j] - h[0][i][j]) * (1 - xx / 8) : xx < yy?
	x[0][i][j] + (y[0][i][j] - x[0][i][j]) * yy / 8 + (h[0][i][j] - y[0][i][j]) * xx / 8:
	x[0][i][j] + (w[0][i][j] - x[0][i][j]) * xx / 8 + (h[0][i][j] - w[0][i][j]) * yy / 8;
}
*/
void drawWall(int x1, int y1, float z1top, float z1bot, int x2, int y2, float z2top, float z2bot, uchar image)
{
	if (z1top > z1bot || z2top > z2bot)
	{
		float h1 = (texture.y2[image] - texture.y1[image]) * (z1top - z1bot);
		float h2 = (texture.y2[image] - texture.y1[image]) * (z2top - z2bot);
		glTexCoord2f(texture.x2[image], texture.y1[image] + h1); glVertex3f(x1, y1, z1bot);
		glTexCoord2f(texture.x2[image], texture.y1[image]); glVertex3f(x1, y1, z1top);
		glTexCoord2f(texture.x1[image], texture.y1[image]); glVertex3f(x2, y2, z2top);
		glTexCoord2f(texture.x1[image], texture.y1[image] + h2); glVertex3f(x2, y2, z2bot);
	}
}

void mkrm(int x1, int y1, int x2, int y2, int z1, int z2, int flr, int wll)
{
	for(int i = x1; i <= x2; i++)
	{
		xy[i][y1 - 1] = yy[i][y1 - 1] = yx[i][y2 + 1] = xx[i][y2 + 1] = z2;
		wall[i][y1 - 1] = wall[i][y2 + 1] = wll;
	}
	
	for(int j = y1; j <= y2; j++)
	{
		yx[x1 - 1][j] = yy[x1 - 1][j] = xx[x2 + 1][j] = xy[x2 + 1][j] = z2;
		wall[x1 - 1][j] = wall[x2 + 1][j] = wll;
	}
	
	yy[x1 - 1][y1 - 1] = xy[x2 + 1][y1 - 1] = yx[x1 - 1][y2 + 1] = xx[x2 + 1][y2 + 1] = z2;
	wall[x1 - 1][y1 - 1] = wall[x2 + 1][y1 - 1] = wall[x1 - 1][y2 + 1] = wall[x2 + 1][y2 + 1] = wll;
	
	for(int i = x1; i <= x2; i++)
	{
		for(int j = y1; j <= y2; j++)
		{
			xx[i][j] = z1;
			xy[i][j] = z1;
			yx[i][j] = z1;
			yy[i][j] = z1;
			floor[i][j] = flr;
		}
	}
}

	GM_OBJECT_o_world(float GM_x, float GM_y, float GM_z);
	void GM_destructor();
	void GM_step();
	void GM_draw();
	ObjectId GM_id();
} *o_world = (GM_OBJECT_o_world*) GM_OBJECT_ID_o_world;
