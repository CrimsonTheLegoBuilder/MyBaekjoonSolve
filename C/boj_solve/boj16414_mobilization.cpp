#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld TOL = 1e-10;

bool z(ld x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	bool operator < (const Pos& p) const { return z(p.x - x) ? y < p.y : x < p.x; }
};
std::vector<Pos> C, H;

ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
void upper_hull(std::vector<Pos>& C, std::vector<Pos>& H) {
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& d : C) H.push_back(d);
		std::reverse(H.begin(), H.end());
		return;
	}
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 ||
			z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) H.pop_back();
		H.push_back(C[i]);
	}
	return;
}



int main() {
	int N;
	ld B, c, h, p, x, y, vy, vx, MAX = 0;
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	//freopen("mobilizationin/Mobilization-1003.in", "r", stdin);
	std::cin >> N >> B;
	for (int i = 0; i < N; i++) {
		std::cin >> c >> h >> p;
		x = (B / c) * h, y = (B / c) * p;
		MAX = std::max(MAX, x * y);
		C.push_back({ x, y });
	}
	upper_hull(C, H);
	int l = H.size() - 1;
	for (int i = 0; i < l; i++) {
		vy = H[i + 1].y - H[i].y;
		vx = H[i].x - H[i + 1].x;
		c = vy * H[i].x + vx * H[i].y;  //vy * x + vx * y - c = 0
		if (!z(vy)) {
			x = c / (vy * 2);  //x-intercept / 2, MAXIMUM rectangle area in triangle x-int - y-int - O
			if (H[i + 1].x < x && x < H[i].x) {
				y = (c - vy * x) / vx;  //vy * x + vx * y - c = 0
				MAX = std::max(MAX, x * y);
			}
		}
	}
	std::cout << MAX << "\n";
	return 0;
}


//while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) H.pop_back();
//C.push_back({ 0, 0 });