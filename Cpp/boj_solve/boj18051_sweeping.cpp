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
const int LEN = 50005;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline ll nC2(const ll& n) { return (n - 1) * n >> 1; }

int Z, N, M, Q;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
bool cmpt(const Pos& p, const Pos& q) { assert(p / q); return p / q ? p / q > 0 : p.Euc() < q.Euc(); }
ll conquer(Polygon& C, int l, int m, int r) {
	int i = l, j = m + 1, k = 0;
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
	if (l >= r) return 0;
	ll cnt = 0;
	int m = l + r >> 1;
	cnt += divide(C, l, m);
	cnt += divide(C, m + 1, r);
	cnt += conquer(C, l, m, r);
	return cnt;
}
ll merge_sort(Polygon& C) { return divide(C, 0, C.size() - 1); }
ll inv_count(Polygon& C, const Pos& a, const Pos& b) {
	int sz = C.size();
	ll all = nC2((ll)sz);
	for (int i = 0; i < sz; i++) C[i] -= a;
	std::sort(C.begin(), C.end(), cmpt);
	for (int i = 0; i < sz; i++) C[i] += a - b;
	ll ret = all - merge_sort(C);
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
	Polygon ab, a0b, ab0;
	sz = A.size();
	for (int i = 0; i < sz; i++) {
		if (ccw(a0, b0, A[i]) >= 0 && ccw(b, a, A[i]) >= 0) ab.push_back(A[i]);
		if (ccw(a0, b0, A[i]) >= 0 && ccw(b0, a, A[i]) >= 0) ab0.push_back(A[i]);
		if (ccw(a0, b0, A[i]) >= 0 && ccw(b, a0, A[i]) >= 0) a0b.push_back(A[i]);
	}
	ll ans = inv_count(ab, a, b) - inv_count(ab0, a, b0) - inv_count(a0b, a0, b);
	std::cout << ans << " ";
	return;
}
void query() {
	std::cin >> N; Polygon H(N);
	for (int i = 0; i < N; i++) std::cin >> H[i];
	std::cin >> M; Polygon A(M);
	for (int j = 0; j < M; j++) std::cin >> A[j];
	std::cin >> Q; while (Q--) query(H, A);
	std::cout << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> Z; while (Z--) query();
	return;
}
int main() { solve(); return 0; }//boj18051 Antennas



//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <cmath>
//#include <cstring>
//#include <cassert>
//typedef long long ll;
////typedef long double ld;
//typedef double ld;
//const ld INF = 1e17;
//const ld TOL = 1e-7;
//const ld PI = acos(-1);
//const int LEN = 50005;
//inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
//inline ll nC2(const ll& n) { return (n - 1) * n >> 1; }
//
//int Z, N, M, Q;
//struct Pos {
//	int x, y;
//	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
//	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
//	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
//	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
//	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
//	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
//	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
//	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
//	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
//	Pos operator - () const { return { -x, -y }; }
//	ll Euc() const { return (ll)x * x + (ll)y * y; }
//	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
//}; const Pos O = Pos(0, 0);
//typedef std::vector<Pos> Polygon;
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
//bool cmpt(const Pos& p, const Pos& q) { assert(p / q); return p / q ? p / q > 0 : p.Euc() < q.Euc(); }
//ll conquer(Polygon& C, int l, int m, int r) {
//	int i = l, j = m + 1, k = 0;
//	Polygon tmp(r - l + 1);
//	ll cnt = 0;
//	while (i <= m && j <= r) {
//		if (cmpt(C[i], C[j])) tmp[k++] = C[i++];
//		else tmp[k++] = C[j++], cnt += ((ll)m + 1 - i);
//	}
//	while (i <= m) tmp[k++] = C[i++];
//	while (j <= r) tmp[k++] = C[j++];
//	for (i = l, k = 0; i <= r; i++, k++) C[i] = tmp[k];
//	return cnt;
//}
//ll divide(Polygon& C, int l, int r) {
//	if (l >= r) return 0;
//	ll cnt = 0;
//	int m = l + r >> 1;
//	cnt += divide(C, l, m);
//	cnt += divide(C, m + 1, r);
//	cnt += conquer(C, l, m, r);
//	return cnt;
//}
//ll merge_sort(Polygon& C) { return divide(C, 0, C.size() - 1); }
//ll inv_count(Polygon& C, const Pos& a, const Pos& b) {
//	int sz = C.size();
//	ll all = nC2((ll)sz);
//	for (int i = 0; i < sz; i++) C[i] -= a;
//	std::sort(C.begin(), C.end(), cmpt);
//	for (int i = 0; i < sz; i++) C[i] += a - b;
//	ll ret = all - merge_sort(C);
//	for (int i = 0; i < sz; i++) C[i] += b;
//	return ret;
//}
//void query(const Polygon& H, const Polygon& A) {
//	int a_, b_;
//	std::cin >> a_ >> b_;
//	int sz = H.size();
//	Pos a0 = H[a_], a = H[(a_ + 1) % sz], b = H[b_], b0 = H[(b_ + 1) % sz];
//	/*
//	   a --- b         a == b      a - b
//	   |     |   ||   / \      ||   \ /
//	   a0 -- b0      a0 b0           a0 == b0
//	*/
//	Polygon ab, a0b, ab0;
//	sz = A.size();
//	for (int i = 0; i < sz; i++) {
//		if (ccw(a0, b0, A[i]) >= 0 && ccw(b, a, A[i]) >= 0) ab.push_back(A[i]);
//		if (ccw(a0, b0, A[i]) >= 0 && ccw(b0, a, A[i]) >= 0) ab0.push_back(A[i]);
//		if (ccw(a0, b0, A[i]) >= 0 && ccw(b, a0, A[i]) >= 0) a0b.push_back(A[i]);
//	}
//	ll ans = inv_count(ab, a, b) - inv_count(ab0, a, b0) - inv_count(a0b, a0, b);
//	//std::cout << "FUCK:: " << inv_count(ab, a, b) << " fuck:: " << inv_count(ab0, a, b0) << " fuck:: " << inv_count(a0b, a0, b) << "\n";
//	//std::cout << "SUCK:: " << ab.size() << " suck:: " << ab0.size() << " suck:: " << a0b.size() << "\n";
//	std::cout << ans << " ";
//	return;
//}
//void query() {
//	std::cin >> N; Polygon H(N);
//	for (int i = 0; i < N; i++) std::cin >> H[i];
//	std::cin >> M; Polygon A(M);
//	for (int j = 0; j < M; j++) std::cin >> A[j];
//	std::cin >> Q; while (Q--) query(H, A);
//	std::cout << "\n";
//	return;
//}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> Z; while (Z--) query();
//	return;
//}
//int main() { solve(); return 0; }//boj18051 Antennas
