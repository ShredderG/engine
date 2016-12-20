enum class ObjectId : ushort
{
	qwe 
};

#include "../object.cpp"

#include "o_qwe.h"

Engine::Object* objectCreate(float x, float y, float z, Engine::Object* GAME_ptr)
{
	Engine::Object* GAME_new = nullptr;
	if (GAME_ptr == qwe) GAME_new = new GAME_OBJECT_qwe(x, y, z);
	GAME_new->GAME_insert(GAME_ptr);
	return GAME_new;
}

#include "../room.cpp"
#include "rooms.cpp"

#include "o_qwe.cpp"
