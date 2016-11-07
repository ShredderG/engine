struct GM_OBJECT_o_hero : public GM_object
{
int scale;
float direction;
float direction_need;

bool rotatingMethod;

	GM_OBJECT_o_hero(float GM_x, float GM_y, float GM_z);
	void GM_step();
	void GM_draw();
	void GM_destructor();
	inline GM_objectId GM_id();
} *o_hero = (GM_OBJECT_o_hero*) GM_OBJECT_ID_o_hero;
