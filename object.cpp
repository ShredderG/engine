// With macros
#define with(object)                                               \
for (OBJECT_##object* object = ::object;                           \
    object > (OBJECT_##object*)0x0000FFFF;                         \
    object = (OBJECT_##object*)object->GAME_right)                 \
    if (((Engine::Object*)object)->GAME_id() != ::object->OBJECT_##object::GAME_id()) \
        break;                                                     \
    else if (object->isActive)

// is object exists? macros
#define objectExists(object) (object > (Engine::Object*)0x0000FFFF)

// Cast & get address for the 1st parameter
#define objectCreate(x, y, z, object) ((OBJECT_##object*)_objectCreate(x, y, z, (Engine::Object**)&object))

// Engine
namespace Engine {
	float x;
	float y;
	float z;

	// Object
	class Object {
	public:
		Object* GAME_left;
		Object* GAME_right;

		float x;
		float y;
		float z;

		char zIndex;
		bool isActive;
		bool isSolid;
		bool isPersistent;

		// Constructor
		Object() :
			isActive(true),
			isSolid(false),
			isPersistent(false),
			zIndex(0) {
			// Set some vars
			object = this;
			x = Engine::x;
			y = Engine::y;
			z = Engine::z;
		}

		// Destructor
		void destroy(Object** ptr) {
			// Change first-same-object pointer
			if (*ptr == this) {
				if (GAME_right != nullptr) if (GAME_right->GAME_id() == GAME_id()) *ptr = GAME_right;
				if (*ptr == this) *ptr = (Object*)GAME_id();
			}

			// Change neighbours links to each other
			(GAME_left != nullptr ? GAME_left->GAME_right : objectList) = GAME_right;
			if (GAME_right != nullptr) GAME_right->GAME_left = GAME_left;

			// Delete
			delete this;
		}

		// Destroy
		inline void destroy() {
			isActive = false;
		}

		// Insert object to the object list
		void GAME_insert(Object* object) {
			GAME_left  = nullptr;
			GAME_right = object;

			// If there is the same object giving a place before him
			if (objectExists(GAME_right)) {
				// Put this new object before him
				GAME_left = GAME_right->GAME_left;
			} else {
				// Find a place in whole list to insert this new object
				GAME_right = objectList;
				while (GAME_right != nullptr) {
					if (zIndex < GAME_right->zIndex) {
						GAME_left  = GAME_right;
						GAME_right = GAME_right->GAME_right;
					} else break;
				}
			}

			// Change neighbours links to this new object
			(GAME_left != nullptr ? GAME_left->GAME_right : objectList) = this;
			if (GAME_right != nullptr) GAME_right->GAME_left = this;
		}

		// Virtual funcs
		virtual void     GAME_kill()     = 0;
		virtual void     GAME_step()     = 0;
		virtual void     GAME_draw()     = 0;
		virtual ObjectId GAME_id() const = 0;
	} *objectList = nullptr, *object = nullptr;

	// Delete everything
	void deleteObjects() {
		Object* ptr = objectList;
		while (ptr != nullptr) {
			Object* del = ptr;
			ptr = ptr->GAME_right;
			del->GAME_kill();
		}
	}

	// Global step
	void step() {
		// Step active
		Object* ptr = objectList;
		while (ptr != nullptr) {
			if (ptr->isActive) ptr->GAME_step();
			ptr = ptr->GAME_right;
		}

		// Delete inactive
		ptr = objectList;
		while (ptr != nullptr) {
			Object* del = ptr;
			ptr = ptr->GAME_right;
			if (!del->isActive) del->GAME_kill();
		}

		// Reset mouse & keyboard
		Keyboard::reset();
		Mouse::reset();
	}

	// Global draw
	void draw() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw
		Object* ptr = objectList;
		while (ptr != nullptr) {
			ptr->GAME_draw();
			ptr = ptr->GAME_right;
		}

		SwapBuffers(Window::hDC);
	}
};