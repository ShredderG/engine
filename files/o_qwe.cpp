// Constructor
OBJECT_qwe::OBJECT_qwe() {

}

// Destructor
void OBJECT_qwe::GAME_kill() {

	destroy((Object**)&qwe);
}

// Step
void OBJECT_qwe::GAME_step() {

}

// Draw
void OBJECT_qwe::GAME_draw() {

}

// id
ObjectId OBJECT_qwe::GAME_id() const {
	return ObjectId::qwe;
}