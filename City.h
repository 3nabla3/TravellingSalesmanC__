//
// Created by alban on 11/18/21.
//
#pragma once
#include <GLFW/glfw3.h>
#include <random>

extern const unsigned int BORDER_WIDTH;
extern const int WIDTH, HEIGHT;

struct City
{
	float x = 0;
	float y = 0;

	City()
	{
		std::random_device device;
		std::mt19937 mt(device());
		std::uniform_real_distribution<float> xDist((float)BORDER_WIDTH, (float)(WIDTH - BORDER_WIDTH));
		std::uniform_real_distribution<float> yDist((float)BORDER_WIDTH, (float)(HEIGHT - BORDER_WIDTH));
		x = xDist(mt);
		y = yDist(mt);
	}

	[[maybe_unused]] City(float x, float y) { this->x = x; this->y = y;}

	void Draw() const {
		int i;
		int triangleAmount = 20; //# of triangles used to draw circle

		float radius = 0.04; //radius
		float twicePi = 2.0f * M_PI;

		float actualX, actualY;
		actualX = x / ((float)WIDTH / 2.0f) - 1;
		actualY = y / ((float)HEIGHT / 2.0f) - 1;

		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(actualX, actualY); // center of circle
		for (i = 0; i <= triangleAmount; i++)
			glVertex2f(
					actualX + (radius * (float)std::cos((float)i * twicePi / (float)triangleAmount)),
					actualY + (radius * (float)std::sin((float)i * twicePi / (float)triangleAmount))
			);
		glEnd();
	}
};

std::ostream& operator<<(std::ostream& out, const City& c)
{
	return out << "X: " << c.x << "\tY: " << c.y;
}

double CalculateDistance(City a, City b) {
	double diffXSquared = (a.x - b.x) * (a.x - b.x);
	double diffYSquared = (a.y - b.y) * (a.y - b.y);
	return diffXSquared + diffYSquared;
}

double  CalculateTotalDistance(const City* cities, const int* perm, const unsigned int permSize) {
	double sum = 0;
	for (int i = 0; i < permSize - 1; i++) {
		int n1 = perm[i];
		int n2 = perm[i + 1];
		City c1 = cities[n1];
		City c2 = cities[n2];
		double distance = CalculateDistance(c1, c2);
		sum += distance;
	}
	return sum;
}

