#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>
typedef long long ll;
typedef double ld;
const int LEN = 101;
const ld TOL = 1e-8;
int N;

bool z(ld x) { return std::fabs(x) < TOL; }  // x == zero ?
struct Pos { ld x, y; }pos[LEN];
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
// 		//while (cross(V(H, i, i + 1), V(H, f, f + 1)) > -TOL) {
//		while (cross(V(H, i), V(H, f)) > -TOL) {
//			MD = std::max(MD, dist(H[i], H[f]));
//			f = (f + 1) % l;
//		}
//		MD = std::max(MD, dist(H[i], H[f]));
//	}
//	return MD;
//}

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
bool DIM(Pos p[], ld m) {  //diameter
	std::vector<Line> HP;
	for (int i = 0; i < N; i++) {
		ld dy = p[i + 1].y - p[i].y;
		ld dx = p[i].x - p[i + 1].x;  // -(p[i + 1].x - p[i].x)
		ld c = dy * p[i].x + dx * p[i].y - hypot(dy, dx) * m;
		HP.push_back({ dy, dx, c });
	}
	std::vector<Pos> INX;
	if (!half_plane_inx(HP, INX)) return 0;
	else return 1;
}
ld bi_serach(Pos p[]) {
	ld s = 0, e = 1e5, m;
	for (int i = 0; i < 50; i++) {
		m = (s + e) / 2;
		if (DIM(p, m)) s = m;
		else e = m;
	}
	return s;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	while (1) {
		std::cin >> N;
		if (!N) return 0;
		for (int i = 0; i < N; i++) {
			std::cin >> pos[i].x >> pos[i].y;
		}
		pos[N] = pos[0];
		std::cout << std::fixed;
		std::cout.precision(6);
		std::cout << bi_serach(pos) << "\n";
	}
}
