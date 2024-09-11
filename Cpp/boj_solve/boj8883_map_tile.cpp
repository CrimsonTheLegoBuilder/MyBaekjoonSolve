#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <random>
#include <array>
#include <tuple>
#include <unordered_set>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }

int N, xs, ys;
struct Pii {
	int x, y;
	Pii(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pii& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pii& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
	Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
	Pii operator * (const int& n) const { return { x * n, y * n }; }
	Pii operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pii& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pii& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pii operator ^ (const Pii& p) const { return { x * p.x, y * p.y }; }
	Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
	Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
	Pii& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pii& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pii operator - () const { return { -x, -y }; }
	Pii operator ~ () const { return { -y, x }; }
	Pii operator ! () const { return { y, x }; }
	ll xy() const { return (ll)x * y; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pii& p1, const Pii& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pii& a, const Pii& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
}; const Pii Oii = Pii(0, 0);
struct PiiHash { std::size_t operator() (const Pii& p) const { return (((ll)p.x + 100000) << 32) | ((ll)p.y + 100000); } };
typedef std::unordered_set<Pii, PiiHash> SetPii;
typedef std::vector<Pii> Polygon;
ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pii& d1, const Pii& d2, const Pii& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
int ccw(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { ll ret = cross(d1, d2, d3, d4); return ret < 0 ? -1 : !!ret; }
ld projection(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d1) / (d2 - d1).mag(); }
ld projection(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) * (d4 - d3) / (d2 - d1).mag(); }
bool on_seg_strong(const Pii& d1, const Pii& d2, const Pii& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
bool on_seg_weak(const Pii& d1, const Pii& d2, const Pii& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
bool collinear(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
bool intersect(const Pii& s1, const Pii& s2, const Pii& d1, const Pii& d2, const bool f = 1) {
	//f : include end of seg, !f : ignore end of seg
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	if (!f) return f1 && f2;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
ll area(Polygon& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
void norm(Polygon& H) {
	ll A = area(H);
	if (A < 0) std::reverse(H.begin(), H.end());
	auto s = std::min_element(H.begin(), H.end());
	std::rotate(H.begin(), s, H.end());
	return;
}
Polygon graham_scan(Polygon& C) {
	Polygon H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pii& p, const Pii& q) -> bool {
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
bool inner_check(const Polygon& H, const Pii& p) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) if (ccw(H[i], H[(i + 1) % sz], p) < 0) return 0;
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
typedef std::vector<Pos> Polygonf;
Pos norm(Pos& p, const int& x, const int& y) {
	while (sign(x - p.x) >= 0) p.x -= x;
	while (sign(p.x) < 0) p.x += x;
	while (sign(y - p.y) >= 0) p.y -= y;
	while (sign(p.y) < 0) p.y += y;
	return p;
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ld ret = cross(d1, d2, d3, d4); return sign(ret); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) > 0; }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
Pos P(const Pii& p) { return Pos((ld)p.x, (ld)p.y); }
Pii P(const Pos& p) { return Pii(p.x + TOL, p.y + TOL); }
void norm(int& x, const int& vx, const int& xs) {
	while (x <= (vx - xs)) x += xs;
	while (x > vx) x -= xs;
	return;
}
void norm(int& x, const ld& vx, const int& xs) {
	while (sign((vx - xs) - x) >= 0) x += xs;
	while (sign(x - vx) > 0) x -= xs;
	return;
}
int sweep(const Polygonf& HF, const int& xs, const int& ys) {
	SetPii S;
	Polygonf tmp;
	int sz = HF.size();
	for (int i = 0; i < sz; i++) {
		const Pos& I1 = HF[i], & I2 = HF[(i + 1) % sz];
		Pos s = I1, e = I2;
		norm(s, xs, ys);
		if (zero(s.x)) tmp.push_back(I1);
		norm(e, xs, ys);
		if (zero(e.x)) {
			const Pos& I3 = HF[(i + 2) % sz];
			Pos I0 = I2 + Pos(0, 1);
			if (ccw(I0, I2, I1) == ccw(I0, I2, I3)) tmp.push_back(I2);
		}
		if (zero(I1.x - I2.x)) continue;
		int lx = 0, rx = 0;
		ld minx = std::min(I1.x, I2.x), maxx = std::max(I1.x, I2.x);
		ld miny = std::min(I1.y, I2.y), maxy = std::max(I1.y, I2.y);
		norm(lx, minx, xs);
		norm(rx, maxx, xs);
		if (zero(lx - minx)) lx += xs;
		if (zero(rx - maxx)) rx -= xs;
		for (int x = lx; x <= rx; x += xs) {
			Pos ix = intersection(I1, I2, Pos(x, miny), Pos(x, maxy));
			tmp.push_back(ix);
		}
	}
	std::sort(tmp.begin(), tmp.end());
	int szt = tmp.size(), cnt;
	int y;
	for (int i = 0, j = 0; i < szt; i = j) {
		ld x = tmp[i].x;
		while (j < szt && zero(x - tmp[j].x)) j++;
		cnt = 0;
		y = 0;
		norm(y, tmp[i].y, ys);
		for (int k = i; k < j; k++) {

		}
	}
	return (int)S.size();
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(5);
	std::cin >> N >> xs >> ys;
	Polygon H(N);
	Polygonf V;
	for (int i = 0; i < N; i++) std::cin >> H[i], H[i] += Pii(100, 100);
	norm(H);
	Pii S = H[0];
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			Pii I = H[i], J = H[j], K = H[(j + 1) % N];
			int x = I.x, y = J.y;
			Pii s = S - Pii(x, y);
			Pos v = P(s);
			V.push_back(norm(v, xs, ys));
			if (J.x == K.x) continue;
			if (J.x > K.x) std::swap(J, K);
			int jx = I.x;
			norm(jx, J.x, xs);
			int kx = I.x;
			norm(kx, K.x, xs);
			Pii vec = K - J;
			for (int kw = jx; kw <= kx; kw += xs) {
				int cur = kw * xs + I.x;
				if (J.x > cur || cur > K.x) continue;
				ld yh = J.y + ((ld)kw - J.x) * vec.y / vec.x;
				v = P(S) - Pos(I.x, yh);
				V.push_back(norm(v, xs, ys));
			}
		}
	}
	for (int i = 0; i < N; i++) {//O(50)
		Pii I1 = H[i], I2 = H[(i + 1) % N];
		for (int j = 0; j < N; j++) {//O(50 * 50)
			Pii J1 = H[j], J2 = H[(j + 1) % N];
			if (i == j || !cross(I1, I1, J1, J2)) continue;
			if (I2 < I1) std::swap(I1, I2);
			if (J2 < J1) std::swap(J1, J2);
			Pii vec = I2 - I1;
			Polygon box = { J1, J1 - vec, J2, J2 - vec };
			box = graham_scan(box);
			int lx = 1e9, rx = -1e9, ly = 1e9, uy = -1e9;
			for (int k = 0; k < 4; k++) {
				lx = std::min(lx, box[k].x);
				rx = std::max(rx, box[k].x);
				ly = std::min(ly, box[k].y);
				uy = std::max(uy, box[k].y);
			}
			int jx = I1.x;
			norm(jx, lx, xs);
			int jy = I1.y;
			norm(jy, ly, ys);
			for (int x = jx; x <= rx; x += xs) {//O(50 * 50 * 20)
				for (int y = jy; y <= uy; y += ys) {//O(50 * 50 * 20 * 20)
					if (inner_check(box, Pii(x, y))) {//O(50 * 50 * 20 * 20 * 4) == O(4000000)
						Pos o = intersection(P(J1), P(J2), Pos(x, y), Pos(x, y) + P(vec));
						Pos v = P(S) - o;
						V.push_back(norm(v, xs, ys));
					}
				}
			}
		}
	}
	Polygonf HF(N);
	for (int i = 0; i < N; i++) HF[i] = P(H[i]);
	std::sort(V.begin(), V.end());
	V.erase(unique(V.begin(), V.end()), V.end());
	int sz = V.size();
	int cnt = 1e5;
	for (int z = 0; z < sz; z++) {
		Pos s = V[z];
		Polygonf C = HF;
		Pos v = s - HF[0];
		for (int i = 0; i < N; i++) HF[i] += v;
		cnt = std::min(cnt, sweep(HF, xs, ys));
	}
	std::cout << cnt << "\n";
	return;
}
int main() { solve(); return 0; }//boj8883