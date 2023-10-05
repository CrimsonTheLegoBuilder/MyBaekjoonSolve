#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <set>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-12;
ld A = 0.0;
char grid[50][51];

struct Pos {
	ld x, y;
	bool operator < (const Pos& P) const {
		if (std::abs(x - P.x) < TOL) return y < P.y;
		return x < P.x;
	}
	bool operator == (const Pos& P) const {
		return (std::abs(x - P.x) < TOL && std::abs(y - P.y) < TOL);
	}
}L;
std::set<Pos> X;
std::vector<Pos> hull;

ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x);
}
void get_hull(int y, int x, Pos& L, Pos& a) {
	return;
}



int main() {
	int N, M;
	std::cin >> N >> M;
	for (int i = 0; i < N; i++) {
		std::cin >> grid[i];
		for (int j = 0; j < M; j++) {
			if (grid[i][j] == '*') {
				L.x = j; L.y = i;
			}
			if (grid[i][j] == '#') A += 1.0;
		}
	}


}