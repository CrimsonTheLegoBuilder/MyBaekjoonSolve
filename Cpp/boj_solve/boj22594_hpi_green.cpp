#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-8;
const ld PI = acos(-1);
const int LEN = 3e3 + 5;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ld sq(const ld& x) { return x * x; }

int N, M, T, Q;
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	Pos rot(ld the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
struct Linear {//ps[0] -> ps[1]
	Pos ps[2];
	Pos dir_;
	Pos& operator[](int i) { return ps[i]; }
	Pos dir() const { return dir_; }
	Linear(Pos a = Pos(0, 0), Pos b = Pos(0, 0)) {
		ps[0] = a;
		ps[1] = b;
		dir_ = (ps[1] - ps[0]).unit();
	}
	bool include(const Pos& p) const { return sign(dir_ / (p - ps[0])) > 0; }
	friend bool parallel(const Linear& l0, const Linear& l1) { return zero(l0.dir() / l1.dir()); }
	friend bool same_dir(const Linear& l0, const Linear& l1) { return parallel(l0, l1) && l0.dir() * l1.dir() > 0; }
	bool operator < (const Linear& l0) const {
		if (same_dir(*this, l0)) return l0.include(ps[0]);
		else return cmpq(this->dir(), l0.dir());
	}
};
typedef std::vector<Linear> Planes;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
ld dist(const Pos& d1, const Pos& d2, const Pos& t) { return cross(d1, d2, t) / (d1 - d2).mag(); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
Pos projection(const Pos& s1, const Pos& s2, const Pos& p) { return intersection(s1, s2, p, p + ~(s2 - s1)); }
Pos intersection(Linear& l1, Linear& l2) { return intersection(l1[0], l1[1], l2[0], l2[1]); }
//Polygon convex_cut(const std::vector<Pos>& ps, const Pos& b1, const Pos& b2) {
//	std::vector<Pos> qs;
//	int n = ps.size();
//	for (int i = 0; i < n; i++) {
//		Pos p1 = ps[i], p2 = ps[(i + 1) % n];
//		int d1 = ccw(b1, b2, p1), d2 = ccw(b1, b2, p2);
//		if (d1 >= 0) qs.push_back(p1);
//		if (d1 * d2 < 0) qs.push_back(intersection(p1, p2, b1, b2));
//	}
//	return qs;
//}
//Polygon sutherland_hodgman(const std::vector<Pos>& C, const std::vector<Pos>& clip) {
//	int sz = clip.size();
//	std::vector<Pos> ret = C;
//	for (int i = 0; i < sz; i++) {
//		Pos b1 = clip[i], b2 = clip[(i + 1) % sz];
//		ret = convex_cut(ret, b1, b2);
//	}
//	return ret;
//}
ld area(const Polygon& H) {
	ld a = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) a += H[i] / H[(i + 1) % sz];
	return a * .5;
}
Linear bi_sector(const Pos& p, const Pos& q) {
	Pos m = (p + q) * .5;
	Pos v = (q - p);
	return Linear(m, m + ~v);
}
std::vector<Pos> half_plane_intersection(std::vector<Linear>& HP) {
	auto check = [&](Linear& u, Linear& v, Linear& w) -> bool {
		return w.include(intersection(u, v));
		};
	std::sort(HP.begin(), HP.end());
	std::deque<Linear> dq;
	int sz = HP.size();
	for (int i = 0; i < sz; ++i) {
		if (i && same_dir(HP[i], HP[(i - 1) % sz])) continue;
		while (dq.size() > 1 && !check(dq[dq.size() - 2], dq[dq.size() - 1], HP[i])) dq.pop_back();
		while (dq.size() > 1 && !check(dq[1], dq[0], HP[i])) dq.pop_front();
		dq.push_back(HP[i]);
	}
	while (dq.size() > 2 && !check(dq[dq.size() - 2], dq[dq.size() - 1], dq[0])) dq.pop_back();
	while (dq.size() > 2 && !check(dq[1], dq[0], dq[dq.size() - 1])) dq.pop_front();
	sz = dq.size();
	if (sz < 3) return {};
	std::vector<Pos> HPI;
	for (int i = 0; i < sz; ++i) HPI.push_back(intersection(dq[i], dq[(i + 1) % sz]));
	return HPI;
}
Planes make_hp(const Polygon& H) {
	Planes V;
	int sz = H.size();
	for (int i = 0; i < sz; i++) V.push_back(Linear(H[i], H[(i + 1) % sz]));
	return V;
}
ld expect(const Pos& p, const Pos& a, const Pos& b) {
	//https://drken1215.hatenablog.com/entry/2020/10/15/061200
	Pos h = projection(a, b, p);
	ld h_ = (p - h).mag(), a_ = (a - h).mag(), b_ = (b - h).mag();
	if (on_seg_strong(a, b, h)) a_ *= -1;
	return (sq(a_) + sq(b_) + a_ * b_ + sq(h_) * 3) / 6;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> M >> N;
	Polygon H(M); for (Pos& p : H) std::cin >> p;
	Polygon P(N); for (Pos& p : P) std::cin >> p;
	Planes V = make_hp(H);
	ld A = area(H);
	ld ret = 0;
	for (int i = 0; i < N; i++) {
		const Pos& p = P[i];
		Planes HP = V;
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			HP.push_back(bi_sector(p, P[j]));
		}
		Polygon hpi = half_plane_intersection(HP);
		int sz = hpi.size();
		for (int j = 0; j < sz; j++) {
			const Pos& a = hpi[j], & b = hpi[(j + 1) % sz];
			ret += expect(p, a, b) * std::abs(cross(a, b, p)) * .5;
		}
	}
	std::cout << ret / A << "\n";
	return;
}
int main() { solve(); return 0; }//boj22594
