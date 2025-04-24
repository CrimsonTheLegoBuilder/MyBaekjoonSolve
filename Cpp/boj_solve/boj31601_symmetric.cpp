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
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 30;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }

int N;
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
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ld ret = cross(d1, d2, d3, d4); return sign(ret); }
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
};
typedef std::vector<Line> Vline;
Pos intersection(const Line& u, const Line& v) { return intersection(u.s, u.e, v.s, v.e); }
ld area(const Polygon& H) {
	ld ret = 0; int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
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
	for (int i = 0; i < N; i++) std::cin >> C[i], C[i] *= 10;
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
		for (int j = 0; j < (N << 1); j++) {
			bool f = 0;
			for (int k = 0; k < H.size(); k++) {
				Pos p1 = H[k], p2 = H[(k + 1) % H.size()];
				if (on_seg_strong(p1, p2, D[j])) { f = 1; break; }
			}
			if (!f) { tmp = INF; break; }
		}
		if (tmp < 0) tmp = area(H);
		ans = std::min(ans, tmp);
	}
	if (ans > 1e16) std::cout << "-1\n";
	else std::cout << ans * .005 << "\n";
	return;
}
int main() { solve(); return 0; }//boj31601 Symmetric Boundary