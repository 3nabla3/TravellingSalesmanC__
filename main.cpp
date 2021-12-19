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
	for (int i = 0; i < n_cities; i++) {
		cities[i].Draw();
	}

	DrawPath(cities, perm, n_cities, rgb);
}

bool allThreadsFinished(const bool* threadFinishedArray, unsigned int threadCount) {
	bool allFinished = true;
	for (int i = 0; i < threadCount; i++)
		if (!threadFinishedArray[i]) {
			allFinished = false;
			break;
		}

	return allFinished;
}

int main() {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Shortest route between points", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glClearColor(0.0,0.0,0.0,0.0); //dark blue

	constexpr unsigned int n_cities = 11;
	City cities[n_cities];
	float red[3] = { 255.f, 0.f, 0.f };

#if !MT
	int timeStep = 100;  // only draw every n frames (helps with performance)
	int frameCount = 0;
	PermGen gen(n_cities);  // used to generate the permutations
	double bestDistance = -1;		// invalid state that will be replaced by the first distance calculated
	int* bestPerm;  // holds the current best perm

	Timer t("Single threaded");

	// initialize the first permutation
	int* perm = gen.GetCurrentPerm();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		frameCount++;

		// if the current perm exists
		if (perm) {
			// Calculate the distance between the cities in the order of the perm
			double distance = CalculateTotalDistance(cities, perm, n_cities);

			// Set the first distance calculated as the best distance
			if (distance < bestDistance || bestDistance == -1) {
				bestDistance = distance;
				// Set the current perm as the best perm
				bestPerm = perm;
			}
			// delete the current perm if it is not a running best one
			else delete[] perm;

			// If it is time to render
			if (frameCount == timeStep) {
				glClear(GL_COLOR_BUFFER_BIT);
				DrawCitiesAndPath(cities, bestPerm, n_cities, red);
				frameCount = 0;
			}
		}
		// if there are no more perms
		else {
			glClear(GL_COLOR_BUFFER_BIT);
			DrawCitiesAndPath(cities, bestPerm, n_cities, red);
			t.Stop();
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
	// Get the number of cores in the computer
	const unsigned int THREADCOUNT = std::thread::hardware_concurrency();

	// threads use this to store their best perm
	int** bestPermAddresses = new int*[THREADCOUNT]{nullptr};
	// threads use this to show when they are finished
	auto* threadFinishedArray = new bool[THREADCOUNT]{false};
	// used to store the thread objects
	auto* threads = new std::thread[THREADCOUNT];

	// launch the threads
	for (unsigned int t = 0; t < THREADCOUNT; t++){
		threads[t] = std::thread(findBestPerm, cities, n_cities, bestPermAddresses, threadFinishedArray, THREADCOUNT, t);
	}

	Timer t("Multi threaded");

	// display window until the user presses x
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		// check if every thread has found a permutation yet
		bool canRender = false;
		bool allThreadsReadable = true;
		for (int i = 0; i < THREADCOUNT; i++) {
			// if the bestPermAddress is null, do not render
			if (bestPermAddresses[i] == nullptr) {
				allThreadsReadable = false;
				break;
			}
		}
		if (allThreadsReadable) canRender = true;

		if (canRender) {
			// find the best permutation out of the best one of every thread
			double bestDistance = -1;
			unsigned int bestPermIndex;
			for (unsigned int i = 0; i < THREADCOUNT; i++) {
				double distance = CalculateTotalDistance(cities, bestPermAddresses[i], n_cities);

				if (distance < bestDistance | bestDistance == -1) {
					bestDistance = distance;
					bestPermIndex = i;
				}
			}
			// draw the best permutation overall
			DrawCitiesAndPath(cities, bestPermAddresses[bestPermIndex], n_cities, red);

		}

		// stop the timer when the work is finished
		if (allThreadsFinished(threadFinishedArray, THREADCOUNT)) { t.Stop(); }

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

#endif
}


