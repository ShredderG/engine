// Target
class Target {
public:
	uint frameBuffer = 0;
	uint depthBuffer;
	uint colorBuffer;

	uint width;
	uint height;
	uint widthFull;
	uint heightFull;

	static constexpr float x1 = 0;
	float y1;
	float x2;
	static constexpr float y2 = 0;

	void create(uint width, uint height) {
		this->width  = width;
		this->height = height;

		widthFull  = to2pow(width);
		heightFull = to2pow(height);

		y1 = (float)height / heightFull;
		x2 = (float)width  / widthFull;

		// Texture to store RGBA data
		glGenTextures(1, &colorBuffer);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthFull, heightFull, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		// Texture to store depth data
		glGenTextures(1, &depthBuffer);
		glBindTexture(GL_TEXTURE_2D, depthBuffer);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, widthFull, heightFull, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		// Framebuffer attaching
		gl_GenFramebuffers(1, &frameBuffer);
		gl_BindFramebuffer(GL_FRAMEBUFFER_EXT, frameBuffer);
		gl_FramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, colorBuffer, 0);
		gl_FramebufferTexture2D(GL_FRAMEBUFFER,     GL_DEPTH_ATTACHMENT,      GL_TEXTURE_2D, depthBuffer, 0);

		// Check for errors
		GLenum status = gl_CheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			showMessage("FBO error");
		}

		// Back to default buffer
		gl_BindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	}

	~Target() {
		if (frameBuffer) {
			gl_DeleteFramebuffers(1, &frameBuffer);
			glDeleteTextures(1, &colorBuffer);
			glDeleteTextures(1, &depthBuffer);
		}
	}

	void clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Bind frame buffer uint
	void operator = (uint frameBuffer) {
		window.update(window.width, window.height);
		gl_BindFramebuffer(GL_FRAMEBUFFER_EXT, this->frameBuffer = frameBuffer);
	}

	// Bind frame buffer Target
	void operator = (Target &target) {
		window.update(target.width, target.height);
		gl_BindFramebuffer(GL_FRAMEBUFFER_EXT, frameBuffer = target.frameBuffer);
		y1         = target.y1;
		x2         = target.x2;
		width      = target.width;
		height     = target.height;
		widthFull  = target.widthFull;
		heightFull = target.heightFull;
	}

private:
	uint to2pow(uint x) {
		uint a = 1;
		while ((a <<= 1) < x);
		return a;
	}

} target;