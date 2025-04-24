#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>
typedef long long ll;
typedef long double ld;
const int LEN = 1'001;
const ld TOL = 1e-8;
int T, N;

bool z(ld x) { return std::fabs(x) < TOL; }  // x == zero ?
struct Pos { ld x, y; };
std::vector<Pos> pos;
struct Vec { ld vy, vx; };
struct Line {
	ld vy, vx, c;  // a(vy) * x + b(-vx) * y + c == 0;
	bool operator < (const Line& l) const {
		bool f1 = z(vy) ? vx > 0 : vy > 0;
		bool f2 = z(l.vy) ? l.vx > 0 : l.vy > 0;  // sort CCW
		if (f1 != f2) return f1 > f2;
		ld ccw = vy * l.vx - vx * l.vy;  // ccw == 0 : parallel
		return z(ccw) ? c * hypot(l.vx, l.vy) < l.c * hypot(vx, vy) : ccw > 0;  // sort by distance
	}
};
ld cross(const Vec& v1, const Vec& v2) {
	return v1.vy * v2.vx - v1.vx * v2.vy;  // a(vy) * x + b(-vx) * y + c == 0;
}
ld cross(const Line& l1, const Line& l2) {
	return l1.vy * l2.vx - l1.vx * l2.vy;  // a(vy) * x + b(-vx) * y + c == 0;
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ld dist(const Pos& d1, const Pos& d2) {
	return hypot((d2.x - d1.x), (d2.y - d1.y));
}
//Vec V(std::vector<Pos>& H, int p1, int p2) {
//	p1 %= H.size(), p2 %= H.size();
//	return { H[p2].x - H[p1].x, H[p2].y - H[p1].y };
//}
//Vec V(std::vector<Pos>& H, int p1) {
//	int p2 = (p1 + 1) % H.size();
//	p1 %= H.size();
//	return { H[p2].x - H[p1].x, H[p2].y - H[p1].y };
//}
//ld rotating_calipers(std::vector<Pos>& H) {
//	int f = 0, l = H.size();
//	ld MD = 0;
//	for (int i = 0; i < l; i++) {
//		while (cross(V(H, i), V(H, f)) > -TOL) {
//			if (MD < dist(H[i], H[f])) {
//				MD = dist(H[i], H[f]);
//				P1 = H[i], P2 = H[f];
//				//std::cout << P1.x << " " << P1.y << " " << P2.x << " " << P2.y << "\n";
//			}
//			f = (f + 1) % l;
//		}
//		if (MD < dist(H[i], H[f])) {
//			MD = dist(H[i], H[f]);
//			P1 = H[i], P2 = H[f];
//			//std::cout << P1.x << " " << P1.y << " " << P2.x << " " << P2.y << "\n";
//		}
//	}
//	return MD;
//}
ld A(std::vector<Pos>& H) {
	Pos O = { 0, 0 };
	int l = H.size();
	ld a = 0;
	for (int i = 0; i < l; i++) {
		a += cross(O, H[i], H[(i + 1) % l]);
	}
	return a / 2;
}
Pos IP(const Line& l1, const Line& l2) {
	//ld det = l1.vy * l2.vx - l1.vx * l2.vy;
	ld det = cross(l1, l2);
	return { (l1.c * l2.vx - l1.vx * l2.c) / det, (l1.vy * l2.c - l1.c * l2.vy) / det };
}
bool CW(const Line& l1, const Line& l2, const Line& l) {
	if (cross(l1, l2) <= 0) return 0;
	Pos p = IP(l1, l2);
	return l.vy * p.x + l.vx * p.y >= l.c;
}
bool half_plane_inx(std::vector<Line>& L, std::vector<Pos>& INX) {
	std::deque<Line> D;
	std::sort(L.begin(), L.end());
	for (const Line& l : L) {
		if (!D.empty() && z(cross(D.back(), l))) continue;
		while (D.size() >= 2 && CW(D[D.size() - 2], D.back(), l)) D.pop_back();
		while (D.size() >= 2 && CW(l, D[0], D[1])) D.pop_front();
		D.push_back(l);
	}
	while (D.size() > 2 && CW(D[D.size() - 2], D.back(), D[0])) D.pop_back();
	while (D.size() > 2 && CW(D.back(), D[0], D[1])) D.pop_front();
	//if (D.size() < 3) return 0;
	std::vector<Pos> h;
	for (int i = 0; i < D.size(); i++) {
		Line cur = D[i], nxt = D[(i + 1) % D.size()];
		if (cross(cur, nxt) <= TOL) return 0;
		h.push_back(IP(cur, nxt));
	}
	INX = h;
	return 1;
}
ld trial(std::vector<Pos>& p) {  //area
	std::vector<Line> HP;
	for (int i = 0; i < N; i++) {
		ld dy = p[(i + 1) % N].y - p[i].y;
		ld dx = p[i].x - p[(i + 1) % N].x;  // -(p[i + 1].x - p[i].x)
		ld c = dy * p[i].x + dx * p[i].y; // -hypot(dy, dx) * m;
		HP.push_back({ dy, dx, c });
	}
	std::vector<Pos> INX;
	if (!half_plane_inx(HP, INX)) return 0;
	ld area = A(INX);
	return area;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	//freopen("D.dat", "r", stdin);
	std::cin >> T;
	while (T--) {
		std::cin >> N;
		pos.resize(N);
		for (int i = 0; i < N; i++) {
			std::cin >> pos[i].x >> pos[i].y;
		}
		if (z(A(pos))) {
			std::cout << "0.00\n"; continue;
		}
		if (A(pos) < 0) std::reverse(pos.begin(), pos.end());
		std::cout << trial(pos) << "\n";
	}
	return 0;
}