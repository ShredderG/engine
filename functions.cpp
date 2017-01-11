// PIs
constexpr float PI        = (float)3.141592741;
constexpr float PI_180    = (float)180.0 / PI;
constexpr float PI_HALF   = PI / 2.0;
constexpr float PI_DOUBLE = PI * 2.0;

// byte [0, 255] to float [0.0, 1.0]
constexpr float char_to_float(uchar value) {
	return value / (float)255.0;
}

// float [0.0, 1.0] to byte [0, 255]
constexpr uchar float_to_char(float value) {
	return (uchar)(value * 255.0);
}

// Show message
void showMessage(string text) {
	MessageBox(nullptr, text.c_str(), "Message", MB_OK);
}

// string to int
int to_int(string value) {
	return stoi("0" + value);
}

// string to float
float to_float(string value) {
	return stof(value);
}

// [0 - 0xFFFFFFFF]
uint _random() {
	return (rand() & 0xFF) | ((rand() & 0xFF) << 8) | ((rand() & 0xFF) << 16) | ((rand() & 0xFF) << 24);
}

// [0 - value]
uint random(uint value) {
	return _random() % (value + 1);
}

// [from - to]
int random(int from, int to) {
	return from + _random() % (abs(to - from) + 1);
}

// [0.0 - 1.0]
float random() {
	return (float)rand() / RAND_MAX;
}

// [0.0 - value]
float random(float value) {
	return random((uint)value) + random() * (value - (int)value);
}

// [from - to]
float random(float from, float to) {
	return from + random(fabs(to - from));
}

// Length direction x
float lenDirX(float length, float xAngle, float yAngle = 90.0) {
	return length * cos(xAngle / PI_180) * sin(yAngle / PI_180);
}

// Length direction y
float lenDirY(float length, float xAngle, float yAngle = 90.0) {
	return length * sin(xAngle / PI_180) * sin(yAngle / PI_180);
}

// Length direction z
float lenDirZ(float length, float xAngle, float yAngle = 90.0) {
	return length * sin(yAngle / PI_180 - PI_HALF);
}

// Squared number
constexpr float sqr(float x) {
	return x * x;
}

// Distance between (x1,y1) and (x2,y2)
float pointDistance(float x1, float y1, float x2, float y2) {
	return sqrt(sqr(x2 - x1) + sqr(y2 - y1));
}

// Distance between (x1,y1,z1) and (x2,y2,z2)
float pointDistance(float x1, float y1, float z1, float x2, float y2, float z2) {
	return sqrt(sqr(x2 - x1) + sqr(y2 - y1) + sqr(z2 - z1));
}

// Horizontal direction from (x1,y1) to (x2,y2)
float pointDirection(float x1, float y1, float x2, float y2) {
	float xx = x2 - x1;
	if (xx != 0.0) {
		float direction = atan((y2 - y1) / xx) * PI_180;

		if (x2 > x1) {
			if (y1 > y2)
				return direction + (float)360.0;
			else
				return direction;
		}
		return direction + (float)180.0;
	}
	if (y1 > y2) return 270.0;
	if (y1 < y2) return 90.0;
	return 0.0;
}

// Vertical direction from (x1,y1,z1) to (x2,y2,z2)
float pointDirection(float x1, float y1, float z1, float x2, float y2, float z2) {
	float distance = pointDistance(x1, y1, x2, y2);

	if (distance != 0.0) return atan((z2 - z1) / distance) * PI_180 + (float)90.0;
	if (z1 < z2) return 180.0;
	return 0.0;
}