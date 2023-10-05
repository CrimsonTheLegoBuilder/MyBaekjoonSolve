#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <set>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-9;

struct Pos {
	ld x, y;
	bool operator < (const Pos& P) const {
		if (std::abs(x - P.x) < TOL) return y < P.y;
		return x < P.x;
	}
	bool operator == (const Pos& P) const {
		return (std::abs(x - P.x) < TOL && std::abs(y - P.y) < TOL);
	}
};
std::set<Pos> X;

int main() {
	//float a = atan2l(-1.0, -1.0);
	//std::cout << a;
	Pos S = { 0.1, 0.1 };
	X.insert(S);
	X.insert(S);
	std::cout << X.size();
	return 0;
}