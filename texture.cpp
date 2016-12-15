struct Texture
{
private:
	uchar *data_;
	uint id_;

public:
	ushort width, height, imageWidth, imageHeight, imagesCount;
	float *x1, *x2, *y1, *y2;

	Texture() = default;

	Texture(ushort widthNew, ushort heightNew, ushort imageWidthNew, ushort imageHeightNew, uchar *bytes) :
		width(widthNew),
		height(heightNew),
		imageWidth(imageWidthNew),
		imageHeight(imageHeightNew),
		data_(bytes)
	{
	}

	void load()
	{
		// texture coordinates
		int length = width * height * 4,
			xCount = width  / imageWidth,
			yCount = height / imageHeight;
		imagesCount = xCount * yCount;

		x1 = new float[imagesCount];
		y1 = new float[imagesCount];
		x2 = new float[imagesCount];
		y2 = new float[imagesCount];

		int index = 0;
		for (int j = 0; j < yCount; j++) {
			for (int i = 0; i < xCount; i++) {
				x1[index] = (float)i * imageWidth / width;
				x2[index] = (float)(i + 1) * imageWidth / width;
				y1[index] = 1.0 - (float)j * imageHeight / height;
				y2[index] = 1.0 - (float)(j + 1) * imageHeight / height;
				index++;
			}
		}

		// texture creating
		uchar *alpha = new uchar[length];
		for (uint i = 0; i < length; i += 4) {
			alpha[i]     = *data_++;
			alpha[i + 1] = *data_++;
			alpha[i + 2] = *data_++;
			alpha[i + 3] = alpha[i] == 255 && alpha[i + 1] == 0 && alpha[i + 2] == 255 ? 0 : 255;
		}

		glGenTextures(1, &id_);
		glBindTexture(GL_TEXTURE_2D, id_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, alpha);

		delete[] alpha;
	}

	void unload()
	{
		glDeleteTextures(1, &id_);
		delete[] x1;
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
		if (id_ != tex.id_) {
			id_ = tex.id_;
			x1 = tex.x1;
			x2 = tex.x2;
			y1 = tex.y1;
			y2 = tex.y2;
			width  = tex.width;
			height = tex.height;
			imagesCount = tex.imagesCount;
			imageWidth  = tex.imageWidth;
			imageHeight = tex.imageHeight;
			glBindTexture(GL_TEXTURE_2D, id_);
		}
	}
} texture;