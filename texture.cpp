// Texture
class Texture {
private:
	// Alpha pink color
	static constexpr uchar ALPHA_RED   = 255;
	static constexpr uchar ALPHA_GREEN = 0;
	static constexpr uchar ALPHA_BLUE  = 255;

	uchar* data_;
	uint id_;

public:
	uint width;
	uint height;
	uint imageWidth;
	uint imageHeight;
	uint imagesCount;
	float* x1;
	float* x2;
	float* y1;
	float* y2;

	Texture() = default;

	Texture(uint _width, uint _height, uint _imageWidth, uint _imageHeight, uchar* _data_) :
		width(_width),
		height(_height),
		imageWidth(_imageWidth),
		imageHeight(_imageHeight),
		data_(_data_) {
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
		uchar* data = data_;
		uchar* alpha = new uchar[length];
		for (uint i = 0; i < length; i += 4) {
			alpha[i]     = *data++;
			alpha[i + 1] = *data++;
			alpha[i + 2] = *data++;
			alpha[i + 3] = (alpha[i] == ALPHA_RED && alpha[i + 1] == ALPHA_GREEN && alpha[i + 2] == ALPHA_BLUE) ? 0 : 255;
		}

		// Texture generating
		glGenTextures(1, &id_);
		glBindTexture(GL_TEXTURE_2D, id_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, alpha);

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

	void operator = (Texture &tex) {
		if (id_ != tex.id_) {
			x1 = tex.x1;
			x2 = tex.x2;
			y1 = tex.y1;
			y2 = tex.y2;

			width       = tex.width;
			height      = tex.height;
			imageWidth  = tex.imageWidth;
			imageHeight = tex.imageHeight;
			imagesCount = tex.imagesCount;
			glBindTexture(GL_TEXTURE_2D, id_ = tex.id_);
		}
	}

	void operator = (Target &target) {
		if (id_ != target.colorBuffer) {
			width       = target.widthFull;
			height      = target.heightFull;
			imageWidth  = target.width;
			imageHeight = target.height;
			imagesCount = 1;
			glBindTexture(GL_TEXTURE_2D, id_ = target.colorBuffer);
		}
	}
} texture;