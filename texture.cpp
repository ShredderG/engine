// Texture
class Texture {
private:
	// Alpha pink color
	uchar ALPHA_RED   = 255;
	uchar ALPHA_GREEN = 0;
	uchar ALPHA_BLUE  = 255;

	uchar* data_;
	uint id_;

public:
	ushort width;
	ushort height;
	ushort imageWidth;
	ushort imageHeight;
	ushort imagesCount;
	float* x1;
	float* x2;
	float* y1;
	float* y2;

	Texture() = default;

	Texture(ushort widthNew, ushort heightNew, ushort imageWidthNew, ushort imageHeightNew, uchar* bytes) :
		width(widthNew),
		height(heightNew),
		imageWidth(imageWidthNew),
		imageHeight(imageHeightNew),
		data_(bytes) {
		// nothing here
	}

	// Load texture
	void load() {
		// Texture params
		const uint length = width * height * 4;
		const uint xCount = width  / imageWidth;
		const uint yCount = height / imageHeight;
		imagesCount = xCount * yCount;

		// Texture coordinates
		x1 = new float[imagesCount];
		y1 = new float[imagesCount];
		x2 = new float[imagesCount];
		y2 = new float[imagesCount];

		int index = 0;
		for (uint j = 0; j < yCount; j++) {
			for (uint i = 0; i < xCount; i++) {
				x1[index] = (float)i * imageWidth / width;
				x2[index] = (float)(i + 1) * imageWidth / width;
				y1[index] = (float)1.0 - (float)j * imageHeight / height;
				y2[index] = (float)1.0 - (float)(j + 1) * imageHeight / height;
				index++;
			}
		}

		// Texture alpha channel
		uchar* alpha = new uchar[length];
		for (uint i = 0; i < length; i += 4) {
			alpha[i]     = *data_++;
			alpha[i + 1] = *data_++;
			alpha[i + 2] = *data_++;
			alpha[i + 3] = alpha[i] == ALPHA_RED && alpha[i + 1] == ALPHA_GREEN && alpha[i + 2] == ALPHA_BLUE ? 0 : 255;
		}

		// Texture generating
		glGenTextures(1, &id_);
		glBindTexture(GL_TEXTURE_2D, id_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, alpha);

		delete[] alpha;
	}

	// Unload texture
	void unload() {
		glDeleteTextures(1, &id_);
		delete[] x1;
		delete[] y1;
		delete[] x2;
		delete[] y2;
	}

	bool operator == (Texture &tex) {
		return id_ == tex.id_;
	}

	bool operator != (Texture &tex) {
		return id_ != tex.id_;
	}

	void operator = (Texture &tex) {
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