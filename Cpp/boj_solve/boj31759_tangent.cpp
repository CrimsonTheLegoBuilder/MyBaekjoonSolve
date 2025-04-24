#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <deque>
#include <cassert>
#include <cstring>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-12;
const ld PI = acos(-1);
const int LEN = 1e4 + 5;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}

int N, R;
ld memo[2000];
struct Info { int hi, lo; };
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
	ld rad() const { return norm(atan2(y, x)); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return zero(ret) ? 0 : ret > 0 ? 1 : -1; }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) > 0; }
int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
	int sz = H.size();
	if (!sz) return -1;
	if (sz == 1) return p == H[0] ? 0 : -1;
	if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return 1;
	else if (on_seg_strong(H[s], H[e], p)) return 0;
	else return -1;
}
std::vector<Pos> graham_scan(std::vector<Pos>& C) {
	std::vector<Pos> H;
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
inline Vld circle_line_intersections(const Pos& s, const Pos& e, const ll& r) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	Pos vec = e - s;
	Pos OM = s - O;
	ld a = vec.Euc();
	ld b = vec * OM;
	ld c = OM.Euc() - r * r;
	ld J = b * b - a * c;
	if (J < -TOL) return {};
	ld det = sqrt(std::max((ld)0, J));
	ld lo = (-b - det) / a;
	ld hi = (-b + det) / a;
	Vld ret;
	auto the = [&](ld rt) { return (s + (e - s) * rt).rad(); };
	ret.push_back(the(hi));
	if (zero(det)) return ret;
	ret.push_back(the(lo));
	return ret;
}
Info find_tangent_bi_search(const Polygon& H, const Pos& p) {
	if (inner_check_bi_search(H, p) >= 0) return { -1, -1 };
	int sz = H.size();
	int i1{ 0 }, i2{ 0 };
	int ccw1 = ccw(p, H[0], H[1]), ccwN = ccw(p, H[0], H[sz - 1]);
	if (ccw1 * ccwN >= 0) {
		i1 = 0;
		if (!ccw1 && dot(p, H[1], H[0]) > 0) i1 = 1;
		if (!ccwN && dot(p, H[sz - 1], H[0]) > 0) i1 = sz - 1;
		int s = 0, e = sz - 1, m;
		if (!ccw1) s += 1;
		if (!ccwN) e -= 1;
		bool f = ccw(p, H[s], H[s + 1]) >= 0;
		while (s < e) {
			m = s + e >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % sz];
			if (!f) std::swap(p1, cur);//normalize
			if (ccw(p1, cur, nxt) > 0) s = m + 1;
			else e = m;
		}
		i2 = s;
		if (on_seg_weak(p, H[i2], H[(i2 + 1) % sz])) i2 = (i2 + 1) % sz;
	}
	else {
		//divide hull
		int s = 0, e = sz - 1, k, m;
		bool f = ccw1 > 0 && ccwN < 0;//if H[k] is between H[0] && p
		while (s + 1 < e) {
			k = s + e >> 1;
			int CCW = ccw(H[0], H[k], p);
			if (!f) CCW *= -1;//normailze
			if (CCW > 0) s = k;
			else e = k;
		}

		//search lower hull
		int s1 = 0, e1 = s;
		while (s1 < e1) {
			m = s1 + e1 >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % sz];
			if (!f) std::swap(p1, cur);//normalize
			if (ccw(p1, cur, nxt) > 0) s1 = m + 1;
			else e1 = m;
		}
		i1 = s1;
		if (on_seg_weak(p, H[i1], H[(i1 + 1) % sz])) i1 = (i1 + 1) % sz;

		//search upper hull
		int s2 = e, e2 = sz - 1;
		while (s2 < e2) {
			m = s2 + e2 >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % sz];
			if (!f) std::swap(p1, cur);//normalize
			if (ccw(p1, cur, nxt) < 0) s2 = m + 1;
			else e2 = m;
		}
		i2 = s2;
		if (on_seg_weak(p, H[i2], H[(i2 + 1) % sz])) i2 = (i2 + 1) % sz;
	}
	if (ccw(p, H[i1], H[i2]) < 0) std::swap(i1, i2);//normalize
	return { i1, i2 };
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	std::cin >> N >> R;
	ll x, y;
	Polygon C;
	for (int i = 0; i < N; i++) {
		std::cin >> x >> y;
		//assert(R * R >= x * x + y * y);
		if (R * R < x * x + y * y) continue;
		C.push_back(Pos(x, y));
	}
	N = C.size();

	if (N == 1) { std::cout << 0 << "\n"; return; }
	if (N == 2) {
		Pos& p1 = C[0], & p2 = C[1];
		Pos vec = ~(p1 - p2).unit();
		Pos q1 = vec * R;
		Pos q2 = -vec * R;
		ld a = std::max(std::abs(cross(p1, p2, q1)), std::abs(cross(p1, p2, q2)));
		std::cout << a * .5 << "\n";
		return;
	}
	Polygon H = graham_scan(C);
	N = H.size();

	memset(memo, 0, sizeof memo);
	for (int i = 0; i < N; i++) memo[i + 1] = memo[i] + H[i] / H[(i + 1) % N];

	Vld arcs;
	for (int i = 0; i < N; i++) {
		Pos& s = H[i], & e = H[(i + 1) % N];
		auto inx = circle_line_intersections(s, e, R);
		for (ld& rd : inx) arcs.push_back(rd);
	}

	arcs.push_back(0);
	arcs.push_back(2 * PI);
	std::sort(arcs.begin(), arcs.end());

	ld ret = memo[N];
	int sz = arcs.size();
	for (int i = 0; i < sz - 1; i++) {
		ld& s = arcs[i], & e = arcs[i + 1];
		if (zero(s - e)) continue;
		ld m = (s + e) * .5;
		Pos S = Pos(1, 0).rot(s) * R;
		Pos E = Pos(1, 0).rot(e) * R;
		Pos M = Pos(1, 0).rot(m) * R;
		Info t = find_tangent_bi_search(H, M);
		Pos& u = H[t.hi], & v = H[t.lo];
		Pos vec = ~(v - u).unit();
		Pos OP = vec * R;
		bool f = 0;
		if (ccw(O, S, OP) >= 0 && ccw(O, E, OP) <= 0) f = 1;
		ld a, a1, a2, a3 = 0;
		if (t.hi == t.lo) {
			a1 = a2 = a3 = memo[N];
		}
		else if (t.hi < t.lo) {
			a = memo[t.lo] - memo[t.hi] + H[t.lo] / H[t.hi];
			a1 = a + cross(S, H[t.hi], H[t.lo]);
			a2 = a + cross(E, H[t.hi], H[t.lo]);
			if (f) a3 = a + cross(OP, H[t.hi], H[t.lo]);
		}
		else {
			a = memo[N] - (memo[t.hi] - memo[t.lo] + H[t.hi] / H[t.lo]);
			a1 = a + cross(S, H[t.hi], H[t.lo]);
			a2 = a + cross(E, H[t.hi], H[t.lo]);
			if (f) a3 = a + cross(OP, H[t.hi], H[t.lo]);
		}
		ret = std::max({ ret, a1, a2, a3 });
	}
	std::cout << ret * .5 << "\n";
	return;
}
int main() { solve(); return 0; }//boj31759 Insects, Mathematics, Accuracy, and Efficiency
