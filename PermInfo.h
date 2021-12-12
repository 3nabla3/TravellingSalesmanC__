#pragma once

struct PermInfo
{
	double distance;
	int permSize;
	int* perm;
};

//std::ostream& operator<<(std::ostream& out, const PermInfo& pi) {
//	std::string perm;
//	for (int i = 0; i < pi.permSize; i++) { perm += pi.perm[i] + ","; }
//
//	return out << "PermInfo: \n" << "Distance: " << pi.distance << "\n" << perm;
//}