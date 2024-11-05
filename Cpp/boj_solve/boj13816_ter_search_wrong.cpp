#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef long double ld;
//typedef double ld;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
typedef std::vector<bool> Vbool;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 105;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& u, const ld& v) { return zero(u - v); }
inline bool between(ld s, ld e, const ld& x) { if (e < s) std::swap(s, e); return sign(x - s) > 0 && sign(e - x) > 0; }
inline ld sq(ld x) { return (ld)x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}
int find_y(const Vld& Y, const ld& y) {
	//int s = 0, e = Y.size() - 1, m = 0;
	//while (s < e) {
	//	m = (s + e) >> 1;
	//	if (sign(y - Y[m]) > 0) s = m + 1;
	//	else e = m;
	//}
	//return s;
	for (int i = 0; i < Y.size(); i++) {
		if (i == 0 && y < Y[i]) return i;
		if (eq(Y[i], y)) return i;
		else if (between(Y[i], Y[i + 1], y)) return i + 1;
	}
	return Y.size();
}

#define RIGHT 1
#define LEFT 2

int N;
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
	Pos rot(ld the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return norm(atan2(y, x)); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
Polygon R[LEN], L[LEN];
int rt, lt;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d2, d3)) > 0; }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
ld green(const Pos& s, const Pos& e) { Pos m = (s + e) * .5; return m.y * (s.x - e.x); }
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
	bool f = 1;
	int rl = LEFT;
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
		if (between(H[i].x, H[(i + 1) % sz].x, x)) {
			tmp.push_back(H[i]);
			tmp.push_back(intersection(s, e, H[i], H[(i + 1) % sz]));
			if (tmp[1].x < x) L[lt++] = tmp;
			else if (tmp[1].x > x) R[rt++] = tmp;
			tmp.clear();
			tmp.push_back(intersection(s, e, H[i], H[(i + 1) % sz]));
		}
		else if (eq(H[i].x, x)) {
			tmp.push_back(H[i]);
			if (tmp[1].x < x) L[lt++] = tmp;
			else if (tmp[1].x > x) R[rt++] = tmp;
			if (eq(H[(i + 1) % sz].x, x)) _++, i = (i + 1) % sz;
			tmp.clear();
			tmp.push_back(H[i]);
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
	//std::cout << "DEBUG:: \n";
	//std::cout << "L:: \n";
	//for (int j = 0; j < lt; j++) {
	//	std::cout << "L[" << I[j] << "]\n";
	//	for (const Pos& p : L[I[j]]) std::cout << " (" << p.x << ", " << p.y << "),\n";
	//}
	//std::cout << "DEBUG:: \n";
	for (int j = 0; j < lt; j++) {
		i = I[j];
		if (A[i] < 0) continue;
		ld h = L[i][0].y;
		ld a = A[i];
		int yi = find_y(Y, L[i].back().y);
		int k = j + 1;
		while (1) {
			ld y = Y[yi];
			//std::cout << "FUCK:: L " << lt << " " << j << " " << k << " " << h << " " << y << " " << Y[yi] << "\n";
			if (eq(y, h)) { F.push_back(a); break; }
			while (k < lt && !eq(y, L[I[k]][0].y)) k++;
			//if (k >= I.size()) k--;
			//assert(A[I[k]] < 0);
			a += A[I[k]];
			yi = find_y(Y, L[I[k]].back().y);
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
			//std::cout << "FUCK:: R \n";
			//if (yi <= 0) { F.push_back(a); break; }
			ld y = Y[yi];
			if (eq(y, h)) { F.push_back(a); break; }
			while (k < rt && !eq(y, R[I[k]].back().y)) k++;
			if (k >= I.size()) k--;
			a += A[I[k]];
			yi = find_y(Y, R[I[k]][0].y);
		}
	}
	std::sort(F.rbegin(), F.rend());
	return F[0];
}
ld ternary_search(const Polygon& H) {
	assert(H.size() >= 3);
	ld s = INF, e = -INF;
	for (const Pos& p : H) s = std::min(s, p.x), e = std::max(e, p.x);
	int cnt = 150; while (cnt--) {
		ld m1 = (s + s + e) / 3;
		ld m2 = (s + e + e) / 3;
		ld a1 = largest(H, m1);
		ld a2 = largest(H, m2);
		//std::cout << "a1a2:: " << a1 << " " << a2 << "\n";
		if (a1 > a2) s = m1;
		else e = m2;
	}
	return largest(H, (s + e) * .5);
} 
bool query() {
	std::cin >> N;
	if (!N) return 0;
	Pos v;
	std::cin >> v;
	if ((zero(v.y) && v.x < 0) || v.y < 0) v *= -1;
	ld t = v.rad();
	t = norm(PI * .5 - t);
	Polygon H(N);
	for (Pos& p : H) std::cin >> p, p = p.rot(t);
	std::cout << ternary_search(H) * .5 << "\n";
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	//freopen("../../../input_data/cave.txt", "r", stdin);
	//freopen("../../../input_data/cave.ret", "w", stdout);
	while (query());
	return;
}
int main() { solve(); return 0; }//boj13816 Cave Explorer

/*
* 
10
1 9998
0 0
1 9999
2 0
3 9999
4 0
5 10000
4 1
3 10000
2 1
1 10000
ans 1.4999749962
ret 2.500075061

*/

//ld largest(Polygon H, const ld& x) {
//	Pos s = Pos(x, -1), e = Pos(x, 1);
//	ld ret = 0;
//	int i, sz = H.size();
//	for (i = 0; i < sz; i++) if (eq(H[i].x, x) || between(H[i].x, H[(i + 1) % sz].x, x)) break;
//	rt = lt = 0;
//	Polygon tmp;
//	bool f = 1;
//	int rl = LEFT;
//	if (!eq(H[i].x, x)) {
//		tmp.push_back(intersection(s, e, H[i], H[(i + 1) % sz]));
//		i = (i + 1) % sz;
//		//rl = ccw(s, e, H[i]) > 0 ? LEFT : RIGHT;
//		rl = H[i].x < 0 ? LEFT : RIGHT;
//	}
//	else if (eq(H[i].x, x)) {
//		if (eq(H[(i + 1) % sz].x, x)) {
//			i = (i + 1) % sz;
//		}
//		tmp.push_back(H[i]);
//		i = (i + 1) % sz;
//		//rl = ccw(s, e, H[i]) > 0 ? LEFT : RIGHT;
//		rl = H[i].x < 0 ? LEFT : RIGHT;
//	}
//	for (int _ = 0; _ < sz; _++, i = (i + 1) % sz) {
//		if (between(H[i].x, H[(i + 1) % sz].x, x)) {
//			tmp.push_back(H[i]);
//			tmp.push_back(intersection(s, e, H[i], H[(i + 1) % sz]));
//			//if (rl == LEFT) L[lt++] = tmp;
//			//else if (rl == RIGHT) R[rt++] = tmp;
//			//std::cout << tmp.size();
//			if (tmp[1].x < x) L[lt++] = tmp;
//			else if (tmp[1].x > x) R[rt++] = tmp;
//			tmp.clear();
//			tmp.push_back(intersection(s, e, H[i], H[(i + 1) % sz]));
//			//rl = ccw(s, e, H[(i + 1) % sz]) > 0 ? LEFT : RIGHT;
//			rl = H[(i + 1) % sz].x < x ? LEFT : RIGHT;
//		}
//		else if (eq(H[i].x, x)) {
//			tmp.push_back(H[i]);
//			//std::cout << tmp.size();
//			//if (rl == LEFT) L[lt++] = tmp;
//			//else if (rl == RIGHT) R[rt++] = tmp;
//			if (tmp[1].x < x) L[lt++] = tmp;
//			else if (tmp[1].x > x) R[rt++] = tmp;
//			if (eq(H[(i + 1) % sz].x, x)) _++, i = (i + 1) % sz;
//			tmp.clear();
//			tmp.push_back(H[i]);
//			//rl = ccw(s, e, H[(i + 1) % sz]) > 0 ? LEFT : RIGHT;
//			rl = H[(i + 1) % sz].x < x ? LEFT : RIGHT;
//		}
//		else tmp.push_back(H[i]);
//	}
//
//	//std::cout << "DEBUG:: \n";
//	//std::cout << "x:: " << x << "\n";
//	//std::cout << "L:: \n";
//	//for (int j = 0; j < lt; j++) {
//	//	std::cout << "L[" << j << "]\n";
//	//	for (const Pos& p : L[j]) std::cout << " (" << p.x << ", " << p.y << "),\n";
//	//}
//	//for (int j = 0; j < rt; j++) {
//	//	std::cout << "R[" << j << "]\n";
//	//	for (const Pos& p : R[j]) std::cout << " (" << p.x << ", " << p.y << "),\n";
//	//}
//	//std::cout << "DEBUG:: \n";
//
//	Vld F, A, Y;
//	Vint I;
//	I.resize(lt);
//	for (int j = 0; j < lt; j++) I[j] = j;
//	std::sort(I.begin(), I.end(), [&](const int& p, const int& q) -> bool {
//		const Polygon& P = L[p];
//		const Polygon& Q = L[q];
//		ld py = P[0].y < P.back().y ? P[0].y : P.back().y;
//		ld qy = Q[0].y < Q.back().y ? Q[0].y : Q.back().y;
//		return py < qy;
//		});
//	//std::cout << "DEBUG:: \n";
//	//std::cout << "L:: \n";
//	//for (int j = 0; j < lt; j++) {
//	//	std::cout << "L[" << I[j] << "]\n";
//	//	for (const Pos& p : L[I[j]]) std::cout << " (" << p.x << ", " << p.y << "),\n";
//	//}
//	//std::cout << "DEBUG:: \n";
//	A.resize(lt, 0);
//	for (int j = 0; j < lt; j++) {
//		i = I[j];
//		A[i] = area(L[i]);
//		Y.push_back(L[i][0].y);
//	}
//	std::sort(Y.begin(), Y.end());
//	//std::cout << "Y::\n";
//	//for (const ld& y : Y) std::cout << y << "\n";
//	//std::cout << "Y::\n";
//	//std::cout << "sz:: " << sz << " lt:: " << lt << " rt:: "<< rt << " Y:: " << Y.size() << " I:: " << I.size() << "\n";
//			//std::cout << "DEBUG::\n";
//	for (int j = 0; j < lt; j++) {
//			//std::cout << "DEBUG::\n";
//		i = I[j];
//		//std::cout << "DEBUG:: A[" << I[j] << "] :: " << A[I[j]] << "\n";
//		if (A[i] < 0) continue;
//		//assert(L[i].back().y <= L[i][0].y);
//		ld h = L[i][0].y;
//		ld a = A[i];
//		int yi = find_y(Y, L[i].back().y);
//		int k = j + 1;
//		while (1) {
//			//std::cout << "yi:: " << yi << "\n";
//			//std::cout << "Y:: " << Y.size() << "\n";
//			if (yi <= 0) { F.push_back(a); break; }
//			ld y = Y[yi];
//			//std::cout << y << " " << h << "\n";
//			if (eq(y, h)) { F.push_back(a); break; }
//			while (k < lt && !eq(y, L[I[k]][0].y)) k++;
//			//std::cout << "k:: " << k << "\n";
//			if (k >= I.size()) k--;
//			//assert(A[I[k]] < 0);
//			a += A[I[k]];
//			yi = find_y(Y, L[I[k]].back().y);
//		}
//	}
//	std::cout << "DEBUG:: \n";
//	std::cout << "R:: \n";
//	for (int j = 0; j < lt; j++) {
//		std::cout << "R[" << I[j] << "]\n";
//		for (const Pos& p : R[I[j]]) std::cout << " (" << p.x << ", " << p.y << "),\n";
//	}
//	std::cout << "DEBUG:: \n";
//	Y.clear();
//	I.resize(rt);
//	for (int j = 0; j < rt; j++) I[j] = j;
//	std::sort(I.begin(), I.end(), [&](const int& p, const int& q) -> bool {
//		const Polygon& P = R[p];
//		const Polygon& Q = R[q];
//		ld py = P[0].y < P.back().y ? P[0].y : P.back().y;
//		ld qy = Q[0].y < Q.back().y ? Q[0].y : Q.back().y;
//		return py < qy;
//		});
//	A.resize(rt, 0);
//	for (int j = 0; j < rt; j++) {
//		i = I[j];
//		A[i] = area(R[i]);
//		Y.push_back(R[i].back().y);
//	}
//	std::sort(Y.begin(), Y.end());
//	//std::cout << "Y::\n";
//	//for (const ld& y : Y) std::cout << y << "\n";
//	//std::cout << "Y::\n";
//	for (int j = 0; j < rt; j++) {
//		i = I[j];
//		//std::cout << "DEBUG:: A[" << I[j] << "] :: " << A[I[j]] << "\n";
//		if (A[i] < 0) continue;
//		//std::cout << "FUCK:: " << R[i].back().y << " " <<  R[i][0].y << "\n";
//		//for (Pos& p : R[i]) std::cout << p << "\n";
//		//assert(R[i].back().y >= R[i][0].y);
//		ld a = A[i];
//		ld h = R[i].back().y;
//		int yi = find_y(Y, R[i][0].y);
//		//std::cout << "start y:: " << R[i][0].y << "\n";
//		//std::cout << "start yi:: " << yi << "\n";
//		//std::cout << "start yy:: " << Y[yi] << "\n";
//		//std::cout << "start h:: " << h << "\n";
//		int k = j + 1;
//		while (1) {
//			std::cout << "yi:: " << yi << "\n";
//			if (yi <= 0) { F.push_back(a); break; }
//			ld y = Y[yi];
//			std::cout << y << " " << h << "\n";
//			if (eq(y, h)) { F.push_back(a); break; }
//			while (k < rt && !eq(y, R[I[k]].back().y)) k++;
//			std::cout << "A:: " << A.size() << "\n";
//			std::cout << "I:: " << I.size() << "\n";
//			std::cout << "k:: " << k << "\n";
//			if (k >= I.size()) k--;
//			//assert(A[I[k]] < 0);
//			a += A[I[k]];
//			yi = find_y(Y, R[I[k]][0].y);
//		}
//	}
//	std::sort(F.rbegin(), F.rend());
//	//std::cout << "DEBUG:: \n";
//	//for (int j = 0; j < F.size(); j++) {
//	//	std::cout << "F[" << j << "]:: " << F[j] << "\n";
//	//}
//	//std::cout << "DEBUG:: \n";
//	return F[0];
//}