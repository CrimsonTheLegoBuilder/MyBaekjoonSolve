#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e3;
int N, M, T, Q;
inline bool zero(const ld& x) { return std::abs(x) < TOL; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	inline Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	inline Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	inline ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	inline ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	inline Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	inline Pos operator - () const { return { -x, -y }; }
	inline Pos operator ~ () const { return { -y, x }; }
	inline Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	Pos rot(ld the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	inline ld Euc() const { return x * x + y * y; }
	inline ld mag() const { return sqrt(Euc()); }
	inline Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
inline ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
inline ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
inline bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && (ret > 0 || zero(ret));
}
inline bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && ret > 0;
}
inline bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return !ccw(d1, d2, d3) && !ccw(d1, d2, d4);
}
inline ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2 - d1) * (d4 - d3) / (d2 - d1).mag();
}
inline ld dist(const Pos& d1, const Pos& d2, const Pos& t, bool f = 0) {
	if (f) return std::abs(cross(d1, d2, t)) / (d1 - d2).mag();
	if (sign(projection(d1, d2, d2, t)) <= 0 &&
		sign(projection(d2, d1, d1, t)) <= 0)
		return std::abs(cross(d1, d2, t)) / (d1 - d2).mag();
	return std::min((d1 - t).mag(), (d2 - t).mag());
}
inline Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
inline bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	//return f1 && f2;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
inline bool inner_check(const std::vector<Pos>& H, const Pos& p) {//concave
	int cnt = 0, sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (zero(cur.y - nxt.y)) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return cnt & 1;
}
inline ld area(std::vector<Pos>& H) {
	Pos pivot = Pos(0, 0);
	ld ret = 0;
	int h = H.size();
	for (int i = 0; i < h; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % h];
		ret += cross(pivot, cur, nxt);
	}
	return ret / 2;
}
inline void norm(std::vector<Pos>& H) { if (area(H) < 0) std::reverse(H.begin(), H.end()); }
inline bool valid_check(const Polygon& A, const Polygon& B, const ld& D) {
	int sza = A.size(), szb = B.size();
	for (int i = 0; i < sza; i++) {
		for (int j = 0; j < szb; j++) {
			int ii = (i + 1) % sza;
			int jj = (j + 1) % szb;
			if (intersect(A[i], A[ii], B[j], B[jj])) return 0;
			if (sign(D - dist(A[i], A[ii], B[j])) > 0) return 0;
			if (sign(D - dist(A[i], A[ii], B[jj])) > 0) return 0;
			if (sign(D - dist(B[j], B[jj], A[i])) > 0) return 0;
			if (sign(D - dist(B[j], B[jj], A[ii])) > 0) return 0;
		}
	}
	return !inner_check(A, B[0]) && !inner_check(B, A[0]);
}
inline std::vector<Pos> circle_line_intersection(const Pos& o, const ld& r, const Pos& p1, const Pos& p2) {
	ld d = dist(p1, p2, o, 1);
	if (std::abs(d) > r) return {};
	Pos vec = p2 - p1;
	Pos m = intersection(p1, p2, o, o + ~vec);
	ld distance = vec.mag();
	ld ratio = sqrt(r * r - d * d);
	Pos m1 = m - vec * ratio / distance;
	Pos m2 = m + vec * ratio / distance;
	if (dot(p1, p2, m1, m2) < 0) std::swap(m1, m2);
	return { m1, m2 };//p1->p2
}
inline std::vector<ld> dists(const Pos& a1, const Pos& a2, const Pos& b, const ld& D) {
	std::vector<ld> ret;
	std::vector<Pos> tmp;
	Pos v = ~(a1 - a2).unit();
	Pos p1 = a1 + v * D, p2 = a2 + v * D;
	Pos q1 = a1 - v * D, q2 = a2 - v * D;
	Pos b1 = b + Pos(1, 0);
	if (ccw(b, b1, p1) * ccw(b, b1, p2) <= 0) tmp.push_back(intersection(b, b1, p1, p2));
	if (ccw(b, b1, q1) * ccw(b, b1, q2) <= 0) tmp.push_back(intersection(b, b1, q1, q2));
	auto inx1 = circle_line_intersection(a1, D, b, b1);
	for (const Pos& p : inx1) tmp.push_back(p);
	auto inx2 = circle_line_intersection(a2, D, b, b1);
	for (const Pos& p : inx2) tmp.push_back(p);
	for (const Pos& p : tmp) {
		ld d = dist(a1, a2, p);
		if (sign(D - d) >= 0) ret.push_back((b - p).mag() * sign(dot(b, b1, p)));
	}
	return ret;
}
inline bool query() {
	ld D;
	ld maxx = -INF, minx = INF;
	ld amaxx = -INF, aminx = INF;
	ld amaxy = -INF, aminy = INF;
	ld bmaxx = -INF, bminx = INF;
	ld bmaxy = -INF, bminy = INF;

	std::cin >> D;
	if (zero(D)) return 0;
	std::cin >> N;
	Polygon A(N);
	for (Pos& p : A) {
		std::cin >> p;
		maxx = std::max(maxx, p.x);
		minx = std::min(minx, p.x);
		amaxx = std::max(amaxx, p.x);
		aminx = std::min(aminx, p.x);
		amaxy = std::max(amaxy, p.y);
		aminy = std::min(aminy, p.y);
	}
	norm(A);
	std::cin >> M;
	Polygon B(M);
	for (Pos& p : B) {
		std::cin >> p;
		maxx = std::max(maxx, p.x);
		minx = std::min(minx, p.x);
		bmaxx = std::max(bmaxx, p.x);
		bminx = std::min(bminx, p.x);
		bmaxy = std::max(bmaxy, p.y);
		bminy = std::min(bminy, p.y);
	}
	norm(B);

	if (sign((aminy - bmaxy) - D) >= 0 || sign((bminy - amaxy) - D) >= 0) {
		ld ans = std::min(amaxx - aminx, bmaxx - bminx);
		std::cout << ans << "\n";
		return 1;
	}

	//brute
	ld ans = INF;
	bool F = 0;
	if (valid_check(A, B, D)) ans = std::min(ans, maxx - minx);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			int ii = (i + 1) % N;
			auto VD = dists(A[i], A[ii], B[j], D);
			//std::cout << "DEBUG:: VD " << VD.size() << " DEBUG::\n";
			for (const ld& d : VD) {
				F = 1;
				Polygon MB;
				for (const Pos& p : B) MB.push_back(p + Pos(1, 0) * d);
				if (valid_check(MB, A, D)) {
					ld MIN = std::min(aminx, bminx + d);
					ld MAX = std::max(amaxx, bmaxx + d);
					ans = std::min(ans, MAX - MIN);
				}
			}
			int jj = (j + 1) % M;
			auto WD = dists(B[j], B[jj], A[i], D);
			//std::cout << "DEBUG:: WD" << WD.size() << " DEBUG::\n";
			for (const ld& d : WD) {
				F = 1;
				Polygon MA;
				for (const Pos& p : A) MA.push_back(p + Pos(1, 0) * d);
				if (valid_check(MA, B, D)) {
					ld MIN = std::min(aminx + d, bminx);
					ld MAX = std::max(amaxx + d, bmaxx);
					ans = std::min(ans, MAX - MIN);
				}
			}
		}
	}
	if (!F) ans = std::min(amaxx - aminx, bmaxx - bminx);
	std::cout << ans << "\n";
	return 1;
}
inline void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	//freopen("shy_in.txt", "r", stdin);
	//freopen("shy_out.txt", "w", stdout);
	query();
	return;
}
int main() { solve(); return 0; }//boj1879 Shy Polygon

/*

10.5235
3
0 0
100 100
0 100
4
0 50
20 50
20 80
0 80
0

114.882476

	if (!F) ans = std::min(amaxx - aminx, bmaxx - bminx);
	ans *= 100;
	ans += 0.5;
	ll a = ans;
	ans = a / 100.;
	std::cout << ans << "\n";
	return 1;

*/