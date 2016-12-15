struct Room
{
private:
	void(*function_)()  = nullptr;
	bool hasTransition_ = false;

public:
	// Do transition to room if needed
	bool doTransition()
	{
		if (hasTransition_) {
			function_();
			hasTransition_ = false;
			return true;
		}
		return false;
	}

	// Change current room
	void operator = (void(*function)())
	{
		hasTransition_ = true;
		function_ = function;
		for (Engine::Object *ptr = Engine::objectList; ptr; ptr = ptr->GAME_right) {
			if (!ptr->isPersistent) {
				ptr->destroy();
			}
		}
	}

	bool operator == (void(*function)())
	{
		return function_ == function;
	}

	bool operator != (void(*function)())
	{
		return function_ != function;
	}
} room;