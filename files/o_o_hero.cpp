GM_OBJECT_o_hero::GM_OBJECT_o_hero(float GM_x, float GM_y, float GM_z)
{
o_hero = this;
priority = 1;
solid = false;
persistent = true;
x = GM_x;
y = GM_y;
z = GM_z;
scale = 50;

direction = direction_need = 0;
rotatingMethod = 0;
}

void GM_OBJECT_o_hero::GM_destructor()
{
if (o_hero == this)
{
	if (GM_left)  if (GM_left->GM_id()  == GM_id()) o_hero = (GM_OBJECT_o_hero*) GM_left;
	if (GM_right) if (GM_right->GM_id() == GM_id()) o_hero = (GM_OBJECT_o_hero*) GM_right;
	if (o_hero == this) o_hero = (GM_OBJECT_o_hero*) GM_id();
}
}
void GM_OBJECT_o_hero::GM_step()
{
if (rotatingMethod == 0)
{
	if (keyboard.pressed['Q']) direction_need -= 45;
	if (keyboard.pressed['E']) direction_need += 45;
	
	if (direction != direction_need)
	{
		direction += (direction_need - direction) / 10.0;
		if (fabs(direction_need - direction) < 0.1)
		{
			direction = direction_need;
			while (direction < 0)
			{
				direction += 360;
				direction_need += 360;
			}
			while (direction >= 360)
			{
				direction -= 360;
				direction_need -= 360;
			}
		}
	}
}
else
{
	if (keyboard.pressed['Q'])
{
direction_need -= 45;
direction_need -= 2;
}
	if (keyboard.pressed['E'])
{
direction_need += 45;
direction_need += 2;
}
}

float dir = - 1;
// Обработка клавиатуры
if (keyboard.check['W'])
{
	if (!keyboard.check['S'])
	{
		dir = direction;
		if (keyboard.check['A']) dir += 45;
		if (keyboard.check['D']) dir -= 45;
	}
}
else if (keyboard.check['S'])
{
	dir = direction + 180;
	if (keyboard.check['A']) dir -= 45;
	if (keyboard.check['D']) dir += 45;
}
else if (keyboard.check['D'])
{
	if (!keyboard.check['A']) dir = direction - 90;
}
else if (keyboard.check['A']) dir = direction + 90; 

if (dir != - 1)
{
	x += stepX(.1, dir);
	y += stepY(.1, dir);
}
}

void GM_OBJECT_o_hero::GM_draw()
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho( - window.width / 2.0, window.width / 2.0, - window.height / 2.0, window.height / 2.0, 0, 1);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glRotatef( - direction + 90, 0, 0, 1);
glTranslatef( - x * scale, - y * scale, - z * scale);

glScalef(scale, scale, scale);

glColor3f(1, 1, 1);

texture = t_hero;
glBegin(GL_QUADS);
glTexCoord2f(texture.x2[0], texture.y2[0]); glVertex3f(x - stepX(0.5, direction + 90), y - stepY(0.5, direction + 90), z);
glTexCoord2f(texture.x2[0], texture.y1[0]); glVertex3f(x - stepX(0.5, direction + 90), y - stepY(0.5, direction + 90), z + 1);
glTexCoord2f(texture.x1[0], texture.y1[0]); glVertex3f(x + stepX(0.5, direction + 90), y + stepY(0.5, direction + 90), z + 1);
glTexCoord2f(texture.x1[0], texture.y2[0]); glVertex3f(x + stepX(0.5, direction + 90), y + stepY(0.5, direction + 90), z);
glEnd(); 
}

inline GM_objectId GM_OBJECT_o_hero::GM_id()
{
return GM_OBJECT_ID_o_hero;
}