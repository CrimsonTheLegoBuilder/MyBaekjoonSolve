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
const int LEN = 30;
int N;
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
bool zero(const ld& x) { return !sign(x); }

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
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
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
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
struct Line {
	Pos s, e;
	Line(Pos S = Pos(), Pos E = Pos()) : s(S), e(E) {}
	Pos dir() const { return (e - s).unit(); }
	ld operator / (const Line& l) const { return cross(s, e, l.s, l.e); }
	bool include(const Pos& q) const { return sign(cross(s, e, q)) > 0; }
	bool operator < (const Line& l) const {
		bool f1 = O < dir();
		bool f2 = O < l.dir();
		if (f1 != f2) return f1;
		int CCW = dir() / l.dir();
		return !CCW ? l.include(s) : CCW;
	}
	Line& operator += (const ld& d) {
		Pos del = dir() * d;
		s += del; e += del;
		return *this;
	}
	Line& operator -= (const ld& d) {
		Pos del = dir() * d;
		s -= del; e -= del;
		return *this;
	}
	Line operator + (const ld& d) const {
		Pos del = dir() * d;
		Pos s_ = s + del, e_ = e + del;
		return Line(s_, e_);
	}
	Line operator - (const ld& d) const {
		Pos del = dir() * d;
		Pos s_ = s - del, e_ = e - del;
		return Line(s_, e_);
	}
};
typedef std::vector<Line> Vline;
Pos intersection(const Line& u, const Line& v) { return intersection(u.s, u.e, v.s, v.e); }
ld area(const Polygon& H) {
	ld ret = 0; int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
	int sz = H.size();
	//if (!sz) return -1;
	//if (sz == 1) return p == H[0] ? 0 : -1;
	//if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
	assert(sz >= 3);
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) >= 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return 1;
	else if (on_seg_strong(H[s], H[e], p)) return 0;
	else return -1;
}
Polygon graham_scan(Polygon& C) {
	Polygon H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	//C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(12);
	std::cin >> N;
	Polygon C(N);
	Vline L;
	for (int i = 0; i < N; i++) std::cin >> C[i];
	for (int i = 0; i < N; i++) {
		Pos& I = C[i];
		for (int j = 0; j < N; j++) {
			Pos& J = C[j], & K = C[(j + 1) % N];
			Pos m1 = (I + J) * .5;
			Pos m2 = (I + K) * .5;
			L.push_back(Line(m1, m2));
		}
	}
	Polygon INX;
	int sz = L.size();
	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			if (i == j) continue;
			if (!sign(L[i] / L[j])) continue;
			INX.push_back(intersection(L[i], L[j]));
		}
	}
	std::sort(INX.begin(), INX.end());
	INX.erase(unique(INX.begin(), INX.end()), INX.end());
	ld ans = INF;
	sz = INX.size();
	for (int i = 0; i < sz; i++) {
		Polygon D = C;
		Pos X = INX[i];
		for (int j = 0; j < N; j++) {
			Pos vec = X - C[j];
			D.push_back(X + vec);
		}
		Polygon H = graham_scan(D);
		ld tmp = -1;
		for (int j = 0; j < N << 1; j++) {
			if (inner_check_bi_search(H, D[j])) { tmp = INF; break; }
		}
		if (tmp < 0) tmp = area(H);
		ans = std::min(ans, tmp);
	}
	std::cout << (ans > 1e16 ? -1 : ans * .5) << "\n";
	return;
}
int main() { solve(); return 0; }//boj31601 Symmetric Boundary