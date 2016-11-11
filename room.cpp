struct Room
{
private:
	void(*create_)();
	bool transition_;

public:
	Room() :
		create_(NULL),
		transition_(false)
	{
	}

	bool operator == (void(*function)())
	{
		return create_ == function;
	}

	bool operator != (void(*function)())
	{
		return create_ != function;
	}

	// Change current room
	void operator = (void(*function)())
	{
		transition_ = true;
		create_ = function;
		for (Object *ptr = GM_list; ptr; ptr = ptr->GM_right) {
			if (!ptr->isPersistent) {
				ptr->destroy();
			}
		}
	}

	// Do transition to room if needed
	inline bool doTransition()
	{
		if (transition_) {
			create_();
			transition_ = false;
			return true;
		}
		return false;
	}
} room;