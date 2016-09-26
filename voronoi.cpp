const double PRECISION = 0.00001;
const double PI = 3.141592741;
const double PI_DOUBLE = PI * 2;

struct Voronoi
{
	// Точка
	struct Point
	{
		double x, y, z;
		bool exists;
		Point *next;

		Point(double _x, double _y, double _z, Point **top = NULL, bool _exists = true) :
			x(_x), y(_y), z(_z), exists(_exists), next(NULL)
		{
			if (top)
			{
				next = *top;
				*top = this;
			}
		}
	};

	// Плоскость
	struct Plane
	{
		double value[4];
		bool exists;
		Plane *next;

		Plane(double a, double b, double c, double d, Plane **top = NULL, bool _exists = true) :
			exists(_exists), next(NULL)
		{
			double size = length(a, b, c);
			value[0] = a / size;
			value[1] = b / size;
			value[2] = c / size;
			value[3] = d / size;
			if (top)
			{
				next = *top;
				*top = this;
			}
		}

		double distanceToPoint(Point *p)
		{
			return value[0] * p->x + value[1] * p->y + value[2] * p->z + value[3];
		}
	};

	// Описание
	double x1, y1, z1, x2, y2, z2;
	Point *point;
	int model;

	// Конструктор
	Voronoi(double _x1, double _y1, double _z1, double _x2, double _y2, double _z2) :
		x1(_x1), y1(_y1), z1(_z1), x2(_x2), y2(_y2), z2(_z2), point(NULL), model(-1)
	{
	}

	// Добавление точки
	void add(double x, double y, double z, bool used = false)
	{
		new Point(x, y, z, &point, used);
	}

	// Длина вектора
	static double length(double ax, double ay, double az)
	{
		return sqrt(sqr(ax) + sqr(ay) + sqr(az));
	}

	// Определитель
	double determinant(Plane *p0, Plane *p1, Plane *p2, int i0, int i1, int i2)
	{
		return
			p0->value[i0] * p1->value[i1] * p2->value[i2] +
			p0->value[i1] * p1->value[i2] * p2->value[i0] +
			p0->value[i2] * p1->value[i0] * p2->value[i1] -
			p0->value[i0] * p1->value[i2] * p2->value[i1] -
			p0->value[i1] * p1->value[i0] * p2->value[i2] -
			p0->value[i2] * p1->value[i1] * p2->value[i0];
	}

	void del(Point *p)
	{
		for (Point *points = p, *next = points->next; points; points = next, next = points ? points->next : NULL)
			delete points;
	}

	// Создание модели
	void make()
	{
		if (model != -1) glDeleteLists(model, 1);

		model = glGenLists(1);
		glNewList(model, GL_COMPILE);

		for (Point *everyPoint = point; everyPoint; everyPoint = everyPoint->next) if (everyPoint->exists)
		{
			Plane *plane = NULL;

			// Ограничивающий параллелепипед
			new Plane(-1, 0, 0, x1, &plane);
			new Plane(0, -1, 0, y1, &plane);
			new Plane(0, 0, -1, z1, &plane);
			new Plane(1, 0, 0, -x2, &plane);
			new Plane(0, 1, 0, -y2, &plane);
			new Plane(0, 0, 1, -z2, &plane);

			// Находим все плоскости между этой точкой и остальными
			for (Point *otherPoint = point; otherPoint; otherPoint = otherPoint->next) if (everyPoint != otherPoint)
			{
				double
					a = otherPoint->x - everyPoint->x,
					b = otherPoint->y - everyPoint->y,
					c = otherPoint->z - everyPoint->z;

				new Plane(a, b, c, -a * (everyPoint->x + otherPoint->x) / 2 - b * (everyPoint->y + otherPoint->y) / 2 - c * (everyPoint->z + otherPoint->z) / 2, &plane, !otherPoint->exists);
			}

			// Для каждой рисуемой грани
			for (Plane *p1 = plane; p1; p1 = p1->next) if (p1->exists)
			{
				Point *intersection = NULL;
				Point center(NULL, 0, 0, 0);
				double determ;

				int count = 0;
				for (Plane *p2 = plane; p2; p2 = p2->next) if (p1 != p2)
					for (Plane *p3 = p2->next; p3; p3 = p3->next) if (p1 != p3)
						if (fabs(determ = -determinant(p1, p2, p3, 0, 1, 2)) > PRECISION)
						{
							// Точка пересечения 3х плоскостей
							Point p(
								determinant(p1, p2, p3, 3, 1, 2) / determ,
								determinant(p1, p2, p3, 0, 3, 2) / determ,
								determinant(p1, p2, p3, 0, 1, 3) / determ);

							// Если точка внутри параллелепипеда
							if (p.x >= x1 - PRECISION) if (p.y >= y1 - PRECISION) if (p.z >= z1 - PRECISION) if (p.x <= x2 + PRECISION) if (p.y <= y2 + PRECISION) if (p.z <= z2 + PRECISION)
							{
								// Не берем, если точка находится за плоскостями
								for (Plane *planes = plane; planes; planes = planes->next)
									if (planes->distanceToPoint(&p) > PRECISION)
									{
										p.exists = false;
										break;
									}

								// Не берем, если уже есть похожая точка
								for (Point *points = intersection; points; points = points->next)
									if (fabs(p.x - points->x) < PRECISION) if (fabs(p.y - points->y) < PRECISION) if (fabs(p.z - points->z) < PRECISION)
									{
										p.exists = false;
										break;
									}

								if (p.exists)
								{
									new Point(p.x, p.y, p.z, &intersection);
									center.x += p.x;
									center.y += p.y;
									center.z += p.z;
									count++;
								}
							}
						}

				if (count > 2)
				{
					center.x /= count;
					center.y /= count;
					center.z /= count;

					// Находим перпендикулярную плоскость, проходящую через центр и случайную точку
					double
						a1 = intersection->x - center.x,
						b1 = intersection->y - center.y,
						c1 = intersection->z - center.z,
						a2 = p1->value[1] * c1 - p1->value[2] * b1,
						b2 = p1->value[2] * a1 - p1->value[0] * c1,
						c2 = p1->value[0] * b1 - p1->value[1] * a1;

					Plane plane(a2, b2, c2, -center.x * a2 - center.y * b2 - center.z * c2);

					// Располагаем точки по часовой стрелке
					Point **drawPoints = new Point*[count];
					double *drawPointsX = new double[count];
					double *drawPointsY = new double[count];
					for (int i = 0; i < count; i++)
					{
						double radius = 10;

						for (Point *points = intersection; points; points = points->next)
							if (points->exists)
							{
								double radius_new = 0;
								if (intersection != points)
								{
									double
										ax = intersection->x - center.x,
										ay = intersection->y - center.y,
										az = intersection->z - center.z,
										bx = points->x - center.x,
										by = points->y - center.y,
										bz = points->z - center.z,
										argument = (ax * bx + ay * by + az * bz) / length(ax, ay, az) / length(bx, by, bz);

									radius_new = fabs(argument) > 1 ? PI : acos(argument);
									if (plane.distanceToPoint(points) < 0) radius_new = PI_DOUBLE - radius_new;
								}

								if (radius_new < radius)
								{
									radius = radius_new;
									drawPoints[i] = points;
								}
							}

						Point onPlane(0, 0, 0);
						if (fabs(p1->value[2]) < PRECISION)
						{
							onPlane.x = center.x;
							onPlane.y = center.y;
							onPlane.z = center.z + 1;
						}
						else if (fabs(p1->value[0]) < PRECISION || fabs(p1->value[1]) < PRECISION)
						{
							onPlane.x = center.x - 1;
							onPlane.y = center.y;
							onPlane.z = center.z;
						}
						else
						{
							onPlane.x = center.x - p1->value[0];
							onPlane.y = center.y - p1->value[1];
							onPlane.z = -(p1->value[0] * onPlane.x + p1->value[1] * onPlane.y + p1->value[3]) / p1->value[2];
						}


						double radius_new = 0;
						double
							ax = intersection->x - center.x,
							ay = intersection->y - center.y,
							az = intersection->z - center.z,
							bx = onPlane.x - center.x,
							by = onPlane.y - center.y,
							bz = onPlane.z - center.z,
							argument = (ax * bx + ay * by + az * bz) / length(ax, ay, az) / length(bx, by, bz);

						//if (length(ax, ay, az) < PRECISION || length(bx, by, bz) < PRECISION) radius_new = PI;
						//else
						radius_new = fabs(argument) > 1 ? PI : acos(argument);
						if (plane.distanceToPoint(&onPlane) < 0) radius_new = PI_DOUBLE - radius_new;

						drawPoints[i]->exists = false;
						//showMessage(strf(radius_new));

						radius = (radius - radius_new) * 180 / PI;
						double distance = pointDistance(center.x, center.y, center.z, drawPoints[i]->x, drawPoints[i]->y, drawPoints[i]->z);

						drawPointsX[i] = center.x + stepX(distance, radius);
						drawPointsY[i] = center.y + center.z + stepY(distance, radius);
					}

					double shadow = (p1->value[0] * 2 + p1->value[1] * 3 + p1->value[2] + 6) / 12;
					glColor4f(shadow, shadow, shadow, 1);
					//glColor4f(random(), random(), random(), 1);

					// Рисуем полигон
					glNormal3f(p1->value[0], p1->value[1], p1->value[2]);
					glBegin(GL_POLYGON);
					for (int i = 0; i < count; i++)
					{
						glTexCoord2f(drawPointsX[i], drawPointsY[i]);
						glVertex3f(drawPoints[i]->x, drawPoints[i]->y, drawPoints[i]->z);
					}
					glEnd();

					delete[] drawPoints;
					delete[] drawPointsX;
					delete[] drawPointsY;
				}

				if (intersection)
					del(intersection);
			}

			for (Plane *planes = plane, *next = planes->next; planes; planes = next, next = planes ? planes->next : NULL)
				delete planes;
		}

		del(point);
		point = NULL;

		glEndList();
	}

	// Рисование модели
	void draw()
	{
		notex.set();
		glColor4f(1, 1, 1, 1);
		//glPolygonMode(GL_FRONT, GL_LINE);
		glCallList(model);
		//glPolygonMode(GL_FRONT, GL_FILL);
	}
};