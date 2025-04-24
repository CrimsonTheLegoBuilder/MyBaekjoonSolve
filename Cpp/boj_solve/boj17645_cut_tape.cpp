#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef double ld;
typedef std::vector<int> Vint;
const ld INF = 1e17;
const ld TOL = 1e-6;
const ld PI = acos(-1);
const int LEN = 20005;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& u, const ld& v) { return zero(u - v); }

//https://codeforces.com/blog/entry/68748

void cut(const int& i, const int& cnt) { std::cout << "scissors\n"; std::cout << i << " " << cnt << "\n"; }
void tape(const int& cnt, const Vint& I) { std::cout << "tape\n"; std::cout << cnt; for (const int& i : I) std::cout << " " << i; std::cout << "\n"; }

int N, M;
struct Pii {
	int x, y;
	Pii(int X = 0, int Y = 0) : x(X), y(Y) {}
	Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
	Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pii& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pii& p) const { return (ll)x * p.y - (ll)y * p.x; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
}; const Pii Oii = Pii();
typedef std::vector<Pii> Vpii;
void print(const Vpii& H) { std::cout << H.size(); for (const Pii& p : H) std::cout << " " << p; std::cout << "\n"; return; }
ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pii& d1, const Pii& d2, const Pii& d3) { return sign(cross(d1, d2, d3)); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d2); }
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
		if (on_seg_weak(H[i], H[j], H[k0])) return 0;
		if (on_seg_weak(H[i], H[j], H[k1])) return 0;
	}
	return 1;
}
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos operator ~ () const { return { -y, x }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
Polygon P[LEN]; int t;
Polygon TRI[20]; int tr;
Pos cnv(const Pii& p) { return Pos(p.x, p.y); }
void print(const Polygon& H) { std::cout << H.size(); for (const Pos& p : H) std::cout << " " << p; std::cout << "\n"; return; }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
ld area(const Polygon& H) {
	ld a = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) a += H[i] / H[(i + 1) % sz];
	return a * .5;
}
Polygon box(const ld& x0, const ld& y0, const ld& x1, const ld& y1) { return { Pos(x0, y0), Pos(x1, y0), Pos(x1, y1), Pos(x0, y1) }; }
void triangulation(const Vpii& H) {
	int sz = H.size(); assert(sz >= 3);
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
				triangulation(H1);
				triangulation(H2);
				return;
			}
		}
	}
	return;
}
void rect_to_rect(const int& i, const ld& x, const ld& y, const ld& l, Vint& id, const ld& yy = 0) {
	if (eq(x, l)) { id.push_back(i); return; }
	if (sign(x - l * 2) >= 0 || sign(l - x * 2) >= 0) {
		Polygon R0, R1, R2, R2_;
		ld nx, ny;
		if (sign(x - l * 2) >= 0) {
			nx = x * .5; ny = y * 2;
			R0 = box(0, 0, nx, ny); R1 = box(0, 0, nx, y); R2 = box(nx, 0, x, y);
			R2_ = box(0, y, nx, ny);
		}
		else if (sign(l - x * 2) >= 0) {
			nx = x * 2; ny = y * .5;
			R0 = box(0, 0, nx, ny); R1 = box(0, 0, x, ny), R2 = box(0, ny, x, y);
			R2_ = box(x, 0, nx, ny);
		}
		if (!zero(yy)) {
			for (Pos& p : R0) p.y += yy;
			for (Pos& p : R1) p.y += yy;
			for (Pos& p : R2) p.y += yy;
			for (Pos& p : R2_) p.y += yy;
		}
		Vint I = { t, t + 1 };
		cut(i, 2); print(R1); print(R2);
		tape(2, I); print(R1); print(R2_); print(R0);
		t += 2;
		P[t++] = R0;
		rect_to_rect(t - 1, nx, ny, l, id, yy);
		return;
	}
	Polygon R0, TZ, TB, TS;
	Pos vb, vs;
	ld h = (x * y) / l, dx, dy;
	dx = std::abs(x - l);
	dy = std::abs(y - h);
	R0 = box(0, 0, l, h);
	if (sign(x - l) > 0) {
		TZ = { O, Pos(l, 0), Pos(l, dy), Pos(dx, y), Pos(0, y) };
		TB = { Pos(dx, y), Pos(x, 0), Pos(x, y) };
		TS = { Pos(l, 0), Pos(x, 0), Pos(l, dy) };
		vb = Pos(-dx, dy);
		vs = Pos(-l, y);
	}
	else {
		TZ = { O, Pos(x, 0), Pos(x, dy), Pos(dx, h), Pos(0, h) };
		TB = { Pos(0, y), Pos(x, dy), Pos(x, y) };
		TS = { Pos(0, h), Pos(dx, h), Pos(0, y) };
		vb = Pos(dx, -dy);
		vs = Pos(x, -h);
	}
	if (!zero(yy)) {
		for (Pos& p : R0) p.y += yy;
		for (Pos& p : TZ) p.y += yy;
		for (Pos& p : TB) p.y += yy;
		for (Pos& p : TS) p.y += yy;
	}
	Vint I = { t, t + 1, t + 2 };
	cut(i, 3); print(TZ); print(TB); print(TS);
	for (Pos& b : TB) b += vb;
	for (Pos& s : TS) s += vs;
	tape(3, I); print(TZ); print(TB); print(TS); print(R0);
	t += 3;
	id.push_back(t);
	P[t++] = R0;
	return;
}
void rect_to_rect(const int& i, const ld& l, Vint& id, const ld& yy = 0) {
	Polygon& H = P[i];
	int sz = H.size();
	assert(sz == 4);
	ld x = H[1].x - H[0].x;
	ld y = H[3].y - H[0].y;
	rect_to_rect(i, x, y, l, id, yy);
	return;
}
void tri_to_rect(const int& tt, Vint& id, int rt = -1) {
	Polygon H = TRI[tt];
	int sz = H.size();
	assert(sz == 3);
	const Pos& p0 = H[0], p1 = H[1], p2 = H[2];
	Pos v = p0 - p1;
	ld y;
	if (~rt) {//rect_to_tri
		assert(P[rt].size() == 4);
		ld l0 = v.mag();
		Vint idx;
		y = P[rt][0].y;
		rect_to_rect(rt, l0, idx, y);
		assert(idx.size() == 1);
		rt = idx[0];
		assert(rt == t - 1);
	}
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
	Vint I = { t, t + 1, t + 2 };
	t += 3;
	id.push_back(t);
	if (~rt) {//rect_to_tri
		for (Pos& p : R0_) p.y += y;
		for (Pos& p : Tl_) p.y += y;
		for (Pos& p : Tr_) p.y += y;
		cut(rt, 3); print(R0_); print(Tl_); print(Tr_);
		tape(3, I); print(R0); print(Tl); print(Tr); print(H);
		P[t++] = H;
	}
	else {//tri_to_rect
		Polygon R = box(0, 0, dd, h);
		cut(tt, 3); print(R0); print(Tl); print(Tr);
		tape(3, I); print(R0_); print(Tl_); print(Tr_); print(R);
		P[t++] = R;
	}
	return;
}
void rect_to_square(const ld& l, Vint& id) {
	Polygon square = box(0, 0, l, l);
	tape(id.size(), id);
	ld y = 0;
	for (const int& i : id) {
		Polygon B = P[i];
		assert(B.size() == 4);
		for (Pos& b : B) b.y += y;
		print(B);
		y = B[2].y;
	}
	print(square);
	P[t++] = square;
	return;
}
void square_split(const Vpii& Tii, Vint& id) {
	tr = 1;
	ld l = sqrt(area(Tii) * .5);
	triangulation(Tii);
	cut(t - 1, tr - 1);
	ld y = 0;
	for (int i = 1; i < tr; i++) {
		ld a = std::abs(area(TRI[i]));
		ld h = a / l;
		Polygon B = box(0, y, l, y + h);
		print(B);
		id.push_back(t);
		P[t++] = B;
		y += h;
	}
	return;
}
void start_to_square(const Vpii& Sii) {
	tr = 1;
	ld l = sqrt(area(Sii) * .5);
	triangulation(Sii);
	//P[0] = S
	for (t = 1; t < tr; t++) P[t] = TRI[t];
	cut(0, tr - 1);
	for (int i = 1; i < t; i++) print(P[i]);
	int t1 = t;
	Vint id, idx;
	for (int i = 1; i < t1; i++) tri_to_rect(i, idx);
	for (const int& i : idx) rect_to_rect(i, l, id);
	rect_to_square(l, id);
	assert(P[t - 1][2] == Pos(l, l));
	return;
}
void square_to_target(const Vpii& Tii) {
	Vint id, idx;
	square_split(Tii, idx);
	int tt = 1;
	for (const int& rt : idx) tri_to_rect(tt++, id, rt);
	tape(id.size(), id);
	for (const int& i : id) print(P[i]);
	print(Tii);
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	std::cin >> N; Vpii Sii(N); for (Pii& p : Sii) std::cin >> p;
	std::cin >> M; Vpii Tii(M); for (Pii& p : Tii) std::cin >> p;
	assert(area(Sii) > 0); assert(area(Tii) > 0); assert(area(Sii) == area(Tii));
	start_to_square(Sii);
	square_to_target(Tii);
	return;
}
int main() { solve(); return 0; }//boj17645
//https://codeforces.com/blog/entry/68748