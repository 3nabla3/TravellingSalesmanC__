//
// Created by alban on 11/18/21.
//
#pragma once
#include <GLFW/glfw3.h>

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

double CalculateDistance(City a, City b) {
	double diffXSquared = (a.x - b.x) * (a.x - b.x);
	double diffYSquared = (a.y - b.y) * (a.y - b.y);
	return diffXSquared + diffYSquared;
}

// TODO: does this really need to take a return value as a ptr ?
void CalculateTotalDistance(const City* cities, const int* perm, const unsigned int permSize, double* distance_ptr) {
	*distance_ptr = 0.0;
	for (int i = 0; i < permSize - 1; i++) {
		*distance_ptr += CalculateDistance(cities[perm[i]], cities[perm[i + 1]]);
	}
}

std::ostream& operator<<(std::ostream& out, const City& c)
{
	return out << "X: " << c.x << "\tY: " << c.y;
}