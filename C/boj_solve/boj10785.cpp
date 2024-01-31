#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-7;
const ld INF = 1e7;
const ld LIMIT = 3e4;
int N, M;

bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	Pos(ld X, ld Y) : x(X), y(Y) {}
	Pos() : x(0), y(0) {}
	//bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator == (const Pos& p) const { return z(x - p.x) && z(y - p.y); }
	//bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	ld operator ! () const { return x * y; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld mag() const { return hypot(x, y); }
} velA, velB; const Pos O = { 0, 0 };
std::vector<Pos> HA, HB;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return z(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld dot_ = dot(d1, d3, d2);
	return z(cross(d1, d2, d3)) && (dot_ > 0 || z(dot_));
}
ld dist(const Pos& d1, const Pos& d2, const Pos& target) {
	if (dot(d1, d2, target) < 0 && dot(d2, d1, target) < 0) return (cross(d1, d2, target) / (d1 - d2).mag());
	else return (std::min((d1 - target).mag(), (d2 - target).mag()));
}
bool cross_check(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	bool f1 = ccw(d1, d2, d3) * ccw(d2, d1, d4) > 0;
	bool f2 = ccw(d3, d4, d1) * ccw(d4, d3, d2) > 0;
	bool f3 = on_seg(d1, d2, d3) || on_seg(d1, d2, d4) || on_seg(d3, d4, d1) || on_seg(d3, d4, d2);
	return (f1 && f2) || f3;
}
std::vector<Pos> moved_hull(const std::vector<Pos>& H, const Pos& dir, const ld& t) {
	std::vector<Pos> nxt;
	Pos disp = dir * t;
	for (const Pos& h : H) nxt.push_back(h + disp);
	return nxt;
}
int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {
	int sz = H.size();
	if (sz < 3 || cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return 0;
	if (on_seg(H[0], H[1], p) || on_seg(H[0], H[sz - 1], p)) return 1;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0 || on_seg(H[s], H[e], p)) return 1;
	else 0;
}
bool cross_check(const std::vector<Pos>& H1, const std::vector<Pos>& H2, const Pos& vel2) {
	for (const Pos& h : H2) {
		Pos nxt = h + (vel2 * 2e4), pr = H1[0], pl = H1[0];
		for (int i = 1; i < H1.size(); i++) {
			if (ccw(h, pr, H1[i]) < 0) pr = H1[i];
			if (ccw(h, pl, H1[i]) > 0) pl = H1[i];
		}
		if (cross_check(h, nxt, pl, pr)) return 1;
	}
	return 0;
}
ld cal_dist_Euc(const std::vector<Pos>& H1, const std::vector<Pos>& H2, const Pos& vel2, const ld& t, bool f = 0) {
	std::vector<Pos> MH2 = moved_hull(H2, vel2, t);
	ld d = !f ? INF : -INF;
	int sz1 = H1.size(), sz2 = MH2.size();
	for (int i = 0; i < sz1; i++) {
		if (inner_check_bi_search(MH2, H1[i])) return 0;
		for (int j = 0; j < sz2; j++) {
			Pos cur = MH2[j], nxt = MH2[(j + 1) % sz2];
			if (!f) d = std::min(d, dist(cur, nxt, H1[i]));
			if (f) d = std::max(d, dist(cur, nxt, H1[i]));
		}
	}
	for (int j = 0; j < sz2; j++) {
		if (inner_check_bi_search(H1, MH2[j])) return 0;
		for (int i = 0; i < sz1; i++) {
			Pos cur = H1[i], nxt = H1[(i + 1) % sz1];
			if (!f) d = std::min(d, dist(cur, nxt, MH2[j]));
			if (f) d = std::max(d, dist(cur, nxt, MH2[j]));
		}
	}
	return d;
}
ld ternary_search(const ld t) {
	ld s = 0, e = t, l, r, L, R;
	int cnt = 100;
	while (cnt--) {
		l = (s + s + e) / 3;
		r = (s + e + e) / 3;
		L = cal_dist_Euc(HA, HB, velB, l);
		R = cal_dist_Euc(HA, HB, velB, r);
		if (z(L) && z(R)) e = r;
		else if (L < R) e = r;
		else s = l;
	}
	return s;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	Pos P;

	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> P.x >> P.y, HA.push_back(P);
	std::cin >> velA.x >> velA.y;

	std::cin >> M;
	for (int j = 0; j < M; j++) std::cin >> P.x >> P.y, HB.push_back(P);
	std::cin >> velB.x >> velB.y;
	
	Pos rel_b = velB - velA;
	if (!cross_check(HA, HB, rel_b)) std::cout << "never\n";
	else {
		ld low_vel = std::min(std::abs(rel_b.x), std::abs(rel_b.y));
		ld timelimit = (cal_dist_Euc(HA, HB, rel_b, 0, 1) / low_vel) * 2;
		ld ans = ternary_search(timelimit);
		std::cout << ans << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj10785
