//
// Created by alban on 11/18/21.
//

#include "PermGen.h"
#include <cassert>

PermGen::PermGen(unsigned int n, unsigned int skip, unsigned int offset)
	: PERMSIZE(n), SKIP(skip), OFFSET(offset){
	current_perm = new int[n];
	Reset();
}

PermGen::~PermGen() {
	delete[] current_perm;
}

// Swaps the elements at index a and b in current_perm
void PermGen::Swap(unsigned int a, unsigned int b) {
	int temp = current_perm[a];
	current_perm[a] = current_perm[b];
	current_perm[b] = temp;
}

void PermGen::PrintCurrentPerm() {
	if (current_perm) {
		for (int i = 0; i < PERMSIZE; i++)
			std::cout << current_perm[i] << ", ";
		std::cout << std::endl;
	}
}

void PermGen::Reset() {
	if (!current_perm) {
		std::cout << "ERROR!!" << std::endl;
		return;
	}

	// Fills the array with numbers 1 to n
	for (int i = 0; i < PERMSIZE; i++)
		current_perm[i] = i;

	// Skip first permutation(s) if needed
	for (int i = 0; i < OFFSET; i++) GenNextPerm();

	finished = false;
}

int *PermGen::GenPerm() {
	// Skip the next permutation(s) if needed
	for (int i = 0; i < SKIP; i++) delete[] GenNextPerm();

	return GenNextPerm();
}

int *PermGen::GenNextPerm() {
	// make sure the PermGen is initialized correctly
	if (finished || PERMSIZE < 0 || current_perm == nullptr) return nullptr;

	// 1. Find the largest i such that values[i] < values[i+1]
	// If there is no such i, values is the last permutation
	int largest_i = -1;
	for (int i = 0; i < PERMSIZE - 1; i++)
		if (current_perm[i] < current_perm[i + 1])
			largest_i = i;

	if (largest_i == -1) {
		// currently on the last permutation
		finished = true;

		int* returnArray = new int[PERMSIZE];
		std::memcpy(returnArray, current_perm, PERMSIZE * sizeof(int));
		return returnArray;
	}

	// 2. Find the largest j such that values[i] < values[j]
	int largest_j = -1;
	for (int j = 0; j < PERMSIZE; j++)
		if (current_perm[largest_i] < current_perm[j])
			largest_j = j;

	// 3. Swap values[largest_i] and values[largest_j]
	Swap(largest_i, largest_j);

	// 4. reverse values[largest_i + 1 .. n]
	unsigned int a = largest_i + 1;
	unsigned int b = PERMSIZE - 1;
	while (a < b)
		Swap(a++, b--);

	int* returnArray = new int[PERMSIZE];
	std::memcpy(returnArray, current_perm, PERMSIZE * sizeof(int));
	return returnArray;
}

int* PermGen::GetCurrentPerm() {
	int* returnArray = new int[PERMSIZE];
	memcpy(returnArray, current_perm, PERMSIZE * sizeof(unsigned int));
	return returnArray;
}

