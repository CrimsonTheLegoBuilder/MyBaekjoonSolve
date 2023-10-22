#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <cmath>
typedef long long ll;
typedef double ld;
const int LEN = 50'001;
const ld TOL = 1e-8;
int N;

ld z(ld x) { return std::fabs(x) < TOL; }  // x == zero ?
ld dist(ld a, ld b) { return sqrtl(a * a + b * b); }
struct P { int x, y; }pos[LEN];
struct Pos { ld x, y; };
struct Vec { ld vy, vx; };
struct Line {
	ld vy, vx, c;  // a(vy) * x + b(-vx) * y + c == 0;
	bool operator < (const Line& v) const {
		bool f1 = z(vy) ? vx > 0 : vy > 0;
		bool f2 = z(v.vy) ? v.vx > 0 : v.vy > 0;
		if (f1 != f2) return f1 > f2;
		ld ccw = vy * v.vx - vx * v.vy;  // ccw == 0 : parallel
		return z(ccw) ? c * dist(v.vx, v.vy) < v.c * dist(vx, vy) : ccw > 0;  //order by distance
	}
	Vec V(const Line& v) { return { v.vy, v.vx }; }
};
ld cross(const Vec& v1, const Vec& v2) {
	return v1.vy * v2.vx - v1.vx * v2.vy;  // a(vy) * x + b(-vx) * y + c == 0;
}
ld cross(const Line& l1, const Line& l2) {
	return l1.vy * l2.vx - l1.vx * l2.vy;  // a(vy) * x + b(-vx) * y + c == 0;
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {  //cross for rotating calipers
	return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x);
}
ld dist(const Pos& d1, const Pos& d2) {
	return sqrtl((d2.x - d1.x) * (d2.x - d1.x) + (d2.y - d1.y) * (d2.y - d1.y));
}
ld rotating_calipers(std::vector<Pos>& H) {
	int i = 0, f2i = 1, l = H.size();
	ld MD = 0;
	for (i; i <= l; i++) {
		while ((f2i + 1) % l != (i + 1) % l && cross(H[i % l], H[(i + 1) % l], H[f2i % l], H[(f2i + 1) % l]) > -TOL) {
			if (MD < dist(H[i % l], H[f2i % l])) MD = dist(H[i % l], H[f2i % l]);
			f2i++;
		}
		if (MD < dist(H[i % l], H[f2i % l])) MD = dist(H[i % l], H[f2i % l]);
	}
	return MD;
}
Pos IP(const Line& l1, const Line& l2) {
	ld det = l1.vy * l2.vx - l1.vx * l2.vy;
	//ld det = cross(l1, l2);
	return { (l1.c * l2.vx - l1.vx * l2.c) / det, (l1.vy * l2.c - l1.c * l2.vy) / det };
}
bool CW(const Line& l1, const Line& l2, const Line& l) {
	if (cross(l1, l2) <= 0) return 0;
	Pos p = IP(l1, l2);
	return l.vy * p.x + l.vx * p.y >= l.c;
}
bool HPINX(std::vector<Line>& L, std::vector<Pos>& H) {
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
	//std::cout << D.size() << "\n";
	//if (D.size() < 3) return 0;
	std::vector<Pos> h;
	for (int i = 0; i < D.size(); i++) {
		Line cur = D[i], nxt = D[(i + 1) % D.size()];
		//std::cout << cur.vx << " " << cur.vy << "\n";
		if (cross(cur, nxt) <= TOL) return 0;
		h.push_back(IP(cur, nxt));
	}
	//for (const Pos& p : h) {
	//	std::cout << p.x << " " << p.y << "\n";
	//}
	H = h;
	return 1;
}
bool cal_diameter(P p[], ld m) {
	std::vector<Line> HP;
	for (int i = 0; i < N; i++) {
		ld dy = p[i + 1].y - p[i].y;
		ld dx = p[i].x - p[i + 1].x;  // -(p[i + 1].x - p[i].x)
		ld c = dy * p[i].x + dx * p[i].y - dist(dy, dx) * m;
		HP.push_back({ dy, dx, c });
	}
	std::vector<Pos> INX;
	if (!HPINX(HP, INX)) return 0;
	ld MD = rotating_calipers(INX);
	//std::cout << MD << " " << m << "\n";
	return MD >= 2 * m;
}
ld bi_serach(P p[]) {
	ld s = 0, e = 1e7, m;
	for (int i = 0; i < 50; i++) {
		m = (s + e) / 2;
		if (cal_diameter(p, m)) s = m;
		else e = m;
	}
	return s;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> pos[i].x >> pos[i].y;
	}
	pos[N] = pos[0];
	std::cout << std::fixed;
	std::cout.precision(3);
	std::cout << bi_serach(pos) << "\n";
}

