#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
typedef long double ld;
const ld TOL = 1e-6;
const ld MAX = 1e9;
const int LEN = 100;

struct Pos {
	ld x, y;
	bool operator == (const Pos& p) const { return (std::abs(x - p.x) < TOL && std::abs(y - p.y) < TOL); }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (ld mag) const { return { x * mag, y * mag }; }
	Pos operator / (ld mag) const { return { x / mag, y / mag }; }
}C[LEN];
Pos operator*(ld mag, const Pos& p) { return p * mag; }
std::vector<Pos> dots;


ld dist(const Pos& u, const Pos& v) {
	return sqrtl((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y));
}
ld dist(const Pos& u) {
	return sqrtl(u.x * u.x + u.y * u.y);
}
ld dist_sq(const Pos& u, const Pos& v) {
	return (u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y);
}
bool is_cross(const Pos& u, const Pos& v, ld R) {
	if (dist(u, v) >= 2 * R) return 0;
	return 1;
}
bool is_inside(const Pos& p, const Pos& c, ld R) {
	if (dist_sq(p, c) < R * R - TOL) return 1;
	return 0;
}
void get_closer(const Pos& p, ld R, int N) {
	if (p == Pos{ 0, 0 }) {
		Pos d = { 0, R };
		dots.push_back(d);
		return;
	}
	ld L = dist(p);
	ld mag = R / L;
	Pos d1 = p + mag * p;
	Pos d2 = p - mag * p;
	Pos d = dist(d1) < dist(d2) ? d1 : d2;
	dots.push_back(d);
}
void get_inx(const Pos& u, const Pos& v, ld R, int N) {
	//if (is_cross(u, v, R)) {
	//	ld mx = (u.x + v.x) / 2, my = (u.y + v.y) / 2;
	//	ld vx = mx - u.x, vy = my - u.y;
	//	ld mag = sqrtl(R * R - dist_sq(u, Pos{ mx, my })) / dist(u, Pos{ mx, my });
	//	Pos d1 = { mx + -(mag * vy), my + (mag * vx) };
	//	Pos d2 = { mx + (mag * vy), my + -(mag * vx) };
	//	//std::cout << d1.x << " " << d1.y << "\n";
	//	//std::cout << d2.x << " " << d2.y << "\n";
	//	dots.push_back(d1);
	//	dots.push_back(d2);
	//}
	if (is_cross(u, v, R)) {
		Pos m = (u + v) / 2;
		Pos V = m - u;
		Pos perp = { -V.y, V.x };
		ld mag = sqrtl(R * R - dist_sq(u, m)) / dist(u, m);
		Pos d1 = m + mag * perp;
		Pos d2 = m - mag * perp;
		//std::cout << d1.x << " " << d1.y << "\n";
		//std::cout << d2.x << " " << d2.y << "\n";
		dots.push_back(d1);
		dots.push_back(d2);
	}
}



int main() {
	//freopen("boj9382.txt", "r", stdin);
	std::cout << std::fixed;
	std::cout.precision(6);
	while (1) {
		dots.clear();
		int N; ld R;
		std::cin >> N >> R;
		if (!N) return 0;
		R *= 2;
		Pos p = { 0, 0 };  //center check
		dots.push_back(p);

		for (int i = 0; i < N; i++) {
			std::cin >> C[i].x >> C[i].y;
			get_closer(C[i], R, N);
		}

		for (int i = 0; i < N; i++) {
			for (int j = i + 1; j < N; j++) {
				get_inx(C[i], C[j], R, N);
			}
		}
		ld ans = MAX;
		//std::cout << dots.size();
		for (int i = 0; i < dots.size(); i++) {
			bool f = 1;
			//std::cout << dots[i].x << " " << dots[i].y << "\n";
			for (int j = 0; j < N; j++) {
				if (is_inside(dots[i], C[j], R - TOL * 100)) {
					f = 0;
					break;
				}
			}
			if (f) ans = std::min(ans, dist(dots[i]));
		}
		std::cout << ans << "\n";
	}
}

/*
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-6;
const int LEN = 100;
const ld MAX = 1e9;

struct Pos {
	ld x, y;
	bool operator == (const Pos& p) const {
		return (std::abs(x - p.x) < TOL && std::abs(y - p.y) < TOL);
	}
}C[LEN];
std::vector<Pos> dots;

ld dist(const Pos& u, const Pos& v) {
	return sqrtl((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y));
}
ld dist_sq(const Pos& u, const Pos& v) {
	return (u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y);
}
ld dist(const Pos& u) {
	return sqrtl(u.x * u.x + u.y * u.y);
}
bool is_cross(const Pos& u, const Pos& v, ld R) {
	if (u == v) return 0;
	if (dist(u, v) >= 2*R) return 0;
	return 1;
}
bool is_inside(const Pos& p, const Pos& c, ld R) {
	if (dist_sq(p, c) < R*R - TOL*100) return 1;
	return 0;
}
void get_closer(const Pos& p, ld R, int N) {
	if (p == Pos{ 0, 0 }) {
		Pos d = { 0, R };
		dots.push_back(d);
		return;
	}
	ld L = dist(p);
	Pos d1 = { p.x + R * (p.x / L), p.y + R * (p.y / L) };
	Pos d2 = { p.x - R * (p.x / L), p.y - R * (p.y / L) };
	Pos d = dist(d1) < dist(d2) ? d1 : d2;
	dots.push_back(d);
}
void get_inx(const Pos& u, const Pos& v, ld R, int N) {
	if (is_cross(u, v, R)) {
		ld mx = (u.x + v.x) / 2, my = (u.y + v.y) / 2;
		ld vx = mx - u.x, vy = my - u.y;
		ld mag = sqrtl(R * R - dist_sq(u, Pos{ mx, my })) / dist(u, Pos{ mx, my });
		Pos d1 = { mx + -(mag * vy), my + (mag * vx) };
		Pos d2 = { mx + (mag * vy), my + -(mag * vx) };
		//std::cout << d1.x << " " << d1.y << "\n";
		//std::cout << d2.x << " " << d2.y << "\n";
		dots.push_back(d1);
		dots.push_back(d2);
	}
}



int main() {
	freopen("boj9382.txt", "r", stdin);
	std::cout << std::fixed;
	std::cout.precision(6);
	while (1) {
		dots.clear();
		int N; ld R;
		std::cin >> N >> R;
		if (!N) return 0;
		R *= 2;
		Pos p = { 0, 0 };  //center check
		dots.push_back(p);
		for (int i = 0; i < N; i++) {
			std::cin >> C[i].x >> C[i].y;
			get_closer(C[i], R, N);
		}


		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				get_inx(C[i], C[j], R, N);
			}
		}
		ld ans = MAX;
		//std::cout << dots.size();
		for (int i = 0; i < dots.size(); i++) {
			bool f = 1;
			//std::cout << dots[i].x << " " << dots[i].y << "\n";
			for (int j = 0; j < N; j++) {
				if (is_inside(dots[i], C[j], R - TOL*100)) {
					f = 0;
					break;
				}
			}
			if (f) ans = std::min(ans, dist(dots[i]));
		}
		std::cout << ans << "\n";
	}
}

*/