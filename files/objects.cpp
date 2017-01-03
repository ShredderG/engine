// Object id list
enum class ObjectId : ushort
{
	qwe 
};

// Base object and functions
#include "../object.cpp"

// .h files
#include "o_qwe.h"

// Create object
Engine::Object *_objectCreate(float x, float y, float z, Engine::Object** object) {
	Engine::Object* objectNew = nullptr;
	Engine::Object* objectPtr = *object;
	if (objectPtr == ::qwe) objectNew = new OBJECT_qwe();
	objectNew->GAME_insert(objectPtr);
	objectNew->x = x;
	objectNew->y = y;
	objectNew->z = z;
	return *object = objectNew;
}

// Rooms
#include "../room.cpp"
#include "rooms.cpp"

// .cpp files
#include "o_qwe.cpp"
