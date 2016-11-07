struct GM_room
{
	void(*create)();
	bool transition;

	GM_room() :
		create(NULL),
		transition(false)
	{
	}

	bool operator == (void(*function)())
	{
		return create == function;
	}

	bool operator != (void(*function)())
	{
		return create != function;
	}

	void operator = (void(*function)())
	{
		transition = true;
		create = function;
		for (GM_object *ptr = GM_list; ptr; ptr = ptr->GM_right)
			if (!ptr->persistent) ptr->destroy();
	}

	inline bool transite()
	{
		if (transition)
		{
			create();
			transition = false;
			return true;
		}
		return false;
	}

} room;