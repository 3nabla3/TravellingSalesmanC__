//
// Created by alban on 11/16/21.
//
#include "PermGen.h"
#include "City.h"

std::mutex mtx;

void findBestPerm(City* cities, unsigned int nCities, std::atomic<int**> returnPermAddresses,
				  std::atomic<bool*> threadFinishedArray, unsigned int step=0, unsigned int offset=0) {
	// TODO: refactor this to make it use more stack arrays than heap allocated objects

	PermGen gen(nCities, step, offset);
	double bestDistance = -1; // invalid state that will be replaced by the first calculated distance

    int* currentPerm = gen.GetCurrentPerm();
	while (currentPerm) {
		double distance;
		CalculateTotalDistance(cities, currentPerm, nCities, &distance);

		// if the distance is a new best distance
		if (distance < bestDistance || bestDistance == -1) {
			bestDistance = distance;
//			std::cout << "Found a best distance of " << bestDistance << "!" << std::endl;

			// delete the old best perm before setting the new one
			mtx.lock();
            delete returnPermAddresses[offset];

			// set the return perm to the current best perm
			returnPermAddresses[offset] = currentPerm;
			mtx.unlock();
		}
		// delete the current perm if it was not the running shortest
		else delete[] currentPerm;
        currentPerm = gen.GenPerm();
	}

	threadFinishedArray[offset] = true;

//	std::cout << "The best distance was " << bestDistance << "!" << std::endl;
}
