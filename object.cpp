// With macros
#define with(object)                                                    \
for (GAME_OBJECT_##object *object = ::object;                           \
	object > (GAME_OBJECT_##object*)0x0000FFFF;                         \
	object = (GAME_OBJECT_##object*)object->GAME_right)                 \
	if (object->GAME_id() != ::object->GAME_OBJECT_##object::GAME_id()) \
		break;                                                          \
	else if (object->isActive)

// —уществует ли экземпл€р объекта
#define objectExists(object) (object > (Engine::Object*)0x0000FFFF)

namespace Engine
{
	struct Object
	{
		Object *GAME_left, *GAME_right;
		float x, y, z;
		bool isActive, isSolid, isPersistent;
		int zIndex;

		// constructor
		Object() :
			isActive(true),
			isSolid(false),
			isPersistent(false),
			zIndex(0)
		{
		}

		// destructor
		~Object()
		{
			(GAME_left ? GAME_left->GAME_right : objectList) = GAME_right;
			if (GAME_right) GAME_right->GAME_left = GAME_left;
		}

		inline void destroy()
		{
			isActive = false;
		}

		// insert object to the object list
		void GAME_insert(Object *ptr)
		{
			GAME_left = nullptr;
			GAME_right = ptr;

			if (objectExists(GAME_right)) {
				GAME_left = GAME_right->GAME_left;
			}
			else {
				GAME_right = objectList;
			}

			while (GAME_right) {
				if (zIndex < GAME_right->zIndex) {
					GAME_left  = GAME_right;
					GAME_right = GAME_right->GAME_right;
				}
				else break;
			}

			(GAME_left ? GAME_left->GAME_right : objectList) = this;
			if (GAME_right) GAME_right->GAME_left = this;
		}

		// ¬иртуальные функции
		virtual void     GAME_kill()     = 0;
		virtual void     GAME_step()     = 0;
		virtual void     GAME_draw()     = 0;
		virtual ObjectId GAME_id() const = 0;

	} *objectList = nullptr;

	// Delete everything
	void deleteObjects()
	{
		for (Object *ptr = objectList, *ptr_next = ptr ? ptr->GAME_right : nullptr; ptr; ptr = ptr_next, ptr_next = ptr ? ptr->GAME_right : nullptr) {
			ptr->GAME_kill();
			delete ptr;
		}
	}

	// Global step
	void step()
	{
		for (Object *ptr = objectList; ptr; ptr = ptr->GAME_right) {
			if (ptr->isActive) {
				ptr->GAME_step();
			}
		}

		for (Object *ptr = objectList, *ptr_next = ptr ? ptr->GAME_right : nullptr; ptr; ptr = ptr_next, ptr_next = ptr ? ptr->GAME_right : nullptr) {
			if (!ptr->isActive) {
				ptr->GAME_kill();
				delete ptr;
			}
		}

		keyboard.reset();
		mouse.reset();
	}

	// Global draw
	void draw()
	{
		shader.gl_UseProgram(shader.program);
		shader.gl_BindFramebuffer(GL_FRAMEBUFFER_EXT, shader.fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (Object *ptr = objectList; ptr; ptr = ptr->GAME_right) {
			ptr->GAME_draw();
		}

		shader.gl_UseProgram(shader.program2);
		shader.gl_BindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.set2d(0, 0, window.width, window.height);

		glBindTexture(GL_TEXTURE_2D, shader.texture);
		glBegin(GL_QUADS);
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(0, window.height / (float)shader.SIZE); glVertex2f(0, 0);
		glTexCoord2f(0, 0); glVertex2f(0, window.height);
		glTexCoord2f(window.width / (float)shader.SIZE, 0); glVertex2f(window.width, window.height);
		glTexCoord2f(window.width / (float)shader.SIZE, window.height / (float)shader.SIZE); glVertex2f(window.width, 0);
		glEnd();

		camera.set3d();

		SwapBuffers(window.hDC);
	}
};