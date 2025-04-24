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
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-6;
const ld PI = acos(-1);
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
bool cmpvld(const Vld& v1, const Vld& v2) {
	int sz = v1.size();
	if (sz != v2.size()) return 0;
	for (int i = 0; i < sz; i++) if (!zero(v1[i] - v2[i])) return 0;
	return 1;
}

//#define DEBUG
#define WHAT_THE_FUCK

#define MID 0
#define LEFT 2
#define RIGHT 1

int N, M, Q;
ld A, D;
Vll T[4];
bool D_OK[4];
ld THE[4][3];
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos rot(ld the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2(p1 / p2, p1 * p2); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return zero(ret) ? 0 : ret > 0 ? 1 : -1; }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) > 0; }
ld cos_2nd(const ll& a, const ll& b, const ll& c) {
	ll num = a + b - c;
	ld den = 2 * sqrt(a) * sqrt(b);
	ld t = num / den;
	return std::abs(acos(std::min(std::max(t, -(ld)1.0), (ld)1.0)));
}
ld heron(const ll& a2, const ll& b2, const ll& c2) {
	ld a = sqrt(a2), b = sqrt(b2), c = sqrt(c2);
	ld s = (a + b + c) / 2;
	ld ret = sqrt(s * (s - a) * (s - b) * (s - c));
	return ret;
}
ld heron(const Vll& v) { assert(3 == v.size()); return heron(v[0], v[1], v[2]); }
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
	ld a1 = heron(T[i]);
	ld a2 = heron(T[j]);
	if (!zero(D / sqrt(2) - sqrt(a1 + a2))) return 0;
	Vld T1 = { sqrt(T[j][0]), sqrt(T[j][1]), sqrt(T[j][2]) };
	std::sort(T1.begin(), T1.end());
	for (int k = 1; k <= 2; k++) {
		Vld T2;
		if (zero(THE[i][k] - PI * .5)) {
			T2.push_back(sqrt(A * 2));
			T2.push_back(D - sqrt(T[i][k]));
			T2.push_back(sqrt(T[i][k == LEFT ? RIGHT : LEFT]));
		}
		else if (zero(THE[i][k] - PI * .25)) {
			T2.push_back(D);
			T2.push_back(sqrt(A * 2) - sqrt(T[i][k]));
			T2.push_back(sqrt(T[i][k == LEFT ? RIGHT : LEFT]));
		}
		if (T2.empty()) continue;
		std::sort(T2.begin(), T2.end());
		if (cmpvld(T1, T2)) return 1;
	}
	return 0;
}
bool _2and2() {
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
bool compose_triangle(const Vint& vi, Vld& vd) {
	Vll T1 = T[vi[0]], T2 = T[vi[1]];
	for (int k = 0; k < 2; k++) {
		for (int l = 0; l < 2; l++) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					ld t1 = cos_2nd(T1[i], T1[(i + 1) % 3], T1[(i + 2) % 3]);
					ld t2 = cos_2nd(T2[j], T2[(j + 1) % 3], T2[(j + 2) % 3]);
					if (!zero(PI - (t1 + t2))) continue;
					for (int m = 0; m < 2; m++) {
						if (T1[(i + m) % 3] == T2[(j + m) % 3]) {
							Vld v;
							v.push_back(sqrt(T1[(i + 2) % 3]));
							v.push_back(sqrt(T2[(j + 2) % 3]));
							v.push_back(sqrt(T1[(i + (m + 1) % 2) % 3]) + sqrt(T2[(j + (m + 1) % 2) % 3]));
							std::sort(v.begin(), v.end());
							if (cmpvld(vd, v)) return 1;
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
bool _2in2() {
	for (int i = 0; i < 4; i++) {
		if (!D_OK[i]) continue;
		for (int j = 0; j < 4; j++) {
			if (j == i) continue;
			if (!D_OK[j]) continue;
			for (int k = 1; k <= 2; k++) {
				for (int l = 1; l <= 2; l++) {
					if (zero(PI * .5 - THE[i][k]) && zero(PI * .5 - THE[j][l])) {
						if (!zero(D - (sqrt(T[i][k]) + sqrt(T[j][l])))) continue;
						ld d1 = sqrt(T[i][k == LEFT ? RIGHT : LEFT]);
						ld d2 = sqrt(T[j][l == LEFT ? RIGHT : LEFT]);
						Vld vd = { D, d1, d2 };
						std::sort(vd.begin(), vd.end());
						Vint vi;
						for (int q = 0; q < 4; q++) if (q != i && q != j) vi.push_back(q);
						if (compose_triangle(vi, vd)) return 1;
					}
				}
			}
		}
	}
	return 0;
}
bool half_check_3(const Vint& v) {
	assert(3 == v.size());
	for (int i = 0; i < 3; i++) {
		Vint vi;
		for (int j = 0; j < 3; j++) if (j != i) vi.push_back(v[j]);
		if (D_OK[v[i]]) {
			Vld vd;
			for (int j = 1; j <= 2; j++) {
				if (zero(PI * .5 - THE[v[i]][j]) && sign(D - sqrt(T[v[i]][j])) > 0) {
					vd.clear();
					vd.push_back(sqrt(A * 2));
					vd.push_back(sqrt(T[v[i]][j == LEFT ? RIGHT : LEFT]));
					vd.push_back(D - sqrt(T[v[i]][j]));
					std::sort(vd.begin(), vd.end());
					if (compose_triangle(vi, vd)) return 1;
				}
				if (zero(PI * .25 - THE[v[i]][j]) && sign(sqrt(A * 2) - sqrt(T[v[i]][j])) > 0) {
					vd.clear();
					vd.push_back(D);
					vd.push_back(sqrt(T[v[i]][j == LEFT ? RIGHT : LEFT]));
					vd.push_back(sqrt(A * 2) - sqrt(T[v[i]][j]));
					std::sort(vd.begin(), vd.end());
					if (compose_triangle(vi, vd)) return 1;
				}
			}
		}
		for (int j = 0; j < 3; j++) {
			if (zero(sqrt(T[v[i]][j]) - sqrt(A * 2))) {
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
							if (T[i2][LEFT] == dr && zero(PI * .25 - (tr + THE[i2][LEFT]))) {
								d4 = T[i2][RIGHT];
								t4 = THE[i2][RIGHT];
							}
							else if (T[i2][RIGHT] == dr && zero(PI * .25 - (tr + THE[i2][RIGHT]))) {
								d4 = T[i2][LEFT];
								t4 = THE[i2][LEFT];
							}
							if (d3 > -1 && d4 > -1 && d3 == d4 && zero(PI * .5 - (t3 + t4))) return 1;
							std::swap(i1, i2);
						}
					}
				}
				Vld vd;
				vd.push_back(D);
				if (sign(tl - PI * .25) == 0) {
					vd.push_back(D - sqrt(dl));
					vd.push_back(sqrt(dr));
				}
				else if (sign(tr - PI * .25) == 0) {
					vd.push_back(D - sqrt(dr));
					vd.push_back(sqrt(dl));
				}
				std::sort(vd.begin(), vd.end());
				if (vd.size() == 3 && compose_triangle(vi, vd)) return 1;
			}
		}
	}
	return 0;
}
bool _3and1() {
	for (int i = 0; i < 4; i++) {
		if ((zero(D - sqrt(T[i][LEFT])) && zero(THE[i][LEFT] - PI * .5)) ||
			(zero(D - sqrt(T[i][RIGHT])) && zero(THE[i][RIGHT] - PI * .5))) {
			Vint v;
			for (int j = 0; j < 4; j++) if (j != i) v.push_back(j);
			if (half_check_3(v)) return 1;
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
	if (j == -1) {
		ld t0 = rad(B[1] - B[0], B[2] - B[0]);
		ld t1 = rad(B[0] - B[2], B[1] - B[2]);
		ld L = (B[2] - B[0]).mag();
		ld l1 = D - B[2].y;
		Vint vi;
		for (int k = 0; k < 4; k++) if (k != i) vi.push_back(k);
		for (const int& k : vi) {
			if (!D_OK[k]) continue;
			for (int l = 1; l <= 2; l++) {
				if (!zero(PI * .5 - (t0 + THE[k][l]))) continue;
				ld nxtt = THE[k][l == LEFT ? RIGHT : LEFT];
				ld nxtd = sqrt(T[k][l == LEFT ? RIGHT : LEFT]);
				ld l2 = L - sqrt(T[k][l]);
				for (const int& m : vi) {
					if (m == k) continue;
					if (!D_OK[m]) continue;
					for (int n = 1; n <= 2; n++) {
						if (!zero(PI * .5 - (nxtt + THE[m][n]))) continue;
						if (!zero(nxtd - sqrt(T[m][n]))) continue;
						ld l3 = sqrt(T[m][n == LEFT ? RIGHT : LEFT]);
						Vld T1 = { l1, l2, l3 };
						std::sort(T1.begin(), T1.end());
						for (const int& q : vi) {
							if (q == m || q == k) continue;
							Vld T2 = { sqrt(T[q][0]), sqrt(T[q][1]), sqrt(T[q][2]) };
							std::sort(T2.begin(), T2.end());
							if (cmpvld(T1, T2)) return 1;
						}
					}
				}
			}
		}
		return 0;
	}
	Pos p = B[3];
	Vint vi;
	for (int k = 0; k < 4; k++) if (k != i && k != j) vi.push_back(k);
	if (on_seg_weak(Pos(0, D), Pos(D, D), p)) {
		Vld T1 = { (B[2] - B[3]).mag(), B[2].x - B[3].x, B[3].y - B[2].y };
		Vld T2 = { (B[0] - B[3]).mag(), B[3].x - B[0].x, B[3].y - B[0].y };
		std::sort(T1.begin(), T1.end());
		std::sort(T2.begin(), T2.end());
		if (two_tri_check(vi, T1, T2)) return 1;
	}
	if (p == Pos(0, D)) {
		Vld T1 = { D, (B[3] - B[2]).mag(), B[3].y - B[2].y };
		std::sort(T1.begin(), T1.end());
		if (compose_triangle(vi, T1)) return 1;
		return 0;
	}
	Polygon Z;
	if (on_seg_weak(Pos(0, 0), Pos(0, D), p))
		Z = { B[3], B[2], Pos(D, D), Pos(0, D) };
	else if (on_seg_weak(Pos(D, 0), Pos(D, D), p))
		Z = { B[0], B[3], Pos(D, D), Pos(0, D) };
	else return 0;
	int i1 = vi[0], i2 = vi[1];
	Vld T1, T2 = { sqrt(T[i2][0]), sqrt(T[i2][1]), sqrt(T[i2][2]) };
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
		assert(3 == B.size());
		if (sign(B[2].y - D) >= 0) return 0;
		ld nxtd = (B[0] - B[2]).mag();
		ld pvt = rad(B[1] - B[0], B[2] - B[0]);
		if (trap_check(i, -1, B)) return 1;
		for (int j = 0; j < 4; j++) {
			if (j == i) continue;
			for (int k = 0; k < 3; k++) {
				if (!zero(nxtd - sqrt(T[j][k]))) continue;
				for (int l = 1; l <= 2; l++) {
					ld t = cos_2nd(T[j][k], T[j][(k + l) % 3], T[j][(k + (l == LEFT ? RIGHT : LEFT)) % 3]);
					ld d = sqrt(T[j][(k + l) % 3]);
					Pos nxt = Pos(d, 0).rot(t).rot(pvt);
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
	}
	D = sqrt(A);
	M = 0;
	for (int i = 0; i < 4; i++) {
		D_OK[i] = 0;
		for (int j = 0; j < 3; j++) {
			if (zero(sqrt(T[i][j]) - D)) {
				M++;
				rotate(T[i].begin(), T[i].begin() + j, T[i].end());
				D_OK[i] = 1;
				THE[i][LEFT] = cos_2nd(T[i][MID], T[i][LEFT], T[i][RIGHT]);
				THE[i][RIGHT] = cos_2nd(T[i][MID], T[i][RIGHT], T[i][LEFT]);
				THE[i][MID] = cos_2nd(T[i][LEFT], T[i][RIGHT], T[i][MID]);
				break;
			}
		}
	}
#ifdef DEBUG
	std::cout << "DEBUG::\n";
	std::cout << "A:: " << A << "\n";
	std::cout << "D:: " << D << "\n";
	std::cout << "L:: " << D * sqrt(2) << "\n";
	std::cout << "T1: " << sqrt(T[0][0]) << " " << sqrt(T[0][1]) << " " << sqrt(T[0][2]) << "\n";
	std::cout << "T2: " << sqrt(T[1][0]) << " " << sqrt(T[1][1]) << " " << sqrt(T[1][2]) << "\n";
	std::cout << "T3: " << sqrt(T[2][0]) << " " << sqrt(T[2][1]) << " " << sqrt(T[2][2]) << "\n";
	std::cout << "T4: " << sqrt(T[3][0]) << " " << sqrt(T[3][1]) << " " << sqrt(T[3][2]) << "\n";
	std::cout << "DEBUG::\n";
#endif
	if (!M) return 0;
	return _4at1() || _2and2() || _2in2() || _3and1() || stack_up();
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> Q;
	while (Q--) std::cout << query() << "\n";
	return;
}
int main() { solve(); return 0; }//boj31960 Square of Triangles


/*

1
5524800 4475088 9999888
55248 4475088 4530336
9999888 5580048 5524800
55248 4530336 5580048
1

1
9999936 4999968 4999968
902772 5069412 6944400
4999968 902772 5069412
3611088 4999968 277776
1

2
10000000 5078125 3828125
2031250 78125 1953125
703125 5078125 2031250
5000000 10000000 5000000
5000000 10000000 5000000
5000000 2890625 390625
6250000 1250000 10000000
1250000 2890625 3515625
1
1

1
6923007 4319483 4852022
2130156 4319483 769223
9999899 3076892 6923007
6923007 2899379 4852022
0

1
9999916 1689175 5743195
5743195 878371 4999958
9999916 4999958 4999958
1689175 878371 4999958
1

1
4900000 6500000 10000000
2000000 10000000 8000000
900000 6500000 8000000
10000000 2000000 8000000
1

*/