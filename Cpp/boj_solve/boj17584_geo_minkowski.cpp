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
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<bool> Vbool;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
bool zero(const ld& x) { return !sign(x); }
int gcd(int a, int b) { return !b ? a : gcd(b, a % b); }
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

int N, M;
struct Pos {
	int x, y, i;
	Pos(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	//bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator < (const Pos& p) const { return x == p.x ? y == p.y ? i > p.i : y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return ret < 0 ? -1 : !!ret; }
bool same_dir(const Pos& p, const Pos& q) { return p * q >= 0 && p / q == 0; }
ll area(const Polygon& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
void norm(Polygon& H) {
	ll A = area(H);
	if (A < 0) std::reverse(H.begin(), H.end());
	return;
}
void fit(Polygon& H) {
	auto p = std::min_element(H.begin(), H.end());
	std::rotate(H.begin(), p, H.end());
	return;
}
bool convex(Polygon& H) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		int i0 = (i - 1 + sz) % sz, i1 = i, i2 = (i + 1) % sz;
		if (ccw(H[i0], H[i1], H[i2]) <= 0) return 0;
	}
	return 1;
}
bool same_polygon(Polygon& R, Polygon& S) {
	fit(R); fit(S);
	int sz = R.size();
	if (sz != S.size()) return 0;
	Pos v = R[0] - S[0];
	for (Pos& p : S) p += v;
	for (int i = 0; i < sz; i++) if (R[i] != S[i]) return 0;
	return 1;
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
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
bool minkowski_difference(Polygon& R, Polygon& S) {
	ll ar = area(R);
	ll as = area(S);
	if (ar == as) {
		if (same_polygon(R, S)) S.clear();
		return 0;
	}
	if (ar > as) std::swap(R, S);
	if (R.size() > S.size()) return 0;
	int sz = R.size();
	Vbool F(sz, 0);
	Polygon V;
	for (int i = 0; i < sz; i++) {
		Pos v = R[(i + 1) % sz] - R[i];
		int gcd_ = gcd(std::abs(v.x), std::abs(v.y));
		v /= gcd_;
		v.i = i;
		V.push_back(v);
	}
	sz = S.size();
	for (int i = 0; i < sz; i++) {
		Pos v = S[(i + 1) % sz] - S[i];
		int gcd_ = gcd(std::abs(v.x), std::abs(v.y));
		v /= gcd_;
		v.i = ~i;
		V.push_back(v);
	}
	int r = -1, s = -1;
	std::sort(V.begin(), V.end());
	sz = V.size();
	for (int i = 0; i < sz - 1; i++) {
		if (V[i] == V[i + 1]) {
			assert(V[i].i >= 0); assert(V[i + 1].i < 0);
			if (!~r) r = V[i].i, s = ~V[i + 1].i;
			F[V[i].i] = 1;
		}
	}
	if (!~r) return 0;
	sz = F.size();
	for (int i = 0; i < sz; i++) if (!F[i]) return 0;

	Polygon H;
	H.push_back(Pos(0, 0));
	int n = R.size(), m = S.size();
	for (int _ = 0; _ < n + m; _++) {
		int r1 = (r + 1) % n;
		int s1 = (s + 1) % m;
		Pos vr = R[r1] - R[r];
		Pos sr = S[s1] - S[s];
		Pos cur = H.back();
		assert(vr.Euc()); assert(sr.Euc());
		if (same_dir(vr, sr)) {
			if (vr.Euc() > sr.Euc()) return 0;
			r = (r + 1) % n;
			s = (s + 1) % m;
			if (vr.Euc() == sr.Euc()) continue;
			cur += sr - vr;
		}
		else {
			s = (s + 1) % m;
			cur += sr;
		}
		H.push_back(cur);
		if (H.back() == H[0]) break;
	}

	if (H.back() != H[0]) return 0;
	if (H.back() == H[0]) H.pop_back();
	S = graham_scan(H);
	return 1;
}
bool query() {
	std::cin >> N >> M;
	if (!N && !M) return 0;
	Polygon R(N), S(M);
	for (Pos& p : R) std::cin >> p;
	for (Pos& p : S) std::cin >> p;
	norm(R), norm(S);
	assert(convex(R));
	assert(convex(S));
	while (minkowski_difference(R, S));
	std::cout << area(R) + area(S) << "\n";
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("../../../input_data/H_addition/H3", "r", stdin);
	//freopen("../../../input_data/H_addition/H_ret.txt", "w", stdout);
	while (query());
	return;
}
int main() { solve(); return 0; }//boj17584 Addition on Convex Polygons