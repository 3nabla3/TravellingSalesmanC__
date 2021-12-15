#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <thread>

#include "PermGen.h"
#include "Timer.h"
#include "MultiThreaded.h"
#include "City.h"


#define MT 1

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

	constexpr unsigned int n_cities = 6;
	City cities[n_cities];
	float red[3] = { 255.f, 0.f, 0.f };
#if !MT
	int timeStep = 100;
	int frameCount = 0;
	PermGen gen(n_cities);
	double bestDistance = -1;		// invalid state that will be replaced by the first distance calculated
	int bestPerm[n_cities];


	Timer timer("Program");
	Timer timer2("Travelling salesman");
	int* perm = gen.GetCurrentPerm();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		frameCount++;

		// if the current perm exists
		if (perm) {
			// Calculate the distance between the cities in the order of the perm
			double distance;
			CalculateTotalDistance(cities, perm, n_cities, &distance);

			// Set the first distance calculated as the best distance
			if (distance < bestDistance || bestDistance == -1) {
				bestDistance = distance;
				std::cout << "Found best distance of " << bestDistance << "" << std::endl;
				// Set the current perm as the best perm
				for (int i = 0; i < n_cities; i++) { bestPerm[i] = perm[i]; }
			}

			// If it is time to render
			/*if (frameCount == timeStep) {
				glClear(GL_COLOR_BUFFER_BIT);
				DrawCitiesAndPath(cities, perm, n_cities);
				DrawPath(cities, bestPerm, n_cities, red);
				frameCount = 0;
			}*/

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

#else
	const unsigned int THREADS = std::thread::hardware_concurrency();
	int** bestPerms = new int*[THREADS];
	auto* threads = new std::thread[THREADS];

	for (int t = 0; t < THREADS; t++){
		threads[t] = std::thread(findBestPerm, cities, n_cities, bestPerms, THREADS, t);
	}

	// TODO: make the threads join as the program is rendering the best running perm
	for (int i = 0; i < THREADS; i++)
		threads[i].join();

	double bestDistance = -1;
	int bestPermIndex = 0;

	for (int t = 0; t < THREADS; t++) {
		double distance;
		CalculateTotalDistance(cities, bestPerms[t], n_cities, &distance);
		if (distance < bestDistance || bestDistance == -1) {
			bestDistance = distance;
			bestPermIndex = t;
		}
	}

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR);
		DrawCitiesAndPath(cities, bestPerms[bestPermIndex], n_cities, red);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

#endif
}


