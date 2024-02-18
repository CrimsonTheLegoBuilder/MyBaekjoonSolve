#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
typedef long double ld;
const ld TOL = 1e-9;
int N, x1, y_1, z1, x2, y2, z2;

bool z(ld x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	bool operator < (const Pos& p) const {
		return z(p.x - x) ? y < p.y : x < p.x;
	}
};
std::vector<Pos> C, H;
struct Coord { ld x, y, z; }l;
std::vector<Coord> tree;
struct Vec { ld vx, vy, vz; };
std::vector<Vec> ray;

void mk_tree(ld x1, ld y1, ld z1, ld x2, ld y2, ld z2, std::vector<Coord>& T) {
	T = {
		{ x1, y1, z1 },
		{ x2, y1, z1 },
		{ x1, y2, z1 },
		{ x1, y1, z2 },
		{ x2, y2, z1 },
		{ x1, y2, z2 },
		{ x2, y1, z2 },
		{ x2, y2, z2 },
	};
}
void mk_ray(std::vector<Coord>& T, const Coord& l, std::vector<Vec>& R) {
	for (const Coord& t : T) {
		R.push_back({ t.x - l.x, t.y - l.y, t.z - l.z });
	}
}
void mk_shadow(std::vector<Vec>& R, const Coord& l, std::vector<Pos>& C) {
	for (const Vec& r : R) {
		if (r.vz < 0) {
			Pos ex = { l.x - l.z * (r.vx / r.vz), l.y - l.z * (r.vy / r.vz) };
			C.push_back(ex); 
		}
	}
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
void monotone_chain(std::vector<Pos>& C, std::vector<Pos>& H) {
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& p : C) H.push_back(p);
		return ;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	return;
}
ld A(std::vector<Pos>& H) {
	Pos P = { 0, 0 };
	ld area = 0;
	int h = H.size();
	for (int i = 0; i < h; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % h];
		area += cross(P, cur, nxt);
	}
	return area / 2;
}



int main() {
	std::cout << std::fixed;
	std::cout.precision(8);
	std::cin >> x1 >> y_1 >> z1 >> x2 >> y2 >> z2 >> l.x >> l.y >> l.z;
	if ((x1 == x2 && y_1 == y2) || (x1 == x2 && z1 == z2) || (y_1 == y2 && z1 == z2)) {
		std::cout << "0\n";
		return 0;
	}
	if ((x1 == x2 && z(x1 - l.x)) || (y_1 == y2 && z(y_1 - l.y))) {
		std::cout << "0\n";
		return 0;
	}
	mk_tree(x1, y_1, z1, x2, y2, z2, tree);
	mk_ray(tree, l, ray);
	mk_shadow(ray, l, C);
	if (C.size() <= 4) {
		std::cout << "-1\n";
		return 0;
	}
	monotone_chain(C, H);
	std::cout << A(H);
	return 0;
}