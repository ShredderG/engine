struct GM_OBJECT_o_hero : public Object
{
int scale;
float direction;
float direction_need;

bool rotatingMethod;

	GM_OBJECT_o_hero(float GM_x, float GM_y, float GM_z);
	void GM_destructor();
	void GM_step();
	void GM_draw();
	ObjectId GM_id();
} *o_hero = (GM_OBJECT_o_hero*) GM_OBJECT_ID_o_hero;
