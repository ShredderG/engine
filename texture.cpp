struct Texture
{
private:
	uchar *data_;
	uint id_;

public:
	ushort count, width, height, xSize, ySize;
	float *x1, *x2, *y1, *y2;

	Texture()
	{
	}

	Texture(ushort w, ushort h, ushort x, ushort y, uchar *bytes) :
		width(w), height(h), xSize(x), ySize(y),
		data_(bytes)
	{
	}

	void load()
	{
		// texture coordinates
		int xCount = width / xSize;
		int yCount = height / ySize;
		count = xCount * yCount;

		x1 = new float[count];
		y1 = new float[count];
		x2 = new float[count];
		y2 = new float[count];

		int index = 0;
		for (int j = 0; j<yCount; j++)
		{
			for (int i = 0; i<xCount; i++)
			{
				x1[index] = (float)i * xSize / width;
				x2[index] = (float)(i + 1) * xSize / width;
				y1[index] = 1 - (float)j * ySize / height;
				y2[index] = 1 - (float)(j + 1) * ySize / height;
				index++;
			}
		}

		// texture creating
		glGenTextures(1, &id_);

		uint length = width * height * 4;
		uchar *alpha = new uchar[length];

		// colors - rgba
		for (uint i = 0; i<length; i += 4)
		{
			alpha[i] = *data_++;
			alpha[i + 1] = *data_++;
			alpha[i + 2] = *data_++;
			alpha[i + 3] = alpha[i] == 255 && alpha[i + 1] == 0 && alpha[i + 2] == 255 ? 0 : 255;
		}

		glBindTexture(GL_TEXTURE_2D, id_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, alpha);

		delete[] alpha;
	}

	void unload()
	{
		glDeleteTextures(1, &id_);
		delete[] x1,y2;
		delete[] y1;
		delete[] x2;
		delete[] y2;
	}

	bool operator == (Texture &tex)
	{
		return id_ == tex.id_;
	}

	bool operator != (Texture &tex)
	{
		return id_ != tex.id_;
	}

	void operator = (Texture &tex)
	{
		if (id_ != tex.id_)
		{
			id_ = tex.id_;
			x1 = tex.x1;
			x2 = tex.x2;
			y1 = tex.y1;
			y2 = tex.y2;
			width = tex.width;
			height = tex.height;
			glBindTexture(GL_TEXTURE_2D, id_);
		}
	}
} texture;