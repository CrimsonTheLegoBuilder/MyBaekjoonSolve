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
typedef std::vector<ld> Vld;
const ld INF = 1e17;
ld TOL = 1e-15;
const ld PI = acos(-1);
const int LEN = 105;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& u, const ld& v) { return zero(u - v); }
inline bool between(ld s, ld e, const ld& x) { if (e < s) std::swap(s, e); return sign(x - s) > 0 && sign(e - x) > 0; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}
int find_y(const Vld& Y, const ld& y) {
	int s = 0, e = Y.size() - 1, m = 0;
	while (s < e) {
		m = (s + e) >> 1;
		if (sign(y - Y[m]) > 0) s = m + 1;
		else e = m;
	}
	return s;
	//for (int i = 0; i < Y.size(); i++) {
	//	if (i == 0 && y < Y[i]) return i;
	//	if (eq(Y[i], y)) return i;
	//	else if (between(Y[i], Y[i + 1], y)) return i + 1;
	//}
	//return Y.size();
}

int N;
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos rot(ld the) const { return { x * (ld)cosl(the) - y * (ld)sinl(the), x * (ld)sinl(the) + y * (ld)cosl(the) }; }
	ld rad() const { return norm(atan2l(y, x)); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
typedef std::vector<Pos> Polygon;
Polygon R[LEN], L[LEN];
int rt, lt;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
ld area(const Polygon& H) {
	ld a = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) a += H[i] / H[(i + 1) % sz];
	return a;
}
ld largest(Polygon H, const ld& x) {
	Pos s = Pos(x, -1), e = Pos(x, 1);
	ld ret = 0;
	int i, sz = H.size();
	for (i = 0; i < sz; i++) if (eq(H[i].x, x) || between(H[i].x, H[(i + 1) % sz].x, x)) break;
	rt = lt = 0;
	Polygon tmp;
	if (!eq(H[i].x, x)) {
		tmp.push_back(intersection(s, e, H[i], H[(i + 1) % sz]));
		i = (i + 1) % sz;
	}
	else if (eq(H[i].x, x)) {
		if (eq(H[(i + 1) % sz].x, x)) {
			i = (i + 1) % sz;
		}
		tmp.push_back(H[i]);
		i = (i + 1) % sz;
	}
	for (int _ = 0; _ < sz; _++, i = (i + 1) % sz) {
		if (eq(H[i].x, x)) {
			tmp.push_back(H[i]);
			if (tmp[1].x < x) L[lt++] = tmp;
			else if (tmp[1].x > x) R[rt++] = tmp;
			if (eq(H[(i + 1) % sz].x, x)) _++, i = (i + 1) % sz;
			tmp.clear();
			tmp.push_back(H[i]);
		}
		else if (between(H[i].x, H[(i + 1) % sz].x, x)) {
			tmp.push_back(H[i]);
			tmp.push_back(intersection(s, e, H[i], H[(i + 1) % sz]));
			if (tmp[1].x < x) L[lt++] = tmp;
			else if (tmp[1].x > x) R[rt++] = tmp;
			tmp.clear();
			tmp.push_back(intersection(s, e, H[i], H[(i + 1) % sz]));
		}
		else tmp.push_back(H[i]);
	}

	//std::cout << "DEBUG:: \n";
	//std::cout << "x:: " << x << "\n";
	//std::cout << "L:: \n";
	//for (int j = 0; j < lt; j++) {
	//	std::cout << "L[" << j << "]\n";
	//	for (const Pos& p : L[j]) std::cout << " (" << p.x << ", " << p.y << "),\n";
	//}
	//for (int j = 0; j < rt; j++) {
	//	std::cout << "R[" << j << "]\n";
	//	for (const Pos& p : R[j]) std::cout << " (" << p.x << ", " << p.y << "),\n";
	//}
	//std::cout << "DEBUG:: \n";

	Vld F, A, Y;
	Vint I;
	I.resize(lt);
	for (int j = 0; j < lt; j++) I[j] = j;
	std::sort(I.begin(), I.end(), [&](const int& p, const int& q) -> bool {
		const Polygon& P = L[p];
		const Polygon& Q = L[q];
		ld py = P[0].y < P.back().y ? P[0].y : P.back().y;
		ld qy = Q[0].y < Q.back().y ? Q[0].y : Q.back().y;
		return py < qy;
		});
	A.resize(lt, 0);
	for (int j = 0; j < lt; j++) {
		i = I[j];
		A[i] = area(L[i]);
		Y.push_back(L[i][0].y);
	}
	std::sort(Y.begin(), Y.end());
	for (int j = 0; j < lt; j++) {
		i = I[j];
		if (A[i] < 0) continue;
		ld h = L[i][0].y;
		ld a = A[i];
		int yi = find_y(Y, L[i].back().y);
		int k = j + 1;
		while (1) {
			//std::cout << "FUCK:: L " << lt << " " << j << " " << k << " " << h << " " << y << "\n";
			//std::cout << "FUCK:: L \n";
			ld y = Y[yi];
			//if (sign(y - h) > 0) { F.push_back(a); break; }
			if (eq(y, h)) { F.push_back(a); break; }
			while (k < lt && !eq(y, L[I[k]][0].y)) k++;
			if (k >= I.size()) { F.push_back(a); break; }
			if (A[I[k]] > 0) { F.push_back(a); break; }
			if (A[I[k]] < 0) a += A[I[k]];
			yi = find_y(Y, L[I[k]].back().y);
			if (A[I[k]] < 0 && eq(Y[yi], y)) yi++;
		}
	}
	Y.clear();
	I.resize(rt);
	for (int j = 0; j < rt; j++) I[j] = j;
	std::sort(I.begin(), I.end(), [&](const int& p, const int& q) -> bool {
		const Polygon& P = R[p];
		const Polygon& Q = R[q];
		ld py = P[0].y < P.back().y ? P[0].y : P.back().y;
		ld qy = Q[0].y < Q.back().y ? Q[0].y : Q.back().y;
		return py < qy;
		});
	A.resize(rt, 0);
	for (int j = 0; j < rt; j++) {
		i = I[j];
		A[i] = area(R[i]);
		Y.push_back(R[i].back().y);
	}
	std::sort(Y.begin(), Y.end());
	for (int j = 0; j < rt; j++) {
		i = I[j];
		if (A[i] < 0) continue;
		ld a = A[i];
		ld h = R[i].back().y;
		int yi = find_y(Y, R[i][0].y);
		int k = j + 1;
		while (1) {
			//std::cout << "FUCK:: R " << lt << " " << j << " " << k << " " << h << " " << y << "\n";
			//std::cout << "FUCK:: R \n";
			ld y = Y[yi];
			//if (sign(y - h) > 0) { F.push_back(a); break; }
			if (eq(y, h)) { F.push_back(a); break; }
			while (k < rt && !eq(y, R[I[k]].back().y)) k++;
			if (k >= I.size()) { F.push_back(a); break; }
			if (A[I[k]] > 0) { F.push_back(a); break; }
			if (A[I[k]] < 0) a += A[I[k]];
			yi = find_y(Y, R[I[k]][0].y);
			if (A[I[k]] < 0 && eq(Y[yi], y)) yi++;
		}
	}
	std::sort(F.rbegin(), F.rend());
	return F[0];
}
ld ternary_search(const Polygon& H) {
	assert(H.size() >= 3);
	ld s = INF, e = -INF;
	ld sy = INF, ey = -INF;
	for (const Pos& p : H) {
		s = std::min(s, p.x), e = std::max(e, p.x);
		sy = std::min(s, p.x), ey = std::max(e, p.x);
	}
	ld limit = std::max(std::abs(s - e), std::abs(sy - ey));
	if (limit < 10 || H.size() < 10) TOL = 1e-7;
	else TOL = 1e-15;
	s += TOL;
	e -= TOL;
	ld a1 = 0, a2 = 0;
	while (std::abs(e - s) > 1e-8) {
		ld m1 = (s + s + e) / 3;
		ld m2 = (s + e + e) / 3;
		a1 = largest(H, m1);
		a2 = largest(H, m2);
		if (a1 > a2) s = m1;
		else e = m2;
	}
	return largest(H, (s + e) * .5);
}
Vld ANS;
bool query() {
	std::cin >> N;
	if (!N) return 0;
	Pos v;
	std::cin >> v;
	ld t = v.rad();
	t = norm(PI * .5 - t);
	Polygon H(N);
	for (Pos& p : H) std::cin >> p, p = p.rot(t);
	ANS.push_back(ternary_search(H) * .5);
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	while (query());
	for (const ld& a : ANS) std::cout << a << "\n";
	return;
}
int main() { solve(); return 0; }//boj13816 Cave Explorer
