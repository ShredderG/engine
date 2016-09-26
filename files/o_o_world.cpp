GM_OBJECT_o_world::GM_OBJECT_o_world(float GM_x, float GM_y, float GM_z)
{
GM_count++;
o_world = this;
priority = 0;
solid = false;
persistent = false;
x = GM_x;
y = GM_y;
z = GM_z;
for(int i = 0; i < size; i++)
{
	for(int j = 0; j < size; j++)
	{
		xx[i][j] = 2;
		xy[i][j] = 2;
		yx[i][j] = 2;
		yy[i][j] = 2;
		floor[i][j] = 0;
		wall[i][j] = 0;
	}
}

mkrm(1, 1, 10, 8, 1, 2, 1, 2);
mkrm(2, 9, 9, 13, 0, 1, 1, 2);  


}

GM_OBJECT_o_world::~GM_OBJECT_o_world()
{
if (o_world == this)
{
	if (GM_left)  if (GM_left->GM_id()  == GM_id()) o_world = (GM_OBJECT_o_world*) GM_left;
	if (GM_right) if (GM_right->GM_id() == GM_id()) o_world = (GM_OBJECT_o_world*) GM_right;
	if (o_world == this) o_world = (GM_OBJECT_o_world*) GM_id();
}
}

void GM_OBJECT_o_world::destroy()
{
if (!GM_active) return;
GM_count--;
GM_active = false;

}

void GM_OBJECT_o_world::GM_step()
{

}

void GM_OBJECT_o_world::GM_draw()
{
t_world.set();
glBegin(GL_QUADS);
glColor4f(1, 1, 1, 1);

int width = 10,
x1 = o_hero->x - width,
y1 = o_hero->y - width,
x2 = ceil(o_hero->x) + width,
y2 = ceil(o_hero->y) + width;
if (x1 < 0) x1 = 0;
if (y1 < 0) y1 = 0;
if (x2 > size) x2 = size;
if (y2 > size) y2 = size;

for(int i = x1, ki = i - 1, ik = i + 1; i < x2; ki++, i = ik++)
{
	for(int j = y1, kj = j - 1, jk = j + 1; j < y2; kj++, j = jk++)
	{
		// floor
		uchar flr = floor[i][j];
		uchar wll = wall[i][j];
		if (flr != 0)
		{
			glColor4f(1, 1, 1, 1);
			glTexCoord2f(GM_texture::current->x1[flr], GM_texture::current->y1[flr]); glVertex3f(i,  j,  xx[i][j]);
			glTexCoord2f(GM_texture::current->x1[flr], GM_texture::current->y2[flr]); glVertex3f(ik, j,  yx[i][j]);
			glTexCoord2f(GM_texture::current->x2[flr], GM_texture::current->y2[flr]); glVertex3f(ik, jk, yy[i][j]);
			glTexCoord2f(GM_texture::current->x2[flr], GM_texture::current->y1[flr]); glVertex3f(i,  jk, xy[i][j]);
		}
		
		// wall
		if (wll != 0)
		{
			glColor4f(0.6, 0.6, 0.6, 1);
			if (i != zero) drawWall(i,  j,  xx[i][j], yx[ki][j], i,  jk, xy[i][j], yy[ki][j], wll);
			if (i != size) drawWall(ik, jk, yy[i][j], xy[ik][j], ik, j,  yx[i][j], xx[ik][j], wll);
			glColor4f(0.8, 0.8, 0.8, 1);
			if (j != zero) drawWall(ik, j,  yx[i][j], yy[i][kj], i,  j,  xx[i][j], xy[i][kj], wll);
			if (j != size) drawWall(i,  jk, xy[i][j], xx[i][jk], ik, jk, yy[i][j], yx[i][jk], wll);
		}
	}
}

glEnd(); 
}

uint GM_OBJECT_o_world::GM_id()
{
return GM_OBJECT_ID_o_world;
}