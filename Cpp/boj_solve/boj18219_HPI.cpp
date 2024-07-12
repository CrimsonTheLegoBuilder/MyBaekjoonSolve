#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
typedef long long ll;
typedef long double ld;
//typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 3e3 + 5;
int N, M, T, Q;
bool V[LEN];
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }

///========================================================///
//half plane intersection - refer to bulijiojiodibuliduo
//O(N^2logN + 6QN) power-diagram query
///========================================================///

struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { -x, -y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
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
struct Linear {//ps[0] -> ps[1] :: refer to bulijiojiodibuliduo
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
	Linear push() const {//push eps outward
		const double eps = 1e-8;
		Pos delta = ~(ps[1] - ps[0]).unit() * eps;
		return Linear(ps[0] + delta, ps[1] + delta);
	}
	Linear operator + (const double eps) const {//push eps outward
		Pos delta = ~(ps[1] - ps[0]).unit() * eps;
		return Linear(ps[0] + delta, ps[1] + delta);
	}
	Linear operator - (const double eps) const {//pull eps inward
		Pos delta = ~(ps[1] - ps[0]).unit() * eps;
		return Linear(ps[0] - delta, ps[1] - delta);
	}
	friend bool parallel(const Linear& l0, const Linear& l1) { return zero(l0.dir() / l1.dir()); }
	friend bool same_dir(const Linear& l0, const Linear& l1) { return parallel(l0, l1) && l0.dir() * l1.dir() > 0; }
	bool operator < (const Linear& l0) const {
		if (same_dir(*this, l0)) return l0.include(ps[0]);
		else return cmpq(this->dir(), l0.dir());
	}
};
typedef std::vector<Linear> VHP;
struct Seg {
	Pos s, e;
	Seg(Pos S = Pos(), Pos E = Pos()) : s(S), e(E) {}
};
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return sign(ret);
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
ld dist(const Pos& d1, const Pos& d2, const Pos& t) {
	return cross(d1, d2, t) / (d1 - d2).mag();
}
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && sign(ret) >= 0;
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
Pos intersection(Linear& l1, Linear& l2) { return intersection(l1[0], l1[1], l2[0], l2[1]); }
bool block(const Pos& p0, const Pos& p1, const Pos& d0, const Pos& d1) {
	return ccw(p0, p1, d0) <= 0 && ccw(p0, p1, d1) >= 0 && ccw(d0, d1, p1) >= 0;
}
bool back(const Pos& p0, const Pos& p1, const Pos& d0, const Pos& d1) {
	return ccw(p0, p1, d0) < 0 && ccw(p0, p1, d1) > 0 && ccw(d0, d1, p1) < 0;
}
ld area(std::vector<Pos>& H) {
	Pos pivot = Pos(0, 0);
	ld ret = 0;
	int h = H.size();
	for (int i = 0; i < h; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % h];
		ret += cross(pivot, cur, nxt);
	}
	return ret / 2;
}
void norm(Polygon& H) { if (area(H) < 0) std::reverse(H.begin(), H.end()); }
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N;
	Polygon H(N);
	for (Pos& p : H) std::cin >> p;
	norm(H);
	for (int i = 0; i < N; i++) {
		Pos& p0 = H[i], & p1 = H[(i + 1) % N], & p2 = H[(i + 2) % N];
		if (ccw(p0, p1, p2) < 0) {
			int b = -1;
			int d = -1;
			ld dmin = INF;
			ld dmax = -1;
			Pos inx = Pos();
			for (int j = (i + 2) % N; j != (i + 1) % N; j = (j + 1) % N) {
				int k = (j + 1) % N;
				if (on_seg_strong(p0, H[j], p1) && dist(H[j], H[k], p1) < dmin) {
					b = j;
					inx = H[j];
					dmin = (p1 - H[j]).mag();
				}
				else if (block(p0, p1, H[j], H[k]) && dist(H[j], H[k], p1) < dmin) {
					b = j;
					inx = intersection(p0, p1, H[j], H[k]);
					dmin = (p1 - inx).mag();
				}
			}
			assert(b != -1 && d != -1);
			int u = i;
			bool f = 0;
			while (u != (i + 1) % N) {
				int k = (u - 1 + N) % N;
				if (on_seg_strong(p1, H[u], p0) || (ccw(p0, p1, H[u]) != ccw(p0, p1, H[u]))) {
					break;
				}
				u = (u - 1 + N) % N;
			}
			for (int j = i; j != (u - 1 + N) % N; j = (j - 1 + N) % N) {
				if (ccw(p0, p1, H[j]) < 0) { f = 1; break; }
			}

		}
	}
	return;
}
int main() { solve(); return 0; }//boj18219
