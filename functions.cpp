// show text
void showMessage(string text)
{
	MessageBox(NULL, text.c_str(), "Message", MB_OK);
}

// int to string
string str(int a)
{
	char buffer[12];
	string result = "";
	if (a < 0) {
		result += "-";
		a *= -1;
	}
	result += _itoa(a, buffer, 10);
	return result;
}

// float to string
string strf(float a)
{
	char buffer[12];
	string result = "";
	if (a < 0.0) {
		result += "-";
		a *= -1;
	}
	int b = a;
	result += _itoa(b, buffer, 10);
	result += ".";
	b = (a - b) * 100;
	result += _itoa(b, buffer, 10);
	return result;
}

// string to int
int real(string a)
{
	return atoi(a.c_str());
}

// string to float
float realf(string a)
{
	return atof(a.c_str());
}

// [0 - 0xFFFFFFFF]
uint GAME_rand()
{
	return rand() | (rand() << 16);
}

// [0 - value]
int irandom(int value)
{
	return GAME_rand() % (value + 1);
}

// [from - to]
int irandom(int from, int to)
{
	return from + GAME_rand() % (to - from + 1);
}

// [0.0 - 1.0]
float random()
{
	return (float)rand() / RAND_MAX;
}

// [0.0 - value]
float random(float value)
{
	return irandom(value) + random() * (value - int(value));
}

// [from - to]
float random(float from, float to)
{
	return from + random(to - from);
}

// PI
constexpr float PI        = 3.141592741;
constexpr float PI_180    = 180.0 / PI;
constexpr float PI_HALF   = PI / 2.0;
constexpr float PI_DOUBLE = PI * 2.0;

// length_dir
float stepX(float length, float xAngle, float yAngle = 90.0) { return length * cos(xAngle / PI_180) * sin(yAngle / PI_180); }
float stepY(float length, float xAngle, float yAngle = 90.0) { return length * sin(xAngle / PI_180) * sin(yAngle / PI_180); }
float stepZ(float length, float xAngle, float yAngle = 90.0) { return length * sin(yAngle / PI_180 - PI_HALF); }

constexpr float sqr(float x)
{
	return x * x;
}

float pointDistance(float x1, float y1, float x2, float y2)
{
	return sqrt(sqr(x2 - x1) + sqr(y2 - y1));
}

float pointDistance(float x1, float y1, float z1, float x2, float y2, float z2)
{
	return sqrt(sqr(x2 - x1) + sqr(y2 - y1) + sqr(z2 - z1));
}

// Направление по горизонтали из (x1,y1) в (x2,y2)
float pointDirection(float x1, float y1, float x2, float y2)
{
	float xx = x2 - x1;
	if (xx != 0.0) {
		float v = atan((y2 - y1) / xx) * PI_180;

		if (x2 > x1) {
			if (y1 > y2)
				return v + 360.0;
			else
				return v;
		}
		return v + 180.0;
	}
	if (y1 > y2) return 270.0;
	if (y1 < y2) return 90.0;
	return 0.0;
}

// Направление по вертикали из (x1,y1,z1) в (x2,y2,z2)
float pointDirection(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float distance = pointDistance(x1, y1, x2, y2);

	if (distance != 0.0) return atan((z2 - z1) / distance) * PI_180 + 90.0;
	if (z1 < z2) return 180.0;
	return 0;
}