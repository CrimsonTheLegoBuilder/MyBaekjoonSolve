#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef double ld;
const int LEN = 125'001;
const ld TOL = 1e-10;
int T, N, K, M, l, r;
bool V[LEN];

bool z(ld x) { return std::fabs(x) < TOL; }  // x == zero ?
struct Pos { ld x, y; }pos[LEN], pu = { 1e17, 1e17 }, pr = { 1e17, -1 }, pl = { -1, 1e17 }, pd = { -1, -1 };
struct Vec { ld vy, vx; };
struct Line {
	ld vy, vx, c;  // a(vy) * x + b(vx) * y - c == 0;
	bool operator < (const Line& l) const {
		bool f1 = z(vy) ? vx > 0 : vy > 0;
		bool f2 = z(l.vy) ? l.vx > 0 : l.vy > 0;  // sort CCW
		if (f1 != f2) return f1 > f2;
		ld ccw = vy * l.vx - vx * l.vy;  // ccw == 0 : parallel
		return z(ccw) ? c * hypot(l.vx, l.vy) < l.c * hypot(vx, vy) : ccw > 0;  // sort by distance
	}
	ld operator / (const Line& l) const { return vy * l.vx - vx * l.vy; }  //cross
};
Line L(const Pos& s, const Pos& e) {
	ld dy = e.y - s.y;
	ld dx = s.x - e.x;  // -(e.x - s.x)
	ld c = dy * s.x + dx * s.y;// -hypot(dy, dx) * TOL;
	return { dy, dx, c };
}
ld cross(const Vec& v1, const Vec& v2) {
	return v1.vy * v2.vx - v1.vx * v2.vy;  // a(vy) * x + b(vx) * y - c == 0;
}
//ld cross(const Line& l1, const Line& l2) {
//	return l1.vy * l2.vx - l1.vx * l2.vy;  // a(vy) * x + b(vx) * y - c == 0;
//}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ld dist(const Pos& d1, const Pos& d2) {
	return hypot((d2.x - d1.x), (d2.y - d1.y));
}
Pos IP(const Line& l1, const Line& l2) {
	//ld det = cross(l1, l2);  //ld det = l1.vy * l2.vx - l1.vx * l2.vy;
	ld det = l1 / l2;  //ld det = l1.vy * l2.vx - l1.vx * l2.vy;
	return { (l1.c * l2.vx - l1.vx * l2.c) / det, (l1.vy * l2.c - l1.c * l2.vy) / det };
}
bool CW(const Line& l1, const Line& l2, const Line& l) {
	//if (cross(l1, l2) <= 0) return 0;
	if (l1 / l2 <= 0) return 0;
	Pos p = IP(l1, l2);
	return l.vy * p.x + l.vx * p.y >= l.c;
}
bool half_plane_inx(std::vector<Line>& L, std::vector<Pos>& INX) {
	std::deque<Line> D;
	std::sort(L.begin(), L.end());
	for (const Line& l : L) {
		//if (!D.empty() && z(cross(D.back(), l))) continue;
		if (!D.empty() && z(D.back() / l)) continue;
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
		//if (cross(cur, nxt) <= TOL) return 0;
		if (cur / nxt <= TOL) return 0;
		h.push_back(IP(cur, nxt));
	}
	INX = h;
	return 1;
}
bool I(std::vector<Pos>& H, const Pos& x) {
	int s = 0, e = H.size() - 1, m;
	if (e < 2 || cross(H[0], H[1], x) < -TOL || cross(H[0], H[e], x) > TOL) return 0;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], x) > 0) s = m;
		else e = m;
	}
	return cross(H[s], H[e], x) > TOL;
}
void trial(Pos p[]) {  //inner check
	std::vector<Line> HP;
	HP.push_back(L(pr, pu)); HP.push_back(L(pu, pl)); HP.push_back(L(pl, pd)); HP.push_back(L(pd, pr));
	std::cin >> M;
	memset(V, 0, sizeof V);
	while (M--) {
		std::cin >> l >> r;
		V[l] = 1, V[r] = 1;
		ld dy = p[l].y - p[r].y;
		ld dx = p[r].x - p[l].x;  // -(p[l].x - p[r].x)
		ld c = dy * p[l].x + dx * p[l].y;// -hypot(dy, dx) * 1e-5;
		HP.push_back({ dy, dx, c });
	}
	std::vector<Pos> INX;
	if (!half_plane_inx(HP, INX)) { std::cout << "0\n"; return; }
	for (int i = 1; i <= N; i++) { if (!V[i] && I(INX, pos[i])) std::cout << i << "\n"; }  //search
	//for (int i = 1; i <= N; i++) { if (I(INX, pos[i])) std::cout << i << "\n"; }  //search
	std::cout << "0\n";
	return;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("C.in", "r", stdin);
	//freopen("C.ans", "w", stdout);
	std::cin >> T;
	while (T--) {
		std::cin >> N;
		for (int i = 1; i <= N; i++) {
			//std::cout << T << " " << i << "\n";
			std::cin >> pos[i].x >> pos[i].y;
			assert(pos[i].x < (2ll << 28));
			assert(pos[i].x >= 0);
			assert(pos[i].y < (2ll << 28));
			assert(pos[i].y >= 0);
		}
		std::cin >> K;
		while (K--) { trial(pos); }
	}
	return 0;
}



//ld A(std::vector<Pos>& H) {
//	Pos O = { 0, 0 };
//	int l = H.size();
//	ld a = 0;
//	for (int i = 0; i < l; i++) {
//		a += cross(O, H[i], H[(i + 1) % l]);
//	}
//	return a / 2;
//}

//for (int i = 0; i < N; i++) {
//	ld dy = p[i + 1].y - p[i].y;
//	ld dx = p[i].x - p[i + 1].x;  // -(p[i + 1].x - p[i].x)
//	ld c = dy * p[i].x + dx * p[i].y; // -hypot(dy, dx) * m;
//	HP.push_back({ dy, dx, c });
//}