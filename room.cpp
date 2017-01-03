// Room
class Room {
private:
	void(*function_)()  = nullptr;
	bool hasTransition_ = false;

public:
	// Do transition to room if needed
	bool doTransition() {
		if (hasTransition_) {
			function_();
			hasTransition_ = false;
			return true;
		}
		return false;
	}

	// Change current room
	void operator = (void(*function)()) {
		hasTransition_ = true;
		function_ = function;

		// Destroy non-persistent objects
		Engine::Object* ptr = Engine::objectList;
		while (ptr != nullptr) {
			if (!ptr->isPersistent) ptr->destroy();
			ptr = ptr->GAME_right;
		}
	}

	bool operator == (void(*function)()) {
		return function_ == function;
	}

	bool operator != (void(*function)()) {
		return function_ != function;
	}
} room;