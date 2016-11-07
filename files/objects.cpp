enum GM_objectId : unsigned int
{
	GM_OBJECT_ID_o_world,
	GM_OBJECT_ID_o_hero
};

#include "../object.cpp"

#include "o_o_world.h"
#include "o_o_hero.h"

GM_object *objectCreate(float x, float y, float z, GM_object *GM_ptr)
{
	GM_object *GM_new = NULL;
	if (GM_ptr == o_world) GM_new = new GM_OBJECT_o_world(x,y,z);
	if (GM_ptr == o_hero) GM_new = new GM_OBJECT_o_hero(x,y,z);
	GM_new->GM_insert(GM_ptr);
	return GM_new;
}

#include "o_o_world.cpp"
#include "o_o_hero.cpp"
