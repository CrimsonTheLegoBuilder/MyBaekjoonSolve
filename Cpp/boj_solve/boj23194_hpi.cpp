#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>
typedef long long ll;
typedef double ld;
const int LEN = 101;
const ld TOL = 1e-8;
int T, N, seq[LEN];

bool z(ld x) { return std::fabs(x) < TOL; }  // x == zero ?
struct Pos { ld x, y; } pos[LEN], pu, pl, pr, pd;
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
Pos IP(const Line& l1, const Line& l2) {
	ld det = l1 / l2;  //ld det = l1.vy * l2.vx - l1.vx * l2.vy;
	return { (l1.c * l2.vx - l1.vx * l2.c) / det, (l1.vy * l2.c - l1.c * l2.vy) / det };
}
bool CW(const Line& l1, const Line& l2, const Line& l) {
	if (l1 / l2 <= 0) return 0;
	Pos p = IP(l1, l2);
	return l.vy * p.x + l.vx * p.y >= l.c;
}
bool half_plane_intersection(std::vector<Line>& L, std::vector<Pos>& INX) {
	std::deque<Line> D;
	std::sort(L.begin(), L.end());
	for (const Line& l : L) {
		if (!D.empty() && z(D.back() / l)) continue;
		while (D.size() >= 2 && CW(D[D.size() - 2], D.back(), l)) D.pop_back();
		while (D.size() >= 2 && CW(l, D[0], D[1])) D.pop_front();
		D.push_back(l);
	}
	while (D.size() > 2 && CW(D[D.size() - 2], D.back(), D[0])) D.pop_back();
	while (D.size() > 2 && CW(D.back(), D[0], D[1])) D.pop_front();
	std::vector<Pos> h;
	for (int i = 0; i < D.size(); i++) {
		Line cur = D[i], nxt = D[(i + 1) % D.size()];
		if (cur / nxt < TOL) return 0;
		h.push_back(IP(cur, nxt));
	}
	INX = h;
	return 1;
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ld A(std::vector<Pos>& H) {
	Pos O = { 0, 0 };
	int l = H.size();
	ld a = 0;
	for (int i = 0; i < l; i++) {
		a += cross(O, H[i], H[(i + 1) % l]);
	}
	return a / 2;
}
ld area(Pos p[], int s[]) {  //inner check
	std::vector<Line> HP;
	HP.push_back(L(pr, pu)); HP.push_back(L(pu, pl)); HP.push_back(L(pl, pd)); HP.push_back(L(pd, pr));
	for (int i = 1; i < N; i++) {
		for (int j = i + 1; j <= N; j++) {
			ld dy = p[s[i]].y - p[s[j]].y;
			ld dx = p[s[j]].x - p[s[i]].x;  // -(p[l].x - p[r].x)
			ld c = dy * p[s[i]].x + dx * p[s[i]].y;
			HP.push_back({ dy, dx, c });
		}
	}
	std::vector<Pos> INX;
	if (!half_plane_intersection(HP, INX)) return 0;
	
	ld a = A(INX);
	return a;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	ld dx, dy;
	std::cin >> dx >> dy >> N;
	pu = { dx, dy }, pl = { 0, dy }, pr = { dx, 0 }, pd = { 0, 0 };
	for (int i = 1; i <= N; i++) { std::cin >> pos[i].x >> pos[i].y; }
	for (int i = 1; i <= N; i++) std::cin >> seq[i];
	std::cout << area(pos, seq);
	return 0;
}
