#include <iostream>
#include <algorithm>
#include <cmath>
typedef long double ld;
const int LEN = 100;
const ld TOL = 1e-9;
int N;
ld x, y;
char F;

bool z(ld x) { return std::abs(x) < TOL; }  //zero?
struct Pos {
	ld x, y; bool i;
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator * (const ld& s) const { return { x * s, y * s }; }
} T[LEN];
struct Line {
	ld vy, vx, c; //vy * x + vx * y - c = 0
	ld operator / (const Line& l) const { return vy * l.vx - vx * l.vy; }  //cross
	Line operator ~ () const { return { vx, -vy, c }; }  //rotate pi/2
};  
struct Circle { Pos c; ld r; };

ld dist(const Pos& d1, const Pos& d2) {
	ld x = d1.x - d2.x, y = d1.y - d2.y;
	return x * x + y * y;
}
Pos m(const Pos& d1, const Pos& d2) { return (d1 + d2) * .5; }
Line L(const Pos& d1, const Pos& d2) { return { d2.y - d1.y, d1.x - d2.x, (d2.y - d1.y) * d1.x - (d1.x - d2.x) * d1.y }; }
Line L(const Line& v, const Pos& p) { return { v.vy, v.vx, v.vy * p.x + v.vx * p.y }; }
Pos IP(const Line& l1, const Line& l2) {
	ld det = l1 / l2;  	//ld det = l1.vy * l2.vx - l1.vx * l2.vy;
	return { (l1.c * l2.vx - l1.vx * l2.c) / det, (l1.vy * l2.c - l1.c * l2.vy) / det };
}
Circle C(const Pos& d1, const Pos& d2) {
	Pos c = m(d1, d2);
	ld r = dist(c, d1);
	return { c, r };
}
Circle C(const Pos& d1, const Pos& d2, const Pos& d3) {
	Line l1 = L(~L(d1, d2), m(d1, d2)), l2 = L(~L(d2, d3), m(d2, d3));
	if (z(l1 / l2)) return { (0, 0), -1 };
	Pos c = IP(l1, l2);
	ld r = dist(c, d1);
	return { c, r };
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	if (N <= 2) {
		std::cout << "No\n";
		return 0;
	}
	for (int i = 0; i < N; i++) {
		std::cin >> T[i].x >> T[i].y >> F;
		T[i].i = F == 'I' ? 1 : 0;
	}
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			Pos d1 = T[i], d2 = T[j];
			Circle c = C(d1, d2);
			bool f = 1;
			for (int k = 0; k < N; k++) {
				ld d = dist(c.c, T[k]);
				if ((T[k].i && d > c.r) || (!T[k].i && d < c.r)) {
					f = 0; break;
				}
			}
			if (f) {
				std::cout << "No\n";
				return 0;
			}
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			for (int k = j + 1; k < N; k++) {
				Pos d1 = T[i], d2 = T[j], d3 = T[k];
				Circle c = C(d1, d2, d3);
				if (c.r < 0) continue;
				bool f = 1;
				for (int l = 0; l < N; l++) {
					ld d = dist(c.c, T[l]);
					if ((T[l].i && d > c.r) || (!T[l].i && d < c.r)) {
						f = 0; break;
					}
				}
				if (f) {
					std::cout << "No\n";
					return 0;
				}
			}
		}
	}
	std::cout << "Yes\n";
	return 0;
}


//std::cin >> x >> y >> F;
//if (F == 'I') T[i] = { x, y, 1 };
//else if (F == 'N') T[i] = { x, y, 0 };
//struct Vec {
//	ld vy, vx;
//	Vec operator ~ () const { return { vx, -vy }; }  //rotate pi/2
//};
// //Vec V(const Pos& d1, const Pos& d2) { return { d2.y - d1.y, d1.x - d2.x }; }
// //Line L(const Vec& v, const Pos& p) { return { v.vy, v.vx, v.vy * p.x + v.vx * p.y }; }
//Pos m1 = m(d1, d2), m2 = m(d2, d3);
//Vec v1 = V(d1, d2), v2 = V(d2, d3);
//Line l1 = L(~v1, m1), l2 = L(~v2, m2);
//if (z(cross(l1, l2))) return { (0.0, 0.0), -1 };
//ld cross(const Line& l1, const Line& l2) { return l1.vy * l2.vx - l1.vx * l2.vy; }