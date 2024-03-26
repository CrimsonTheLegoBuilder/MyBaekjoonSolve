#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef long double ld;
//typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const ld PI2 = PI * 2 / 3;
const ld PI4 = PI * 4 / 3;
int N;
ll R;
ld len;
bool zero(const ld& x) { return std::abs(x) < TOL; }
ld norm(ld th) {
	while (th < -TOL) th += PI * 2;
	while (th > PI * 2) th -= PI * 2;
	return th;
}

struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ll& n) const { return { x * n, y * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
	Pos operator ~ () const { return { -y, x }; }
	ll operator ! () const { return x * y; }
	ll Euc() const { return x * x + y * y; }
	ll Man() const { return std::abs(x) + std::abs(y); }
	//ld mag() const { return hypot(x, y); }
	ld mag() const { return sqrtl(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
std::vector<Pos> C, H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2 - d1) * (d4 - d3) / (d2 - d1).mag();
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	ll ret = cross(d1, d2, d3, d4);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0;
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
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
	Vec& operator *= (const ld& scalar) { vy *= scalar; vx *= scalar; return *this; }
	Vec& operator /= (const ld& scalar) { vy /= scalar; vx /= scalar; return *this; }
	ld mag() const { return hypot(vy, vx); }
}; const Vec Zero = { 0, 0 };
struct Line {//ax + by = c
	Vec s;
	ld c;
	Line(Vec V = Vec(0, 0), ld C = 0) : s(V), c(C) {}
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ld CCW = s / l.s;
		return zero(CCW) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : CCW > 0;
	}
	ld operator / (const Line& l) const { return s / l.s; }
	ld operator * (const Line& l) const { return s * l.s; }
	Line operator + (const ld& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
	Line operator - (const ld& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
	Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator *= (const ld& scalar) { s *= scalar, c *= scalar; return *this; }
	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	ld mag() const { return s.mag(); }
	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
};
const Line Xaxis = { { 0, -1 }, 0 };
const Line Yaxis = { { 1, 0 }, 0 };
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return { { dy, dx } , c };
}
Line rotate(const Line& l, const Pos& p, ld the) {
	Vec s = l.s;
	ld x = -s.vx, y = s.vy;
	ld vx = -(x * cos(the) - y * sin(the));
	ld vy = x * sin(the) + y * cos(the);
	ld c = vy * p.x + vx * p.y;
	return Line(Vec(vy, vx), c);
}
ld get_theta(const Line& b, const Line& l) {
	ld proj = (b * l) / b.mag();
	ld arm = (b / l) / b.mag();
	return atan2(arm, proj);
}

struct Pdd {
	ld x, y;
	Pdd(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	Pdd operator - (const Pdd& p) const { return { x - p.x, y - p.y }; }
	ld operator * (const Pdd& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pdd& p) const { return { x * p.y - y * p.x }; }
	ld mag() const { return hypot(x, y); }
};
Pdd intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
ld get_width(ld the, const Line& B, const Line& C, const Line& A, const Pos& pb, const Pos& pc, const Pos& pa) {
	Line base = rotate(B, pb, the);
	Line lc = rotate(C, pc, the);
	Line la = rotate(A, pa, the);
	Pdd ba = intersection(base, la);
	Pdd bc = intersection(base, lc);
	return (ba - bc).mag();
}

ld ternary_search(const std::vector<Pos>& H, const int& i, const int& r, const int& l, const Line& B, const Line& C, const Line& A) {
	int sz = H.size(), cnt = 50;
	ld the, a, b, c;
	Line tmp;
	tmp = L(H[(i + 1) % sz], H[(i + 2) % sz]);
	b = get_theta(B, tmp);
	tmp = L(H[r], H[(r + 1) % sz]);
	c = get_theta(C, tmp);
	tmp = L(H[l], H[(l + 1) % sz]);
	a = get_theta(A, tmp);
	the = std::min({ a, b, c });
	ld s = 0, e = the, m1 = 0, m2 = 0, lll = 0, rrr = 0;
	while (cnt--) {
		m1 = (s + s + e) / 3;
		m2 = (s + e + e) / 3;
		lll = get_width(m1, B, C, A, H[(i + 1) % sz], H[r], H[l]);
		rrr = get_width(m2, B, C, A, H[(i + 1) % sz], H[r], H[l]);
		if (lll > rrr) e = m2;
		else s = m1;
	}
	return lll;
}
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N >> R;
	len = R * 2.0 * sqrt(3);
	C.resize(N);
	for (int i = 0; i < N; i++) std::cin >> C[i];
	H = graham_scan(C);
	return;
}
void rotating_calipers() {
	N = H.size();
	assert(N);
	if (N == 1) {
		int cnt = 2;
		while (cnt--) std::cout << 3.4641016151377545870548926830117447338856 * R << "\n";
		return;
	}
	if (N == 2) {
		ld d = (H[0] - H[1]).mag();
		ld r1 = d + len;
		ld r2 = 2 * d / sqrtl(3) + len;
		std::cout << r1 << "\n" << r2 << "\n";
		return;
	}

	//for (const Pos& p : H) std::cout << p << "\n";
	//auto CROSS = [&](const int& i, const int& f) -> ll {
	//	return (H[(i + 1) % N] - H[i]) / (H[(f + 1) % N] - H[f]);
	//	};
	auto r_side = [&](const int& i, const int& j) -> bool {
		int CCW = ccw(H[i], H[(i + 1) % N], H[j], H[(j + 1) % N]);
		ld proj = projection(H[i], H[(i + 1) % N], H[j], H[(j + 1) % N]);
		return CCW >= 0 && (proj > 0 || proj > -((H[j] - H[(j + 1) % N]).mag() * .5 + TOL));
		};
	auto l_side = [&](const int& i, const int& j) -> bool {
		int CCW = ccw(H[i], H[(i + 1) % N], H[j], H[(j + 1) % N]);
		ld proj = projection(H[i], H[(i + 1) % N], H[j], H[(j + 1) % N]);
		return CCW > 0 || proj < -((H[j] - H[(j + 1) % N]).mag() * .5 + TOL);
		};

	ld MIN = INF, MAX = -INF;
	int r = 0;
	while (r_side(0, r)) r = (r + 1) % N;
	//while (r_side(0, r)) {
	//	r = (r + 1) % N;
	//	std::cout << projection(H[0], H[(0 + 1) % N], H[r], H[(r + 1) % N]) << "\n";
	//	std::cout << (H[r] - H[(r + 1) % N]).mag() * .5 + TOL << "\n";
	//	std::cout << H[r] << "\n";
	//}
	//std::cout << "DEBUG::r_find\n";
	int l = r;
	while (l_side(0, l)) l = (l + 1) % N;
	//std::cout << "DEBUG::l_find\n";

	for (int i = 0; i < N; i++) {
		while (r_side(i, r)) r = (r + 1) % N;
		while (l_side(i, l)) l = (l + 1) % N;
		//std::cout << "DEBUG::\n";
		//std::cout << "idx: " << i << " coord: " << H[i] << "\n\n";
		//std::cout << "idx: " << (i + 1) % N << " coord: " << H[(i + 1) % N] << "\n";
		//std::cout << "idx: " << r << " coord: " << H[r] << "\n";
		//std::cout << "idx: " << l << " coord: " << H[l] << "\n";
		//std::cout << "DEBUG::\n";

		Line B = L(H[i], H[(i + 1) % N]);//base
		Line C = rotate(B, H[r], PI2);
		Line A = rotate(B, H[l], PI4);
		Pdd pa = intersection(B, A), pc = intersection(B, C);
		ld d = (pa - pc).mag();
		MIN = std::min(MIN, d + len);
		d = ternary_search(H, i, r, l, B, C, A);
		MAX = std::max(MAX, d + len);
		//std::cout << MIN << " " << MAX << "\n";
	}
	std::cout << MIN << "\n" << MAX << "\n";
	return;
}
void solve() { init(); rotating_calipers(); return; }
int main() { solve(); return 0; }//boj24554 dissonance


//ll ccw(const std::vector<Pos>& H, const int& i, const int& f) {
//	int sz = H.size();
//	return (H[(i + 1) % sz] - H[i]) / (H[(f + 1) % sz] - H[f]);
//}

/*

3 100000
100000000 -100000000
-100000000 -100000000
0 73205100
200346432.381545931100845
400346432.381545901298523

*/