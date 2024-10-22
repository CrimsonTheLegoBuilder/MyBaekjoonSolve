/* https://codeforces.com/blog/entry/68748 */
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
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-6;
const ld PI = acos(-1);
const int LEN = 20005;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& u, const ld& v) { return zero(u - v); }

#define INT

void cut(const int& i, const int& cnt) { std::cout << "scissors\n"; std::cout << i << " " << cnt << "\n"; }
void tape(const int& cnt, const Vint& I) { std::cout << "tape\n"; std::cout << cnt; for (const int& i : I) std::cout << " " << i; std::cout << "\n"; }

int N, M;
struct Pii {
	int x, y;
	Pii(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x &&y == p.y; }
	bool operator != (const Pos& p) const { return x == p.x || y == p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
	Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
	Pii operator * (const int& scalar) const { return { x * scalar, y * scalar }; }
	Pii operator / (const int& scalar) const { return { x / scalar, y / scalar }; }
	ll operator * (const Pii& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pii& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
	Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
	Pii& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pii& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pii operator - () const { return { -x, -y }; }
	Pii operator ~ () const { return { -y, x }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
}; const Pii Oii = Pii();
typedef std::vector<Pii> Vpii;
Vpii Sii, Tii;
void print(const Vpii& H) { std::cout << H.size(); for (const Pii& p : H) std::cout << " " << p; std::cout << "\n"; return; }
ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pii& d1, const Pii& d2, const Pii& d3) { ll ret = cross(d1, d2, d3); return sign(ret); }
int ccw(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { ll ret = cross(d1, d2, d3, d4); return sign(ret); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pii& d1, const Pii& d2, const Pii& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pii& d1, const Pii& d2, const Pii& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
bool intersect(const Pii& s1, const Pii& s2, const Pii& d1, const Pii& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	return f1 && f2;
}
ll area(const Vpii& H) {
	ll a = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) a += H[i] / H[(i + 1) % sz];
	return a;
}
void norm(Vpii& H) { ll a = area(H); if (sign(a) < 0) std::reverse(H.begin(), H.end()); }
bool inside(const Pii& p0, const Pii& p1, const Pii& p2, const Pii& q) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) > 0 || ccw(p1, p2, q) > 0;
	return ccw(p0, p1, q) > 0 && ccw(p1, p2, q) > 0;
}
bool closer(const Vpii& H, const int& i, const int& j) {
	int sz = H.size();
	int i0 = (i - 1 + sz) % sz, i2 = (i + 1) % sz;
	if (!inside(H[i0], H[i], H[i2], H[j])) return 0;
	for (int k0 = 0; k0 < sz; k0++) {
		int k1 = (k0 + 1) % sz;
		if (k0 == i || k0 == j || k1 == i || k1 == j) continue;
		if (intersect(H[i], H[j], H[k0], H[k1])) return 0;
	}
	return 1;
}
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
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
Polygon S, T;
Polygon P[LEN]; int t;
Polygon TRI[20]; int tr;
Pos cnv(const Pii& p) { return Pos(p.x, p.y); }
void print(const Polygon& H) { std::cout << H.size(); for (const Pos& p : H) std::cout << " " << p; std::cout << "\n"; return; }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ld ret = cross(d1, d2, d3, d4); return sign(ret); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	return f1 && f2;
}
ld area(const Polygon& H) {
	ld a = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) a += H[i] / H[(i + 1) % sz];
	return a * .5;
}
void norm(Polygon& H) { ld a = area(H); if (sign(a) < 0) std::reverse(H.begin(), H.end()); }
Polygon box(const ld& x0, const ld& y0, const ld& x1, const ld& y1) { return { Pos(x0, y0), Pos(x1, y0), Pos(x1, y1), Pos(x0, y1) }; }
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) > 0 || ccw(p1, p2, q) > 0;
	return ccw(p0, p1, q) > 0 && ccw(p1, p2, q) > 0;
}
bool closer(const Polygon& H, const int& i, const int& j) {
	int sz = H.size();
	int i0 = (i - 1 + sz) % sz, i2 = (i + 1) % sz;
	if (!inside(H[i0], H[i], H[i2], H[j])) return 0;
	for (int k0 = 0; k0 < sz; k0++) {
		int k1 = (k0 + 1) % sz;
		if (k0 == i || k0 == j || k1 == i || k1 == j) continue;
		if (intersect(H[i], H[j], H[k0], H[k1])) return 0;
	}
	return 1;
}
#ifdef INT
void triangulation_int(const Vpii& H) {
	int sz = H.size();
	if (sz == 3) {
		const Pii& p0 = H[0], p1 = H[1], p2 = H[2];
		ll l0 = (p0 - p1).Euc(), l1 = (p1 - p2).Euc(), l2 = (p2 - p0).Euc();
		ll lmax = std::max({ l0, l1, l2 });
		Polygon tri;
		for (int i = 0; i < 3; i++) {
			const Pii& q0 = H[i], q1 = H[(i + 1) % 3], q2 = H[(i + 2) % 3];
			Pii v = q0 - q1;
			l0 = v.Euc();
			if (l0 == lmax) {
				tri = { cnv(q0), cnv(q1), cnv(q2) };
				break;
			}
		}
		TRI[tr++] = tri;
		return;
	}
	for (int i = 0; i < sz; i++) {
		for (int j = i + 1; j < sz; j++) {
			if (closer(H, i, j)) {
				Vpii H1, H2;
				for (int k = i; k != (j + 1) % sz; k = (k + 1) % sz) H1.push_back(H[k]);
				for (int k = j; k != (i + 1) % sz; k = (k + 1) % sz) H2.push_back(H[k]);
				triangulation_int(H1);
				triangulation_int(H2);
				return;
			}
		}
	}
	return;
}
#endif
void triangulation(const Polygon& H) {
	int sz = H.size();
	if (sz == 3) {
		const Pos& p0 = H[0], p1 = H[1], p2 = H[2];
		ld l0 = (p0 - p1).mag(), l1 = (p1 - p2).mag(), l2 = (p2 - p0).mag();
		ld lmax = std::max({ l0, l1, l2 });
		Polygon tri;
		for (int i = 0; i < 3; i++) {
			const Pos& q0 = H[i], q1 = H[(i + 1) % 3], q2 = H[(i + 2) % 3];
			Pos v = q0 - q1;
			l0 = v.mag();
			if (eq(l0, lmax)) {
				tri = { q0, q1, q2 };
				break;
			}
		}
		TRI[tr++] = tri;
		return; }
	for (int i = 0; i < sz; i++) {
		for (int j = i + 1; j < sz; j++) {
			if (closer(H, i, j)) {
				Polygon H1, H2;
				for (int k = i; k != (j + 1) % sz; k = (k + 1) % sz) H1.push_back(H[k]);
				for (int k = j; k != (i + 1) % sz; k = (k + 1) % sz) H2.push_back(H[k]);
				triangulation(H1);
				triangulation(H2);
				return;
			}
		}
	}
	return;
}
void tri_to_rect(const Polygon& H, Vint& id) {
	int sz = H.size();
	assert(sz == 3);
	const Pos& p0 = H[0], p1 = H[1], p2 = H[2];
	Pos v = p0 - p1;
	Pos pl = (p0 + p2) * .5, pr = (p1 + p2) * .5;
	Pos m = intersection(pl, pr, p2, p2 + ~v);
	Polygon R0 = { p0, p1, pr, pl }, Tl = { p2, pl, m }, Tr = { p2, m, pr };
	ld dd = (p0 - p1).mag();
	ld dr = (pr - m).mag();
	ld dl = (pl - m).mag();
	ld h = std::abs(cross(p0, p1, p2)) / dd / 2;
	Polygon R0_ = { O, Pos(dd, 0), Pos(dd - dr, h), Pos(dl, h) };
	Polygon Tl_ = { O, Pos(dl, h), Pos(0, h) };
	Polygon Tr_ = { Pos(dd, 0), Pos(dd, h), Pos(dd - dr, h) };
	Polygon R = box(0, 0, dd, h);
	Vint I = { t, t + 1, t + 2 };
	cut(t - 1, 3); print(R0); print(Tl); print(Tr);
	tape(3, I); print(R0_); print(Tl_); print(Tr_); print(R);
	t += 3;
	id.push_back(t);
	P[t++] = R;
	return;
}
void rect_to_rect(const int& i, const ld& x, const ld& y, const ld& l, Vint& id) {
	if (eq(x, l)) { id.push_back(i); return; }
	if (sign(x - l * 2) >= 0 || sign(l - x * 2) >= 0) {
		Polygon R0, R1, R2, R2_;
		ld nx, ny;
		if (sign(x - l * 2) >= 0) {
			nx = x * .5; ny = y * 2;
			R0 = box(0, 0, nx, ny); R1 = box(0, 0, nx, y); R2 = box(nx, 0, x, y);
		}
		else if (sign(l - x * 2) >= 0) {
			nx = x * 2; ny = y * .5;
			R0 = box(0, 0, nx, ny); R1 = box(0, 0, x, ny), R2 = box(0, ny, x, y);
		}
		if (sign(x - l * 2) >= 0) R2_ = box(0, y, nx, ny);
		else if (sign(l - x * 2) >= 0) R2_ = box(x, 0, nx, ny);
		Vint I = { t, t + 1 };
		cut(i, 2); print(R1); print(R2);
		tape(2, I); print(R1); print(R2_); print(R0);
		t += 2;
		P[t++] = R0;
		rect_to_rect(t - 1, nx, ny, l, id);
		return;
	}
	Polygon R0, TZ, TB, TS;
	Pos m, vb, vs;
	ld h = (x * y) / l, dx, dy;
	dx = std::abs(x - l);
	dy = std::abs(y - h);
	R0 = box(0, 0, l, h);
	if (sign(x - l) > 0) {
		m = Pos(l, dy);
		TZ = { O, Pos(l, 0), m, Pos(dx, y), Pos(0, y) };
		TB = { Pos(dx, y), Pos(x, 0), Pos(x, y) };
		TS = { Pos(l, 0), Pos(x, 0), m };
		vb = Pos(-dx, dy);
		vs = Pos(-x, y);
	}
	else {
		m = Pos(dx, h);
		TZ = { O, Pos(x, 0), Pos(x, dy), m, Pos(0, h) };
		TB = { Pos(0, y), Pos(x, dy), Pos(x, y) };
		TS = { Pos(0, h), m, Pos(0, y)};
		vb = Pos(dx, -dy);
		vs = Pos(x, -y);
	}
	cut(i, 3); print(TZ); print(TB); print(TS);
	for (Pos& b : TB) b += vb;
	for (Pos& s : TS) s += vs;
	Vint I = { t, t + 1, t + 2 };
	tape(3, I); print(TZ); print(TB); print(TS); print(R0);
	t += 3;
	id.push_back(t);
	P[t++] = R0;
	return;
}
void rect_to_rect(const int& i, const ld& l, Vint& id) {
	Polygon H = P[i];
	int sz = H.size();
	assert(sz == 4);
	ld x = H[1].x - H[0].x;
	ld y = H[3].y - H[0].y;
	rect_to_rect(i, x, y, l, id);
	return;
}
void rect_to_square(const ld& l, Vint& id) {
	Polygon square = box(0, 0, l, l);
	tape(id.size(), id);
	t += id.size();
	ld h = 0;
	for (const int& i : id) {
		Polygon B = P[i];
		assert(B.size() == 4);
		for (Pos& b : B) b.y += h;
		print(B);
		h = B[2].y;
	}
	print(square);
	P[t++] = square;
	return;
}
void square_split(Vint& id) {
	tr = 1;
#ifdef INT
	assert(area(Sii) == area(Tii));
	ld l = sqrt(area(Tii) * .5);
	triangulation_int(Tii);
#else
	assert(eq(area(S), area(T)));
	ld l = sqrt(area(T));
	triangulation(T);
#endif
	cut(t - 1, tr - 1);
	ld y = 0;
	for (int i = 1; i < tr; i++) {
		ld a = std::abs(area(TRI[i]));
		ld h = a * 2 / l;
		Polygon B = box(0, y, l, y + h);
		print(B);
		id.push_back(t);
		P[t++] = B;
		y += h;
	}
	return;
}
void rect_to_tri(const int& rt, const int& tt, Vint& id) {
	Polygon H = TRI[tt];
	int sz = H.size();
	assert(sz == 3);
	const Pos& p0 = H[0], p1 = H[1], p2 = H[2];
	Pos v = p0 - p1;
	ld l0 = v.mag();
	Vint id;
	rect_to_rect(rt, l0, id);
	assert(id.size() == 1);
	assert(id[0] == t - 1);
	Polygon R = P[t - 1];
	Pos pl = (p0 + p2) * .5, pr = (p1 + p2) * .5;
	Pos m = intersection(pl, pr, p2, p2 + ~v);
	Polygon R0 = { p0, p1, pr, pl }, Tl = { p2, pl, m }, Tr = { p2, m, pr };
	ld dd = (p0 - p1).mag();
	ld dr = (pr - m).mag();
	ld dl = (pl - m).mag();
	ld h = std::abs(cross(p0, p1, p2)) / dd / 2;
	Polygon R0_ = { Pos(0, 0), Pos(dd, 0), Pos(dd - dr, h), Pos(dl, h) };
	Polygon Tl_ = { Pos(0, 0), Pos(dl, h), Pos(0, h) };
	Polygon Tr_ = { Pos(dd, 0), Pos(dd, h), Pos(dd - dr, h) };
	Vint I = { t, t + 1, t + 2 };
	cut(rt, 3); print(R0_); print(Tl_); print(Tr_);
	tape(3, I); print(R0); print(Tl); print(Tr); print(H);
	t += 3;
	id.push_back(t);
	P[t++] = H;
	return;
}
void start_to_square() {
	tr = 1;
#ifdef INT
	ld l = sqrt(area(Sii) * .5);
	triangulation_int(Sii);
#else
	ld l = sqrt(area(S));
	triangulation(S);
#endif
	//P[0] = S
	t = 1;
	for (t; t < tr; t++) P[t] = TRI[t];
	cut(0, tr - 1);
	for (int i = 1; i < t; i++) print(P[i]);
	int t1 = t;
	Vint id;
	for (int i = 1; i < t1; i++) tri_to_rect(P[i], id);
	Vint idx = id; id.clear();
	for (const int& i : idx) rect_to_rect(i, l, id);
	rect_to_square(l, id);
	assert(P[t - 1][2] == Pos(l, l));
	return;
}
void square_to_target() {
	Vint id, idx;
	square_split(idx);
	int tt = 1;
	for (const int& rt : idx) rect_to_tri(rt, tt++, id);
	tape(id.size(), id);
	for (const int& i : id) print(P[i]);
#ifdef INT
	print(Tii);
#else
	print(T);
#endif
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
#ifdef INT
	std::cin >> N;
	Sii.resize(N);
	for (Pii& p : Sii) std::cin >> p;
	assert(area(Sii) > 0);
	std::cin >> M;
	Tii.resize(M);
	for (Pii& p : Tii) std::cin >> p;
	assert(area(Tii) > 0);
#else
	std::cin >> N;
	S.resize(N);
	for (Pos& p : S) std::cin >> p;
	assert(area(S) > 0);
	std::cin >> M;
	T.resize(M);
	for (Pos& p : T) std::cin >> p;
	assert(area(T) > 0);
#endif
	start_to_square();
	square_to_target();
	return;
}
int main() { solve(); return 0; }//boj17645
/* https://codeforces.com/blog/entry/68748 */