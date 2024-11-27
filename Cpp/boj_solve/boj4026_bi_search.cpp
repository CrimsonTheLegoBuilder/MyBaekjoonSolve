#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
#include <random>
#include <array>
#include <tuple>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e3;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }

Vld ans;
int T, N, M;
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	//bool operator < (const Pos& r) const { return x == r.x ? y == r.y ? d < r.d : y < r.y : x < r.x; }
	//bool operator < (const Pos& r) const { return zero(x - r.x) ? zero(y - r.y) ? d < r.d : y < r.y : x < r.x; }
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
	Pos rot(ld the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << "(" << p.x << ", " << p.y << ")"; return os; }
}; const Pos O = Pos(0, 0);
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
ld area(const Polygon& H) {
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
std::vector<Pos> graham_scan(std::vector<Pos>& C) {
	std::vector<Pos> H;
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
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
ld bi_search(Polygon H, ld& h, ld& w) {
	//Pos v = H[0];
	//for (Pos& p : H) p -= v;
	//ld s = (H[2] - H[0]).rad(), e = s - rad(H[1] - H[0], H[2] - H[0]);
	//int cnt = 30; while (cnt--) {
	//	ld m = (s + e) * .5;
	//	//std::cout << "m:: " << m << "\n";
	//	Pos b1 = Pos(1, 0).rot(m), b2 = ~b1;
	//	if (cross(O, b2, H[1], H[3]) < 0) { s = m; continue; }
	//	w = dot(O, b1, H[0], H[2]);
	//	h = dot(O, b2, H[1], H[3]);
	//std::cout << h << " " << w << "\n";
	//	if (h > w) e = m;
	//	else s = m;
	//}
	for (int _ = 0; _ < 2; _++) {
		for (int i = 0; i < 4; i++) {
			Pos v = H[i];
			Polygon C = H;
			rotate(C.begin(), C.begin() + i, C.end());
			//std::cout << C[0] << "\n";
			//for (Pos& p : C) p -= v;
			ld t = -TOL + std::min(rad(C[1] - C[0], C[2] - C[0]), PI * .5 - rad(C[2] - C[0], C[3] - C[0]));
			//assert(t >= 0);
			ld s = (C[2] - C[0]).rad(), e = s - t;
			int cnt = 30; while (cnt--) {
				ld m = (s + e) * .5;
				//std::cout << "m:: " << m << "\n";
				Pos b1 = Pos(1, 0).rot(m), b2 = ~b1;
				if (dot(O, b2, H[3], H[2]) > 0 || dot(O, b1, H[2], H[1]) > 0) { e = m; continue; }
				w = dot(O, b1, C[0], C[2]);
				h = dot(O, b2, C[1], C[3]);
				assert(w >= 0);
				//std::cout << h << " " << w << "\n";
				if (h < 0) { e = m; continue; }
				if (h < w) e = m;
				else s = m;
			}
			if (eq(h, w)) std::cout << H[i] << " " << s << " :: theta::\n";
			if (eq(h, w)) return (h + w) * .5;
		}
		for (Pos& p : H) p.x *= -1;
	}
	return -1;
}
ld ratio(const Pos& p1, const Pos& p2, const ld& t, ld& h, ld& w) {
	Pos ho = Pos(1, 0).rot(t);
	Pos ve = ~ho;
	Pos b1 = intersection(O, ho, p1, p1 + ve);
	Pos b2 = intersection(p2, p2 + ho, b1, b1 + ve);
	if (ccw(p2, b2, p1) >= 0) return -1;
	h = b1.mag();
	w = (b1 - b2).mag();
	return h / w;
}
void query(int tc) {
	std::cout << "Case " << tc << ": ";
	ld h = -1, w = -2;
	bool f0 = 0;
	Polygon P(4);
	for (Pos& p : P) std::cin >> p;
	P = graham_scan(P);
	if (P.size() < 4) { std::cout << "sz < 3\n"; }
	if (P.size() < 4) { ans.push_back(-1); return; }
	Pos p1 = P[0], p2 = P[2], q1 = P[1], q2 = P[3];
	if (dot(p1, p2, q1) >= 0 || dot(p2, p1, q1) >= 0 ||
		dot(p1, p2, q2) >= 0 || dot(p2, p1, q2) >= 0 ||
		dot(q1, q2, p1) >= 0 || dot(q2, q1, p1) >= 0 ||
		dot(q1, q2, p2) >= 0 || dot(q2, q1, p2) >= 0) {
		f0 = 1;
	}
	if ((dot(p1, p2, q1) >= 0 && dot(p2, p1, q2) >= 0) ||
		(dot(p1, p2, q2) >= 0 && dot(p2, p1, q1) >= 0) ||
		(dot(q1, q2, p1) >= 0 && dot(q2, q1, p2) >= 0) ||
		(dot(q1, q2, p2) >= 0 && dot(q2, q1, p1) >= 0)) {
		ans.push_back(-1); return;
	}
	Pos v1 = p2 - p1, v2 = q2 - q1;
	if (zero(v1 * v2) && eq(v1.mag(), v2.mag())) { std::cout << "perp && eq\n"; }
	if (zero(v1 * v2) && eq(v1.mag(), v2.mag())) {
		//std::cout << v1.mag() + 10 << "\n";
		ans.push_back(v1.mag() + 10);
		return;
	}
	ld d1 = v1.Euc(), d2 = v2.Euc();
	if (d2 < d1) std::swap(d1, d2); 
	if (sign(d1 - (d1 + d1)) >= 0) { std::cout << "too long\n"; }
	if (sign(d1 - (d1 + d1)) >= 0) { ans.push_back(-1); return; }
	ld t = std::abs(rad(v1, v2));
	t = std::min(t, PI - t);
	ld l1 = v1.mag(), l2 = v2.mag();
	if (l1 < l2) std::swap(l1, l2);
	p1 = Pos(l1, 0);
	p2 = Pos(l2, 0).rot(t);
	std::cout << "p1:: " << p1 << " p2:: " << p2 << "\n";
	std::cout << "l1:: " << l1 << " l2:: " << l2 << "\n";
	//Pos v = ~(p2 - p1);
	//Pos inx = intersection(p1, p2, O, v);
	//ld tt = rad()
	//bool f1 = 0;
	//if (dot(inx, p2, p1) > 0 && )
	ld s = 0, e = -(PI * .5 - t), m;
	int cnt = 30;
	while (cnt--) {
		m = (s + e) * .5;
		if (ratio(p1, p2, m, h, w) > 1) s = m;
		else e = m;
		//std::cout << s << " " << h << " " << w << "\n";
	}
	//if (eq(h, w)) std::cout << w + 10 << "\n";
	//else std::cout << "no solution\n";
	std::cout << h << " " << w << "\n";
	if (eq(h, w)) { ans.push_back(h + 10); return; }
	//if (!f0) {
		bi_search(P, h, w);
		std::cout << h << " " << w << "\n";
		if (eq(h, w)) { ans.push_back(h + 10); return; }
	//}
	ans.push_back(-1);
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> T;
	for (int tc = 1; tc <= T; tc++) query(tc);
	for (int tc = 1; tc <= T; tc++) {
		if (ans[tc - 1] < 0) std::cout << "Case " << tc << ": " << "no solution\n";
		else std::cout << "Case " << tc << ": " << ans[tc - 1] << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj4026

/*
1
0 20 11 50 50 -2 63 0

*/