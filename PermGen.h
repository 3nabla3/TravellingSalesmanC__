#pragma once

#include <cstring>
#include "Timer.h"

// generates permutations of a certain size
class PermGen {
private:
	int* current_perm = nullptr;
	bool finished = false;

	const unsigned int SKIP;		// how many permutations to skip every time GenNextPerm is called
	const unsigned int OFFSET;		// what permutation to start at on the first call of GenNextPerm
	const unsigned int PERMSIZE;	// Number of elements in the permutation

	void Swap(unsigned int a, unsigned int b);
	int* GenNextPerm();				// Generates the next permutation

public:
	void PrintCurrentPerm();
	explicit PermGen(unsigned int n, unsigned int step=0, unsigned int offset=0);
	~PermGen();
	void Reset();					// GenNextPerm() will return the first permutation again
	int* GenPerm(); 				// Generates and returns the next permutation to give
	int* GetCurrentPerm();
};
