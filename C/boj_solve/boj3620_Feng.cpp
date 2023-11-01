#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>
typedef long long ll;
typedef double ld;
const int LEN = 1'001;
const ld TOL = 1e-6;
int N;
ld R;

bool z(ld x) { return std::fabs(x) < TOL; }  // x == zero ?
struct Pos { ld x, y; }P1, P2, pos[LEN];
struct Vec { ld vy, vx; };
struct Line {
	ld vy, vx, c;  // a(vy) * x + b(-vx) * y - c == 0;
	bool operator < (const Line& l) const {
		bool f1 = z(vy) ? vx > 0 : vy > 0;
		bool f2 = z(l.vy) ? l.vx > 0 : l.vy > 0;  // sort CCW
		if (f1 != f2) return f1 > f2;
		ld ccw = vy * l.vx - vx * l.vy;  // ccw == 0 : parallel
		return z(ccw) ? c * hypot(l.vx, l.vy) < l.c * hypot(vx, vy) : ccw > 0;  // sort by distance
	}
};
ld cross(const Vec& v1, const Vec& v2) {
	return v1.vy * v2.vx - v1.vx * v2.vy;  // a(vy) * x + b(-vx) * y - c == 0;
}
ld cross(const Line& l1, const Line& l2) {
	return l1.vy * l2.vx - l1.vx * l2.vy;  // a(vy) * x + b(-vx) * y - c == 0;
}
//ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
//	return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x);
//}
ld dist(const Pos& d1, const Pos& d2) {
	return hypot((d2.x - d1.x), (d2.y - d1.y));
}
//Vec V(std::vector<Pos>& H, int p1, int p2) {
//	p1 %= H.size(), p2 %= H.size();
//	return { H[p2].x - H[p1].x, H[p2].y - H[p1].y };
//}
Vec V(std::vector<Pos>& H, int p1) {
	int p2 = (p1 + 1) % H.size();
	p1 %= H.size();
	return { H[p2].x - H[p1].x, H[p2].y - H[p1].y };
}
ld rotating_calipers(std::vector<Pos>& H) {
	int f = 0, l = H.size();
	if (l <= 2) {
		P1 = H[0], P2 = H[l - 1];
		return dist(H[0], H[l - 1]);
	}
	ld MD = 0;
	for (int i = 0; i < l; i++) {
		while (cross(V(H, i), V(H, f)) > TOL) {
			if (MD < dist(H[i], H[f])) {
				MD = dist(H[i], H[f]);
				P1 = H[i], P2 = H[f];
				//std::cout << P1.x << " " << P1.y << " " << P2.x << " " << P2.y << "\n";
			}
			f = (f + 1) % l;
		}
		if (MD < dist(H[i], H[f])) {
			MD = dist(H[i], H[f]);
			P1 = H[i], P2 = H[f];
			//std::cout << P1.x << " " << P1.y << " " << P2.x << " " << P2.y << "\n";
		}
	}
	return MD;
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
void half_plane_inx(std::vector<Line>& L, std::vector<Pos>& INX) {
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
		//if (cross(cur, nxt) <= TOL) return;
		h.push_back(IP(cur, nxt));
	}
	INX = h;
	return;
}
void DIM(Pos p[], ld m) {  //diameter
	std::vector<Line> HP;
	for (int i = 0; i < N; i++) {
		ld dy = p[i + 1].y - p[i].y;
		ld dx = p[i].x - p[i + 1].x;  // -(p[i + 1].x - p[i].x)
		ld c = dy * p[i].x + dx * p[i].y - hypot(dy, dx) * m + 0.00001;
		HP.push_back({ dy, dx, c });
	}
	std::vector<Pos> INX;
	half_plane_inx(HP, INX);
	ld MD = rotating_calipers(INX);
	return;
}




int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> R;
	for (int i = N; i > 0; i--) {
		std::cin >> pos[i].x >> pos[i].y;
	}
	pos[0] = pos[N];
	std::cout << std::fixed;
	std::cout.precision(5);
	DIM(pos, R);
	std::cout << P1.x << " " << P1.y << " " << P2.x << " " << P2.y << "\n";
	return 0;
}

/*
4 2
0 0
0 4
6 4
6 0
*/

