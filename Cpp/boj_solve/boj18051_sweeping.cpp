#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 505;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll nC2(const ll& n) { return (n - 1) * n >> 1; }

int Z, N, M, Q;
struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ll xy() const { return (ll)x * y; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return sign(ret); }
bool cmpt(const Pos& p, const Pos& q) {
	assert(p / q);
	if (!(p / q)) return p.Euc() < q.Euc();
	return p / q > 0;
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
bool inner_check(const Polygon& H, const Pos& p) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) if (cross(H[i], H[(i + 1) % sz], p) < 0) return 0;
	return 1;
}
ll conquer(Polygon& C, int l, int m, int r) {
	int i = l, j = r, k = 0;
	Polygon tmp(r - l + 1); 
	ll cnt = 0;
	while (i <= m && j <= r) {
		if (cmpt(C[i], C[j])) tmp[k++] = C[i++];
		else tmp[k++] = C[j++], cnt += ((ll)m + 1 - i);
	}
	while (i <= m) tmp[k++] = C[i++];
	while (j <= r) tmp[k++] = C[j++];
	for (i = l, k = 0; i <= r; i++, k++) C[i] = tmp[k];
	return cnt;
}
ll divide(Polygon& C, int l, int r) {
	ll cnt = 0;
	if (l < r) {
		int m = l + r >> 1;
		cnt += divide(C, l, m);
		cnt += divide(C, m + 1, r);
		cnt += conquer(C, l, m, r);
	}
	return cnt;
}
ll merge_sort(Polygon& C) { return divide(C, 0, C.size() - 1); }
ll inv_count(Polygon& C) { ll inv = merge_sort(C); return inv; }
ll inv_count(Polygon& C, const Pos& a, const Pos& b) {
	int sz = C.size();
	ll all = nC2((ll)sz);
	for (int i = 0; i < sz; i++) C[i] -= a;
	std::sort(C.begin(), C.end(), cmpt);
	for (int i = 0; i < sz; i++) C[i] += a - b;
	ll ret = all - inv_count(C);
	for (int i = 0; i < sz; i++) C[i] += b;
	return ret;
}
void query(const Polygon& H, const Polygon& A) {
	int a_, b_;
	std::cin >> a_ >> b_;
	int sz = H.size();
	Pos a0 = H[a_], a = H[(a_ + 1) % sz], b = H[b_], b0 = H[(b_ + 1) % sz];
	/*
	   a --- b         a == b      a - b
	   |     |   ||   / \      ||   \ /
	   a0 -- b0      a0 b0           a0 == b0 
	*/
	Polygon S = { a0, a, b, b0 }, P;
	Polygon B = graham_scan(S);
	sz = A.size();
	for (int i = 0; i < sz; i++) if (inner_check(B, A[i])) P.push_back(A[i]);
	Polygon ab, a0b, ab0;
	sz = P.size();
	for (int i = 0; i < sz; i++) {
		if (ccw(b, a, P[i]) >= 0) ab.push_back(P[i]);
		if (ccw(b0, a, P[i]) >= 0) ab0.push_back(P[i]);
		if (ccw(b, a0, P[i]) >= 0) a0b.push_back(P[i]);
	}
	ll ans = inv_count(ab, a, b) - inv_count(ab0, a, b0) - inv_count(a0b, a0, b);
	std::cout << ans << " ";
	return;
}
void query() {
	std::cin >> N;
	Polygon H(N);
	for (int i = 0; i < N; i++) std::cin >> H[i];
	std::cin >> M;
	Polygon A(M);
	for (int j = 0; j < M; j++) std::cin >> A[j];
	std::cin >> Q;
	while (Q--) query(H, A);
	std::cout << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//std::cout << std::fixed;
	//std::cout.precision(9);
	std::cin >> Z;
	while (Z--) query();
	return;
}
int main() { solve(); return 0; }//boj18051