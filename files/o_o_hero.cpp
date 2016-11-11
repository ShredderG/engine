GM_OBJECT_o_hero::GM_OBJECT_o_hero(float GM_x, float GM_y, float GM_z)
{
x = GM_x;
y = GM_y;
z = GM_z;
o_hero = this;
zIndex = 1;
isPersistent = true;
scale = 48;

direction = direction_need = 0;
rotatingMethod = 0;
}

void GM_OBJECT_o_hero::GM_destructor()
{

if (o_hero == this)
{
	if (GM_right) if (GM_right->GM_id() == GM_OBJECT_o_hero::GM_id()) o_hero = (GM_OBJECT_o_hero*) GM_right;
	if (o_hero == this) o_hero = (GM_OBJECT_o_hero*) GM_OBJECT_o_hero::GM_id();
}
}

void GM_OBJECT_o_hero::GM_step()
{
if (rotatingMethod == 0)
{
	if (keyboard.isPressed['Q']) direction_need -= 45;
	if (keyboard.isPressed['E']) direction_need += 45;
	
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
	if (keyboard.isPressed['Q'])
	{
		direction_need -= 45;
		direction_need -= 2;
	}
	if (keyboard.isPressed['E'])
	{
		direction_need += 45;
		direction_need += 2;
	}
}

float dir = NAN;
// Обработка клавиатуры
if (keyboard.isHeld['W'])
{
	if (!keyboard.isHeld['S'])
	{
		dir = direction;
		if (keyboard.isHeld['A']) dir += 45;
		if (keyboard.isHeld['D']) dir -= 45;
	}
}
else if (keyboard.isHeld['S'])
{
	dir = direction + 180;
	if (keyboard.isHeld['A']) dir -= 45;
	if (keyboard.isHeld['D']) dir += 45;
}
else if (keyboard.isHeld['D'])
{
	if (!keyboard.isHeld['A']) dir = direction - 90;
}
else if (keyboard.isHeld['A']) dir = direction + 90;

if (dir != NAN)
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

ObjectId GM_OBJECT_o_hero::GM_id()
{
return GM_OBJECT_ID_o_hero;
}