#include <cmath>
#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <random>

#include "PermGen.h"
#include "Timer.h"
#include "MultiThreaded.h"
#include "City.h"

#define MT 0

const int WIDTH = 640;
const int HEIGHT = 640;

const unsigned int BORDER_WIDTH = WIDTH * 0.1f; // makes sure cities are not too close to the edge

static float DefaultColor[3] = {100.f, 100.f, 100.f};
void DrawPath(City cities[], const int* perm, unsigned int permSize, float rgb[3]=DefaultColor) {
	glColor3f(rgb[0], rgb[1], rgb[2]);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < permSize; i++) {
		City c = cities[perm[i]];
		float actualX = c.x / (WIDTH / 2.0f) - 1;
		float actualY = c.y / (HEIGHT / 2.0f) - 1;
		glVertex2f(actualX, actualY);
	}
	glEnd();
}

void DrawCitiesAndPath(City* cities, const int* perm, int n_cities, float rgb[3]=DefaultColor){
	for (int i = 0; i < n_cities; ++i) {
		cities[i].Draw();
	}

	DrawPath(cities, perm, n_cities, rgb);
}

int main() {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glClearColor(0.0,0.0,0.0,0.0); //dark blue
#if !MT
	int timeStep = 1000;
	int frameCount = 0;
	constexpr unsigned int n_cities = 9;
	City cities[n_cities];
	PermGen gen(n_cities);
	double bestDistance = -1;		// invalid state that will be replaced by the first distance calculated
	int bestPerm[n_cities];
	float red[3] = {255.f, 0.f, 0.f};
#else
	constexpr unsigned int threads = 4;
	City cities[nCities];
//	int** bestPerms = new int*[threads];
//	for (int t = 0; t < threads; t++){
//		bestPerms[t] = new int[nCities];
//	}
	int* bestPerm = nullptr;						// array that will contain the best perm of a single thread
	findBestPerm(cities, nCities, &bestPerm, 0, 0);

	std::cout << "Outside of function" << std::endl;
	std::cout << "Best perm: " << std::endl;
	for (int i = 0; i < nCities; i++)
		std::cout << bestPerm[i] << ", ";
	std::cout << std::endl;

	delete[] bestPerm;

//	for (int t = 0; t < threads; t++){
//		delete[] bestPerms[t];
//	}
//	delete[] bestPerms;

#endif

	Timer timer("Program");
	Timer timer2("Travelling salesman");
	int* perm = gen.GetPerm();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		frameCount++;

		// if the current perm exists
		if (perm) {
			// Calculate the distance between the cities in the order of the perm
			double distance = 0.0;
			CalculateTotalDistance(cities, perm, n_cities, &distance);

			// Set the first distance calculated as the best distance
			if (distance < bestDistance || bestDistance == -1) {
				bestDistance = distance;
				std::cout << "Found best distance of " << bestDistance << "" << std::endl;
				// Set the current perm as the best perm
				for (int i = 0; i < n_cities; i++) { bestPerm[i] = perm[i]; }
			}

			// If it is time to render
			if (frameCount == timeStep) {
				glClear(GL_COLOR_BUFFER_BIT);
				DrawCitiesAndPath(cities, perm, n_cities);
				DrawPath(cities, bestPerm, n_cities, red);
				frameCount = 0;
			}

			// Delete the current perm (bestPerm is just a copy)
			// TODO: optimize bestPerm by not making a copy but
			//  using the current perm of that iteration
			delete[] perm;

		} else {
			glClear(GL_COLOR_BUFFER_BIT);
			DrawCitiesAndPath(cities, bestPerm, n_cities, red);
			timer2.Stop();
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		// Generate the perm for the next iteration
		perm = gen.GenPerm();
	}

	glfwTerminate();

	return 0;
}


