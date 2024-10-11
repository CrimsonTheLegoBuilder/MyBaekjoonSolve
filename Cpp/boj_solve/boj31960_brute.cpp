#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <deque>
#include <random>
#include <array>
#include <tuple>
#include <complex>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e3;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }
inline ld sq(ld x) { return x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}

#define MID 0
#define LEFT 2
#define RIGHT 1

int N, M, Q;
ld A, D;
Vll T[4];
bool D_OK[4];
ld THE[4][3];
bool cmpvld(const Vld& v1, const Vld& v2) {
	int sz = v1.size();
	if (sz != v2.size()) return 0;
	for (int i = 0; i < sz; i++) if (!zero(v1[i] - v2[i])) return 0;
	return 1;
}
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
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return zero(ret) ? 0 : ret > 0 ? 1 : -1; }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) > 0; }
ld cos_2nd_(const ld& a, const ld& b, const ld& c) {
	ld t = (a * a + b * b - c * c) / (2 * a * b);
	return std::abs(acos(std::min(std::max(t, -1.0), 1.0)));
}
ld cos_2nd(const ll& a, const ll& b, const ll& c) {
	ll num = a + b - c;
	ld den = 2 * sqrt(a) * sqrt(b);
	ld t = num / den;
	return std::abs(acos(std::min(std::max(t, -1.0), 1.0)));
}
ld heron(const ll& a2, const ll& b2, const ll& c2) {
	ld a = sqrt(a2);
	ld b = sqrt(b2);
	ld c = sqrt(c2);
	ld s = (a + b + c) / 2;
	ld area = sqrt(s * (s - a) * (s - b) * (s - c));
	return area;
}
ld heron(const Vll& v) { assert(3 == v.size()); return heron(v[0], v[1], v[2]); }
ld area(const Vll& v) {
	ll a = v[0], b = v[1], c = v[2];
	ld t = cos_2nd(a, b, c);
	return sqrt(a) * sqrt(b) * sin(t);
}
bool _4at1() {
	if (M < 4) return 0;
	ld t0 = 0;
	for (int i = 0; i < 4; i++) t0 += THE[i][MID];
	if (!zero(2 * PI - t0)) return 0;
	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 3; j++) {
			if (j == i) continue;
			for (int k = 1; k <= 3; k++) {
				if (k == i || k == j) continue;
				ll nxtd = T[0][RIGHT];
				ld nxtt = THE[0][RIGHT];
				bool f = 1;
				for (const auto& l : { i, j, k }) {
					if (T[l][LEFT] == nxtd && zero(PI * .5 - (nxtt + THE[l][LEFT]))) {
						nxtd = T[l][RIGHT];
						nxtt = THE[l][RIGHT];
					}
					else if (T[l][RIGHT] == nxtd && zero(PI * .5 - (nxtt + THE[l][RIGHT]))) {
						nxtd = T[l][LEFT];
						nxtt = THE[l][LEFT];
					}
					else { f = 0; break; }
				}
				if (f) {
					bool f1 = nxtd == T[0][LEFT];
					bool f2 = zero(PI * .5 - (nxtt + THE[0][LEFT]));
					assert(f1 && f2);
					return 1;
				}
			}
		}
	}
	return 0;
}
bool half_check(const int& i, const int& j) {
	if (!D_OK[i]) return 0;
	ld a1 = area(T[i]);
	ld a2 = area(T[j]);
	if (!zero(A - (a1 + a2))) return 0;
	Vld T1, T2;
	T2 = { sqrt(T[j][0]), sqrt(T[j][1]), sqrt(T[j][2]) };
	std::sort(T2.begin(), T2.end());
	bool f = 0;
	if (zero(THE[i][LEFT] - PI * .5)) {
		T1.push_back(sqrt(A * 2));
		T1.push_back(D - sqrt(T[i][LEFT]));
		T1.push_back(sqrt(T[i][RIGHT]));
		std::sort(T1.begin(), T1.end());
		if (cmpvld(T1, T2))f = 1;
	}
	if (zero(THE[i][RIGHT] - PI * .5)) {
		T1.push_back(sqrt(A * 2));
		T1.push_back(D - sqrt(T[i][RIGHT]));
		T1.push_back(sqrt(T[i][LEFT]));
		std::sort(T1.begin(), T1.end());
		if (cmpvld(T1, T2))f = 1;
	}
	if (zero(THE[i][LEFT] - PI * .25)) {
		T1.push_back(sqrt(A * 2) - sqrt(T[i][LEFT]));
		T1.push_back(D);
		T1.push_back(sqrt(T[i][RIGHT]));
		std::sort(T1.begin(), T1.end());
		if (cmpvld(T1, T2))f = 1;
	}
	if (zero(THE[i][RIGHT] - PI * .25)) {
		T1.push_back(sqrt(A * 2) - sqrt(T[i][RIGHT]));
		T1.push_back(D);
		T1.push_back(sqrt(T[i][LEFT]));
		std::sort(T1.begin(), T1.end());
		if (cmpvld(T1, T2))f = 1;
	}
	return f;
}
bool compose_triangle(const Vint& vi, Vld& vd) {
	std::sort(vd.begin(), vd.end());
	Vll T1 = T[vi[0]], T2 = T[vi[1]];
	for (int k = 0; k < 2; k++) {
		for (int l = 0; l < 2; l++) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					ld t1 = cos_2nd(T1[0], T1[1], T1[2]);
					ld t2 = cos_2nd(T2[0], T2[1], T2[2]);
					if (!zero(PI - (t1 + t2))) continue;
					for (int m = 0; m < 2; m++) {
						if (T1[m & 1] == T2[m ^ 1]) {
							Vld v;
							v.push_back(sqrt(T1[2]));
							v.push_back(sqrt(T2[2]));
							v.push_back(sqrt(T1[m ^ 1] + sqrt(T2[m & 1])));
							std::sort(v.begin(), v.end());
							bool f = 1;
							for (int n = 0; n < 3; n++) if (!zero(vd[i] - v[i])) f = 0;
							if (f) return 1;
						}
					}
				}
			}
			std::reverse(T2.begin(), T2.end());
		}
		std::reverse(T1.begin(), T1.end());
	}
	return 0;
}
bool _2and2() {
	if (M < 2) return 0;
	for (int i = 1; i <= 3; i++) {
		if (half_check(0, i) || half_check(i, 0)) {
			for (int j = 1; j <= 3; j++) {
				if (j == i) continue;
				for (int k = 1; k <= 3; k++) {
					if (k == i || k == j) continue;
					if (half_check(j, k)) return 1;
				}
			}
		}
	}
	return 0;
}
bool half_check(const Vint& v) {
	for (int i = 0; i < 3; i++) {
		if (D_OK[i]) {
			Vint vi;
			Vld vd;
			for (int j = 0; j < 3; j++) if (j != i) vi.push_back(j);
			for (int j = 1; j <= 2; j++) {
				if (zero(PI * .5 - THE[i][j]) && sign(T[i][j] - A) > 0) {
					vd.clear();
					vd.push_back(sqrt(A * 2));
					vd.push_back(sqrt(T[i][j == LEFT ? RIGHT : LEFT]));
					vd.push_back(D - sqrt(T[i][j]));
					if (compose_triangle(vi, vd)) return 1;
				}
				if (zero(PI * .25 - THE[i][j]) && sign(T[i][j] - A) > 0) {
					vd.clear();
					vd.push_back(D);
					vd.push_back(sqrt(T[i][j == LEFT ? RIGHT : LEFT]));
					vd.push_back(sqrt(A * 2) - sqrt(T[i][j]));
					if (compose_triangle(vi, vd)) return 1;
				}
			}
		}
		for (int j = 0; j < 3; j++) {
			if (zero(T[v[i]][j] - (A * 2))) {
				ld tl = cos_2nd(T[v[i]][j], T[v[i]][(j + 2) % 3], T[v[i]][(j + 1) % 3]);
				ld tr = cos_2nd(T[v[i]][j], T[v[i]][(j + 1) % 3], T[v[i]][(j + 2) % 3]);
				ld tm = cos_2nd(T[v[i]][(j + 2) % 3], T[v[i]][(j + 1) % 3], T[v[i]][j]);
				ll dl = T[v[i]][(j + 2) % 3];
				ll dr = T[v[i]][(j + 1) % 3];
				if (sign(tl - PI * .25) > 0 || sign(tr - PI * .25) > 0) return 0;
				if (sign(tl - PI * .25) < 0 && sign(tr - PI * .25) < 0) {
					int i1 = v[(i + 1) % 3], i2 = v[(i + 2) % 3];
					if (D_OK[i1] && D_OK[i2] && zero(PI * 2 - (tm + THE[i1][MID] + THE[i2][MID]))) {
						for (int _ = 0; _ < 2; _++) {
							ld d3 = -1, d4 = -1, t3 = -1, t4 = -1;
							if (T[i1][LEFT] == dl && zero(PI * .25 - (tl + THE[i1][LEFT]))) {
								d3 = T[i1][RIGHT];
								t3 = THE[i1][RIGHT];
							}
							else if (T[i1][RIGHT] == dl && zero(PI * .25 - (tl + THE[i1][RIGHT]))) {
								d3 = T[i1][LEFT];
								t3 = THE[i1][LEFT];
							}
							if (T[i2][LEFT] == dl && zero(PI * .25 - (tl + THE[i2][LEFT]))) {
								d4 = T[i2][RIGHT];
								t4 = THE[i2][RIGHT];
							}
							else if (T[i2][RIGHT] == dl && zero(PI * .25 - (tl + THE[i2][RIGHT]))) {
								d4 = T[i2][LEFT];
								t4 = THE[i2][LEFT];
							}
							if (d3 > 0 && d4 > 0 && d3 == d4 && zero(PI * .5 - (t3 + t4))) return 1;
							std::swap(i1, i2);
						}
					}
				}
			}
		}
	}
	return 0;
}
bool _3and1() {
	for (int i = 0; i < 4; i++) {
		if ((zero(T[i][LEFT] - D) && zero(THE[i][LEFT] - PI * .5)) ||
			(zero(T[i][RIGHT] - D) && zero(THE[i][RIGHT] - PI * .5))) {
			Vint v;
			for (int j = 0; j < 4; j++) if (j != i) v.push_back(j);
			if (half_check(v)) return 1;
		}
	}
	return 0;
}
bool two_tri_check(const Vint& vi, Vld& T1, Vld& T2) {
	assert(2 == vi.size());
	Vld T3, T4;
	for (int i = 0; i < 3; i++) {
		T3.push_back(sqrt(T[vi[0]][i]));
		T4.push_back(sqrt(T[vi[1]][i]));
	}
	std::sort(T3.begin(), T3.end());
	std::sort(T4.begin(), T4.end());
	if (cmpvld(T1, T3) && cmpvld(T2, T4)) return 1;
	if (cmpvld(T1, T4) && cmpvld(T2, T3)) return 1;
	return 0;
}
bool trap_check(const int& i, const int& j, Polygon& B) {
	Pos p = B[3];
	Vint vi;
	for (int k = 0; k < 4; k++) if (k != i && k != j) vi.push_back(k);
	if (on_seg_weak(Pos(0, D), Pos(D, D), p)) {
		Vld T1, T2;
		T1.push_back((B[2] - B[3]).mag());
		T1.push_back(B[2].x - B[3].x);
		T1.push_back(B[3].y - B[2].y);
		T2.push_back((B[0] - B[3]).mag());
		T2.push_back(B[3].x - B[0].x);
		T2.push_back(B[3].y - B[0].y);
		std::sort(T1.begin(), T1.end());
		std::sort(T2.begin(), T2.end());
		if (two_tri_check(vi, T1, T2)) return 1;
	}
	int i1 = vi[0], i2 = vi[1];

	if (p == Pos(0, D)) {
		Vld T1 = { D, (B[3] - B[2]).mag(), B[3].y - B[2].y }, T2;
		std::sort(T1.begin(), T1.end());
		if (!D_OK[i1] && !D_OK[i2]) return 0;
		if (!D_OK[i1] && D_OK[i2]) std::swap(i1, i2);
		for (int k = 0; k < 3; k++) {
			ld tl = cos_2nd(T[i2][k], T[i2][(k + 2) % 3], T[i2][(k + 1) % 3]);
			ld tr = cos_2nd(T[i2][k], T[i2][(k + 1) % 3], T[i2][(k + 2) % 3]);
			if (T[i2][k] == T[i1][LEFT]) {
				if (zero(PI - (T[i1][MID] + tl))) {
					T2 = { D,
						sqrt(T[i2][(k + 1) % 3]),
						sqrt(T[i1][RIGHT]) + sqrt(T[i2][(k + 2) % 3])
					};
					std::sort(T2.begin(), T2.end());
					if (cmpvld(T1, T2)) return 1;
				}
				if (zero(PI - (T[i1][MID] + tr))) {
					T2 = { D,
						sqrt(T[i2][(k + 2) % 3]),
						sqrt(T[i1][RIGHT]) + sqrt(T[i2][(k + 1) % 3])
					};
					std::sort(T2.begin(), T2.end());
					if (cmpvld(T1, T2)) return 1;
				}
			}
			if (T[i2][k] == T[i1][RIGHT]) {
				if (zero(PI - (T[i1][MID] + tl))) {
					T2 = { D,
						sqrt(T[i2][(k + 1) % 3]),
						sqrt(T[i1][LEFT]) + sqrt(T[i2][(k + 2) % 3])
					};
					std::sort(T2.begin(), T2.end());
					if (cmpvld(T1, T2)) return 1;
				}
				if (zero(PI - (T[i1][MID] + tr))) {
					T2 = { D,
						sqrt(T[i2][(k + 2) % 3]),
						sqrt(T[i1][LEFT]) + sqrt(T[i2][(k + 1) % 3])
					};
					std::sort(T2.begin(), T2.end());
					if (cmpvld(T1, T2)) return 1;
				}
			}
		}
		return 0;
	}

	Polygon Z;
	if (on_seg_weak(Pos(0, 0), Pos(0, D), p))
		Z = { B[3], B[2], Pos(D, D), Pos(0, D) };
	
	else if (on_seg_weak(Pos(D, 0), Pos(D, D), p))
		Z = { B[0], B[3], Pos(D, D), Pos(0, D) };
	
	Vld T1;
	Vld T2 = { sqrt(T[i2][0]), sqrt(T[i2][1]), sqrt(T[i2][2]) };
	for (int k = 0; k < 4; k++) {
		const Pos& pre = Z[k], cur = Z[(k + 1) % 4], nxt = Z[(k + 2) % 4];
		ld t1 = rad(nxt - cur, pre - cur);
		ld d1 = (cur - pre).mag();
		ld d2 = (cur - nxt).mag();
		std::sort(T2.begin(), T2.end());
		for (int l = 0; l < 3; l++) {
			for (int m = 1; m <= 2; m++) {
				if (zero(sqrt(T[i1][l]) - d1) && zero(sqrt(T[i1][(l + m) % 3]) - d2)) {
					ld t = cos_2nd(T[i1][l], T[i1][(l + m) % 3], T[i1][(l + (m == LEFT ? RIGHT : LEFT)) % 3]);
					if (zero(t - t1)) {
						T1 = { (Z[(k + 2) % 4] - Z[(k + 3) % 4]).mag(), (Z[(k + 3) % 4] - Z[(k + 4) % 4]).mag() };
						T1.push_back(sqrt(T[i1][(l + (m == LEFT ? RIGHT : LEFT)) % 3]));
						std::sort(T1.begin(), T1.end());
						if (cmpvld(T1, T2)) return 1;
					}
				}
			}
		}
	}
	return 0;
}
bool stack_up() {
	for (int i = 0; i < 4; i++) {
		if (!D_OK[i]) continue;
		if (!zero(PI * .5 - THE[i][LEFT]) && !zero(PI * .5 - THE[i][RIGHT])) continue;
		Polygon B = { Pos(0, 0), Pos(D, 0) };
		if (zero(PI * .5 - THE[i][LEFT])) B.push_back(Pos(D, sqrt(T[i][LEFT])));
		if (zero(PI * .5 - THE[i][RIGHT])) B.push_back(Pos(D, sqrt(T[i][RIGHT])));
		ld nxtd = (B[0] - B[2]).mag();
		ld nxtt = rad(B[0] - B[2], B[1] - B[2]);
		ld pvt = rad(B[1] - B[0], B[2] - B[0]);
		for (int j = 0; j < 4; j++) {
			if (j == i) continue;
			for (int k = 0; k < 3; k++) {
				Pos nxt;
				if (zero(nxtd - sqrt(T[j][k]))) {
					ld tl = cos_2nd(T[j][k], T[j][(k + 2) % 3], T[j][(k + 1) % 3]);
					ld dl = sqrt(T[j][(k + 2) % 3]);
					nxt = Pos(dl, 0).rot(tl).rot(pvt);
					B.push_back(nxt);
					if (trap_check(i, j, B)) return 1;
					B.pop_back();
					ld tr = cos_2nd(T[j][k], T[j][(k + 1) % 3], T[j][(k + 2) % 3]);
					ld dr = sqrt(T[j][(k + 1) % 3]);
					nxt = Pos(dr, 0).rot(tr).rot(pvt);
					B.push_back(nxt);
					if (trap_check(i, j, B)) return 1;
					B.pop_back();
				}
			}
		}
	}
	return 0;
}
bool query() {
	A = 0;
	for (int i = 0; i < 4; i++) {
		T[i].resize(3);
		std::cin >> T[i][0] >> T[i][1] >> T[i][2];
		A += heron(T[i]);
		//A += area(T[i]);
	}
	D = sqrt(A);
	M = 0;
	for (int i = 0; i < 4; i++) {
		D_OK[i] = 0;
		for (int j = 0; j < 3; j++) {
			if (zero(T[i][j] - A)) {
				M++;
				rotate(T[i].begin(), T[i].begin() + j, T[i].end());
				D_OK[i] = 1;
				ld tl = cos_2nd(T[i][MID], T[i][LEFT], T[i][RIGHT]);
				ld tr = cos_2nd(T[i][MID], T[i][RIGHT], T[i][LEFT]);
				THE[i][LEFT] = tl;
				THE[i][RIGHT] = tr;
				THE[i][MID] = cos_2nd(T[i][LEFT], T[i][RIGHT], T[i][MID]);
				break;
			}
		}
	}
	if (!M) return 0;
	return _4at1() || _2and2() || _3and1() || stack_up();
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> Q;
	while (Q--) std::cout << query() << "\n";
	return;
}
int main() { solve(); return 0; }//boj31960