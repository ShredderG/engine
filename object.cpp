// Конструкция with
#define with(object)                                                     \
for (GM_OBJECT_##object *object = ::object;                              \
	object > (GM_OBJECT_##object*)0x0000FFFF;                            \
	object = (GM_OBJECT_##object*)object->GM_right)                      \
	if (object->GM_id() != ::object->GM_OBJECT_##object::GM_id()) break; \
	else if (object->GM_active)

// Существует ли экземпляр объекта
#define objectExists(object) (object > (GM_object*)0x0000FFFF)

struct GM_object
{
	GM_object *GM_left, *GM_right;
	float x, y, z;
	bool GM_active, solid, persistent;
	int priority;

	// constructor
	GM_object() :
		GM_active(true),
		solid(false),
		persistent(false),
		priority(0)
	{
	}

	// destructor
	~GM_object()
	{
		(GM_left ? GM_left->GM_right : GM_list) = GM_right;
		if (GM_right) GM_right->GM_left = GM_left;
	}

	inline void destroy()
	{
		GM_active = false;
	}

	// insert object to the object list
	inline void GM_insert(GM_object *ptr)
	{
		GM_object *before = NULL, *after = ptr;

		if (objectExists(after))
			before = after->GM_left;
		else
			after = GM_list;

		while (after)
		{
			if (priority < after->priority)
			{
				before = after;
				after = after->GM_right;
			}
			else break;
		}

		GM_left = before;
		GM_right = after;

		(GM_left ? GM_left->GM_right : GM_list) = this;
		if (GM_right) GM_right->GM_left = this;
	}

	// Виртуальные функции
	virtual void GM_destructor() = 0;
	virtual void GM_step() = 0;
	virtual void GM_draw() = 0;
	virtual inline GM_objectId GM_id() = 0;

} *GM_list = NULL;

// Delete everything
inline void GM_deleteObjects()
{
	for (GM_object *ptr = GM_list, *ptr_next = ptr ? ptr->GM_right : NULL; ptr; ptr = ptr_next, ptr_next = ptr ? ptr->GM_right : NULL)
		delete ptr;
}

// Global step
inline void GM_step()
{
	mouse.update(window.hWnd, window.width, window.height);

	for (GM_object *ptr = GM_list; ptr; ptr = ptr->GM_right)
		if (ptr->GM_active) ptr->GM_step();

	for (GM_object *ptr = GM_list, *ptr_next = ptr ? ptr->GM_right : NULL; ptr; ptr = ptr_next, ptr_next = ptr ? ptr->GM_right : NULL)
		if (!ptr->GM_active)
		{
			ptr->GM_destructor();
			delete ptr;
		}

	keyboard.reset();
	mouse.reset();
}

// Global draw
inline void GM_draw()
{
	shader.gl_UseProgram(shader.program);
	shader.gl_BindFramebuffer(GL_FRAMEBUFFER_EXT, shader.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (GM_object *ptr = GM_list; ptr; ptr = ptr->GM_right)
		ptr->GM_draw();

	shader.gl_BindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	view.set2d(0,0,window.width, window.height);
	shader.gl_UseProgram(shader.program2);

	glBindTexture(GL_TEXTURE_2D, shader.texture);
	glBegin(GL_QUADS);
	glColor4f(1, 1, 1, 1);
	glTexCoord2f(0, window.height / (float)shader.SIZE); glVertex2f(0, 0);
	glTexCoord2f(0, 0); glVertex2f(0, window.height);
	glTexCoord2f(window.width / (float)shader.SIZE, 0); glVertex2f(window.width, window.height);
	glTexCoord2f(window.width / (float)shader.SIZE, window.height / (float)shader.SIZE); glVertex2f(window.width, 0);
	glEnd();

	/*
	shader.gl_UseProgram(0);
	glBindTexture(GL_TEXTURE_2D, shader.rbo);
	glBegin(GL_QUADS);
	glColor4f(1, 1, 1, 1);
	glTexCoord2f(0, window.height / (float)shader.SIZE); glVertex2f(0, 0);
	glTexCoord2f(0, 0); glVertex2f(0, window.height / 2);
	glTexCoord2f(window.width / (float)shader.SIZE, 0); glVertex2f(window.width / 2, window.height / 2);
	glTexCoord2f(window.width / (float)shader.SIZE, window.height / (float)shader.SIZE); glVertex2f(window.width / 2, 0);
	glEnd();
	*/

	view.set3d();

	SwapBuffers(window.hDC);
}