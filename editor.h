#define GM_EDITOR

#ifdef GM_EDITOR

struct GM_editor
{
	float x, y, z;
	float dx, dy;
	float mx, my;
	float zPlace;
	float size;
	float grid;
	GM_texture *texture;
	bool loaded;

	GM_editor();
	void step();
	void draw();

	void rect(float x, float y, float w, float h)
	{
		glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex2f(x, y);
		glTexCoord2f(0, 0);
		glVertex2f(x, y + h);
		glTexCoord2f(1, 0);
		glVertex2f(x + w, y + h);
		glTexCoord2f(1, 1);
		glVertex2f(x + w, y);
		glEnd();
	}
} editor;


#endif