#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef long double ld;
typedef std::pair<int, int> pi;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
int N, M, T, Q;
inline bool zero(const ld& x) { return std::abs(x) < TOL; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }

struct Pdd {
	ld x, y;
	Pdd(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pdd& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pdd& p) const { return !(*this == p); }
	bool operator < (const Pdd& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator <= (const Pdd& p) const { return *this == p || *this < p; }
	Pdd operator + (const Pdd& p) const { return { x + p.x, y + p.y }; }
	Pdd operator - (const Pdd& p) const { return { x - p.x, y - p.y }; }
	Pdd operator * (const int& n) const { return { x * n, y * n }; }
	Pdd operator / (const int& n) const { return { x / n, y / n }; }
	ld operator * (const Pdd& p) const { return (ld)x * p.x + (ld)y * p.y; }
	ld operator / (const Pdd& p) const { return (ld)x * p.y - (ld)y * p.x; }
	Pdd operator ^ (const Pdd& p) const { return { x * p.x, y * p.y }; }
	Pdd& operator += (const Pdd& p) { x += p.x; y += p.y; return *this; }
	Pdd& operator -= (const Pdd& p) { x -= p.x; y -= p.y; return *this; }
	Pdd& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pdd& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pdd operator - () const { return { -x, -y }; }
	Pdd operator ~ () const { return { -y, x }; }
	Pdd operator ! () const { return { y, x }; }
	ld xy() const { return (ld)x * y; }
	ld Euc() const { return (ld)x * x + (ld)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pdd& p1, const Pdd& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pdd& a, const Pdd& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pdd& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pdd& p) { os << p.x << " " << p.y; return os; }
};
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	inline Pos operator * (const int& n) const { return { x * n, y * n }; }
	inline Pos operator / (const int& n) const { return { x / n, y / n }; }
	inline ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	inline ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	inline Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	inline Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	inline Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	inline Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	inline Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	inline Pos operator - () const { return { -x, -y }; }
	inline Pos operator ~ () const { return { -y, x }; }
	inline Pos operator ! () const { return { y, x }; }
	inline ll xy() const { return (ll)x * y; }
	inline ll Euc() const { return (ll)x * x + (ll)y * y; }
	inline int Man() const { return std::abs(x) + std::abs(y); }
	inline ld mag() const { return hypot(x, y); }
	inline ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
//bool cmpi(const Pos& p, const Pos& q) { return p.i < q.i; }
typedef std::vector<Pos> Polygon;
std::vector<Pos> C, H;
inline ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
inline ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
inline ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	ll ret = cross(d1, d2, d3, d4);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
inline std::vector<Pos> monotone_chain(std::vector<Pos>& C, int d = 0) {
	std::vector<Pos> H;
	if (!d) std::sort(C.begin(), C.end(), cmpx);
	if (d == 1 || d == 3) std::reverse(C.begin(), C.end());
	if (d == 2) std::sort(C.begin(), C.end(), cmpy);
	//C.erase(unique(C.begin(), C.end()), C.end());
	if (C.size() <= 2) { for (const Pos& pos : C) H.push_back(pos); }
	else {
		for (int i = 0; i < C.size(); i++) {
			while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		//H.pop_back();
	}
	if (d == 1 || d == 3) std::reverse(H.begin(), H.end());
	return H;
}
int bi_search(const Polygon& H, const ld& q, bool f = 0) {
	int s = 0, e = H.size() - 1, m = 0;
	while (s < e) {
		m = s + e >> 1;
		bool J = f ? H[m].y < q : H[m].x < q;
		if (J) s = m + 1;
		else e = m;
	}
	assert(s < H.size());
	return s;
}
ld box_area(const Polygon& L, const Polygon& U, const ld& q, const ld& w, Pdd& p1, Pdd& p2, bool f = 0) {
	int l1 = bi_search(L, q, f);
	int l2 = bi_search(L, q + w, f);
	int u1 = bi_search(U, q, f);
	int u2 = bi_search(U, q + w, f);
	std::cout << "DEBUG:: hull size:: " << L.size() << " " << U.size() << "\n";
	std::cout << "DEBUG:: idx :: " << l1 << " " << l2 << " " << u1 << " " << u2 << "\n";
	ld ret = 0;
	if (!f) {
		ld ly1, ly2, uy1, uy2;
		ly1 = L[l1].y + ((ll)L[l1 + 1].y - L[l1].y) * ((q - L[l1].x) / ((ll)L[l1 + 1].x - L[l1].x));
		if (zero(L[l2].x - (q + w))) ly2 = L[l2].y;
		else ly2 = L[l2].y + ((ll)L[l2 + 1].y - L[l2].y) * ((q + w - L[l2].x) / ((ll)L[l2 + 1].x - L[l1].x));
		uy1 = U[u1].y + ((ll)U[u1 + 1].y - U[u1].y) * ((q - U[u1].x) / ((ll)U[u1 + 1].x - U[u1].x));
		if (zero(U[u2].x - (q + w))) uy2 = U[u2].y;
		else uy2 = U[u2].y + ((ll)U[u2 + 1].y - U[u2].y) * ((q + w - U[u2].x) / ((ll)U[u2 + 1].x - U[u1].x));
		
		if (ly2 < ly1 || uy2 < ly1) ret = 0;
		else ret = w * (std::min(uy1, uy2) - std::max(ly1, ly2)), p1 = Pdd(q, std::max(ly1, ly2)), p2 = Pdd(q + w, std::min(uy1, uy2));
	}
	if (f) {
		ld lx1, lx2, ux1, ux2;
		lx1 = L[l1].x + ((ll)L[l1 + 1].x - L[l1].x) * ((q - L[l1].y) / ((ll)L[l1 + 1].y - L[l1].y));
		if (zero(L[l2].y - (q + w))) lx2 = L[l2].x;
		else lx2 = L[l2].x + ((ll)L[l2 + 1].x - L[l2].x) * ((q + w - L[l2].y) / ((ll)L[l2 + 1].y - L[l1].y));
		ux1 = U[u1].x + ((ll)U[u1 + 1].x - U[u1].x) * ((q - U[u1].y) / ((ll)U[u1 + 1].y - U[u1].y));
		if (zero(U[u2].y - (q + w))) ux2 = U[u2].x;
		else ux2 = U[u2].x + ((ll)U[u2 + 1].x - U[u2].x) * ((q + w - U[u2].y) / ((ll)U[u2 + 1].y - U[u1].y));
		
		if (lx2 > lx1 || ux2 > lx1) ret = 0;
		else ret = w * (std::min(lx1, lx2) - std::max(ux1, ux2)), p1 = Pdd(std::max(ux1, ux2), q), p2 = Pdd(std::min(lx1, lx2), q + w);
	}
	return ret;
}
ld ternary_search(const Polygon& L, const Polygon& U, const ld& w, Pdd& p1, Pdd& p2, bool f = 0) {
	int cnt = 50;
	ld ret = 0;
	ld s, e, m1, m2, a1, a2;
	if (!f) s = L[0].x, e = L.back().x - w;
	if (f) s = L[0].y, e = L.back().y - w;
	//while (cnt--) {
	while (!zero(e - s)) {
		m1 = (s + s + e) / 3;
		m2 = (s + e + e) / 3;
		a1 = box_area(L, U, m1, w, p1, p2, f);
		a2 = box_area(L, U, m2, w, p1, p2, f);
		if (a1 < a2) s = m1;
		else e = m2;
	}
	return box_area(L, U, s, w, p1, p2, f);
}
void max_area(Polygon& C) {
	Polygon LO, UP, RI, LE;
	Pdd q1 = Pdd(C[0].x, C[0].y), q2 = Pdd(C[0].x, C[0].y);
	Pdd p1, p2;
	ld ret = 0;
	LO = monotone_chain(C, 0);
	UP = monotone_chain(C, 1);
	RI = monotone_chain(C, 2);
	LE = monotone_chain(C, 3);
	std::cout << "DEBUG:: hull init\n";
	std::cout << "DEBUG:: hull size " << LO.size() << " " << UP.size() << " " << LE.size() << " " << RI.size() << "\n";
	ll min_x = LO[0].x, max_x = LO.back().x;
	//for (int i = 1; i < LO.size() - 1; i++) {
	//	if (LO[i].y <= LO[0].y && LO[0].y <= LO[i + 1].y) {
	//		max_x = LO[i].x + ((ll)LO[i + 1].x - LO[i].x) * ((ld)((ll)LO[0].y - LO[i].y) - ((ll)LO[i + 1].y - LO[i].y));
	//	}
	//}
	//for (int i = 1; i < UP.size() - 1; i++) {
	//	if (UP[i].y <= UP[0].y && UP[0].y <= UP[i + 1].y) {
	//		max_x = UP[i].x + ((ll)UP[i + 1].x - UP[i].x) * ((ld)((ll)UP[0].y - UP[i].y) - ((ll)UP[i + 1].y - UP[i].y));
	//	}
	//}
	ll min_y = RI[0].y, max_y = RI.back().y;
	ld w1 = 0, w2 = (max_x - min_x);
	ld h1 = 0, h2 = (max_y - min_y);
	ld m1 = 0, m2 = 0, a1 = 0, a2 = 0;
	std::cout << "DEBUG:: range init\n";
	int cnt;
	cnt = 1;
	//while (cnt--) {
	while (!zero(w2 - w1)) {
		m1 = (w1 + w1 + w2) / 3;
		m2 = (w1 + w2 + w2) / 3;
		std::cout << "DEBUG:: ts LO UP :: " << cnt++ << "\n";;
		a1 = ternary_search(LO, UP, m1, p1, p2);
		a2 = ternary_search(LO, UP, m2, p1, p2);
		if (a1 < a2) w1 = m1;
		else w2 = m2;
	}
	if (ret < a1) ret = a1, q1 = p1, q2 = p2;
	std::cout << "DEBUG:: ternary search LO UP\n";
	cnt = 1;
	//while (cnt--) {
	while (!zero(h2 - h1)) {
		m1 = (h1 + h1 + h2) / 3;
		m2 = (h1 + h2 + h2) / 3;
		std::cout << "DEBUG:: ts LE RI :: " << cnt++ << "\n";;
		a1 = ternary_search(RI, LE, m1, p1, p2, 1);
		a2 = ternary_search(RI, LE, m2, p1, p2, 1);
		if (a1 < a2) h1 = m1;
		else h2 = m2;
	}
	if (ret < a1) ret = a1, q1 = p1, q2 = p2;
	std::cout << "DEBUG:: ternary search LE RI\n";
	std::cout << q1 << " " << q2 << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N;
	Polygon C(N);
	for (Pos& p : C) std::cin >> p;
	//std::reverse(C.begin(), C.end());
	max_area(C);
	return;
}
int main() { solve(); return 0; }//boj9583 NEERC 2013 E Easy Geometry