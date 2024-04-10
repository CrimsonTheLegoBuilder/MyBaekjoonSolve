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
const ld PI2 = PI * 2 / 3;
const ld PI4 = PI * 4 / 3;
int N;
ll R;
ld len;
bool zero(const ld& x) { return std::abs(x) < TOL; }

struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
std::vector<Pos> C, H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
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
	//C.erase(unique(C.begin(), C.end()), C.end());
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
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	ld mag() const { return hypot(vy, vx); }
}; const Vec Zero = { 0, 0 };
struct Line {//ax + by = c
	Vec s;
	ld c;
	Line(Vec V = Vec(0, 0), ld C = 0) : s(V), c(C) {}
	ld operator * (const Line& l) const { return s * l.s; }
	ld operator / (const Line& l) const { return s / l.s; }
	ld mag() const { return s.mag(); }
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
	ld x = b * l;//dot
	ld y = b / l;//cross
	return atan2(y, x);
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
ld get_width(ld the, const Line& I, const Line& J, const Line& K, const Pos& pi, const Pos& pj, const Pos& pk) {
	Line base = rotate(I, pi, the);
	Line lj = rotate(J, pj, the);
	Line lk = rotate(K, pk, the);
	Pdd dj = intersection(base, lj);
	Pdd dk = intersection(base, lk);
	return (dk - dj).mag();
}
ld ternary_search(const std::vector<Pos>& H, const int& i, const int& j, const int& k, const Line& I, const Line& J, const Line& K) {
	int sz = H.size();// , cnt = 50;
	ld the, t1, t2, t3;
	Line tmp;
	tmp = L(H[i], H[(i + 1) % sz]);
	t1 = get_theta(I, tmp);
	tmp = L(H[j], H[(j + 1) % sz]);
	t2 = get_theta(J, tmp);
	tmp = L(H[k], H[(k + 1) % sz]);
	t3 = get_theta(K, tmp);
	the = std::min({ t1, t2, t3 });
	ld s = 0, e = the, m1 = 0, m2 = 0, l = 0, r = 0;
	//while (cnt--) {
	while (!zero(e - s)) {
		m1 = (s + s + e) / 3;
		m2 = (s + e + e) / 3;
		l = get_width(m1, I, J, K, H[i], H[j], H[k]);
		r = get_width(m2, I, J, K, H[i], H[j], H[k]);
		if (l > r) e = m2;
		else s = m1;
	}
	return l;
	//return (l + r) * .5;
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
	//assert(N);
	if (N == 1) {
		std::cout << len << "\n" << len << "\n";
		return;
	}
	if (N == 2) {
		ld d = (H[0] - H[1]).mag();
		ld r1 = d + len;
		ld r2 = 2 * d / sqrtl(3) + len;
		std::cout << r1 << "\n" << r2 << "\n";
		return;
	}

	auto r_side = [&](const int& i, const int& j) -> bool {
		int CCW = ccw(H[i], H[(i + 1) % N], H[j], H[(j + 1) % N]);
		ld proj = projection(H[i], H[(i + 1) % N], H[j], H[(j + 1) % N]);
		return CCW >= 0 && proj > -((H[j] - H[(j + 1) % N]).mag() * .5 + TOL);
		};
	auto l_side = [&](const int& i, const int& j) -> bool {
		int CCW = ccw(H[i], H[(i + 1) % N], H[j], H[(j + 1) % N]);
		ld proj = projection(H[i], H[(i + 1) % N], H[j], H[(j + 1) % N]);
		return CCW > 0 || proj < -((H[j] - H[(j + 1) % N]).mag() * .5 + TOL);
		};

	ld MIN = INF, MAX = -INF;
	int j = 0;
	while (r_side(0, j)) j = (j + 1) % N;
	int k = j;
	while (l_side(0, k)) k = (k + 1) % N;

	for (int i = 0; i < N; i++) {
		while (r_side(i, j)) j = (j + 1) % N;
		while (l_side(i, k)) k = (k + 1) % N;

		Line I = L(H[i], H[(i + 1) % N]);//base
		Line J = rotate(I, H[j], PI2);
		Line K = rotate(I, H[k], PI4);
		Pdd pj = intersection(I, J), pk = intersection(I, K);
		ld d = (pj - pk).mag();
		MIN = std::min(MIN, d + len);
		d = ternary_search(H, (i + 1) % N, j, k, I, J, K);
		MAX = std::max({ MAX, MIN, d + len });
	}
	std::cout << MIN << "\n" << MAX << "\n";
	return;
}
void solve() { init(); rotating_calipers(); return; }
int main() { solve(); return 0; }//boj24554 dissonance

/*

6 1
-1 1
0 1
1 0
2 1
2 2
0 2
7.3185471151148753492520503245706996303351
8.4521211336566876305770188367804703807810

2 1
0 0
0 0
3.4641016151377545870548926830117447338856
3.4641016151377545870548926830117447338856

3 1000
1000000 -1000000
-1000000 -1000000
0 732051
2003464.3238154587832122592932813272231824980393
4003464.3238154587446747262015174035707576541323

6 10000000
-10000000 10000000
0 10000000
10000000 0
20000000 10000000
20000000 20000000
0 20000000
73185471.151148751378059
84521211.336566895246506

3 100000
100000000 -100000000
-100000000 -100000000
0 73205100
200346432.381545931100845
400346432.381545901298523

3 0
0 0
1000 0
500 10
1000.000000000000000
1154.700538379251839

3 0
0 0
100000000 0
50000000 1000000
100000000.000000000000000
115470053.837925180792809

4 0
0 0
1000 0
1000 1000
0 1000
2154.700538379252066
2230.710143300821983

4 0
0 0
100000000 0
100000000 100000000
0 100000000
215470053.837925165891647
223071014.330082148313522

4 0
100000000 100000000
100000000 -100000000
-100000000 -100000000
-100000000 100000000
430940107.675850391387939
446142028.660164296627045


3 0
3000000 0
0 4000000
1500000 2000001
5000000.000000000000000
5773502.691896258853376

3 0
5000000 0
0 0
2500000 1
5000000.000000000000000
5773502.691896260716021

3 0
30000000 0
0 40000000
15000000 20000001
50000000.000000000000000
57735026.918962605297565
57735026.91896257645091487803

3 0
50000000 0
0 0
25000000 1
50000000.000000000000000
57735026.918962590396404
57735026.91896257645091487803

8 0
10 5
5 10
-5 10
-10 5
-10 -5
-5 -10
5 -10
10 -5
37.320508075688778
37.764449636673966

8 0
100000000 50000000
50000000 100000000
-50000000 100000000
-100000000 50000000
-100000000 -50000000
-50000000 -100000000
50000000 -100000000
100000000 -50000000
373205080.756887793540955
377644496.366739809513092

8 0
10000000 5000000
5000000 10000000
-5000000 10000000
-10000000 5000000
-10000000 -5000000
-5000000 -10000000
5000000 -10000000
10000000 -5000000
37320508.075688779354095
37764449.636673972010612

3 0
0 -100000000
-50000000 50000000
50000000 50000000
173205080.756887793540955
273205080.756887793540955

3 0
0 -10
-5 5
5 5
17.320508075688778 ::15 * (2 / 3 ** .5)
27.320508075688782 ::10 + 15 * (2 / 3 ** .5)

3 1000
1000000 -1000000
-1000000 -1000000
0 732048

16 0
-5 -6
0 -7
3 -7
8 -6
9 -5
10 0
10 3
9 8
8 9
3 10
0 10
-5 9
-6 8
-7 3
-7 0
-6 -5
32.032548757776119
32.366884963966747

2 1
0 0
0 1
4.464101615137754
4.618802153517006


*/


//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <cmath>
//#include <cstring>
//#include <cassert>
//typedef long long ll;
//typedef long double ld;
////typedef double ld;
//const ld INF = 1e17;
//const ld TOL = 1e-7;
//const ld PI = acos(-1);
//const ld PI2 = PI * 2 / 3;
//const ld PI4 = PI * 4 / 3;
//int N;
//ll R;
//ld len;
//bool zero(const ld& x) { return std::abs(x) < TOL; }
//ld norm(ld th) {
//	while (th < -TOL) th += PI * 2;
//	while (th > PI * 2) th -= PI * 2;
//	return th;
//}
//
//struct Pos {
//	ll x, y;
//	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
//	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
//	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
//	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	Pos operator * (const ll& n) const { return { x * n, y * n }; }
//	Pos operator / (const ll& n) const { return { x / n, y / n }; }
//	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
//	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
//	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
//	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
//	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
//	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
//	Pos operator ~ () const { return { -y, x }; }
//	ll operator ! () const { return x * y; }
//	ll Euc() const { return x * x + y * y; }
//	ll Man() const { return std::abs(x) + std::abs(y); }
//	//ld mag() const { return hypot(x, y); }
//	ld mag() const { return sqrtl(Euc()); }
//	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
//}; const Pos O = { 0, 0 };
//std::vector<Pos> C, H;
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
//ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
//	return (d2 - d1) * (d4 - d3) / (d2 - d1).mag();
//}
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ll ret = cross(d1, d2, d3);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
//	ll ret = cross(d1, d2, d3, d4);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
//bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
//}
//bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0;
//}
//std::vector<Pos> graham_scan(std::vector<Pos>& C) {
//	std::vector<Pos> H;
//	if (C.size() < 3) {
//		std::sort(C.begin(), C.end());
//		return C;
//	}
//	std::swap(C[0], *min_element(C.begin(), C.end()));
//	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
//		int ret = ccw(C[0], p, q);
//		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
//		return ret > 0;
//		}
//	);
//	C.erase(unique(C.begin(), C.end()), C.end());
//	int sz = C.size();
//	for (int i = 0; i < sz; i++) {
//		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
//			H.pop_back();
//		H.push_back(C[i]);
//	}
//	return H;
//}
//struct Vec {
//	ld vy, vx;
//	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
//	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
//	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
//	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
//	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
//	Vec operator ~ () const { return { -vx, vy }; }
//	Vec& operator *= (const ld& scalar) { vy *= scalar; vx *= scalar; return *this; }
//	Vec& operator /= (const ld& scalar) { vy /= scalar; vx /= scalar; return *this; }
//	ld mag() const { return hypot(vy, vx); }
//}; const Vec Zero = { 0, 0 };
//struct Line {//ax + by = c
//	Vec s;
//	ld c;
//	Line(Vec V = Vec(0, 0), ld C = 0) : s(V), c(C) {}
//	bool operator < (const Line& l) const {
//		bool f1 = Zero < s;
//		bool f2 = Zero < l.s;
//		if (f1 != f2) return f1;
//		ld CCW = s / l.s;
//		return zero(CCW) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : CCW > 0;
//	}
//	ld operator / (const Line& l) const { return s / l.s; }
//	ld operator * (const Line& l) const { return s * l.s; }
//	Line operator + (const ld& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
//	Line operator - (const ld& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
//	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
//	Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
//	Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
//	Line& operator *= (const ld& scalar) { s *= scalar, c *= scalar; return *this; }
//	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
//	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
//	ld mag() const { return s.mag(); }
//	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
//};
//const Line Xaxis = { { 0, -1 }, 0 };
//const Line Yaxis = { { 1, 0 }, 0 };
//Line L(const Pos& s, const Pos& e) {
//	ld dy, dx, c;
//	dy = e.y - s.y;
//	dx = s.x - e.x;
//	c = dy * s.x + dx * s.y;
//	return { { dy, dx } , c };
//}
//Line rotate(const Line& l, const Pos& p, ld the) {
//	Vec s = l.s;
//	ld x = -s.vx, y = s.vy;
//	ld vx = -(x * cos(the) - y * sin(the));
//	ld vy = x * sin(the) + y * cos(the);
//	ld c = vy * p.x + vx * p.y;
//	return Line(Vec(vy, vx), c);
//}
//ld get_theta(const Line& b, const Line& l) {
//	ld proj = (b * l) / b.mag();
//	ld arm = (b / l) / b.mag();
//	return atan2(arm, proj);
//}
//
//struct Pdd {
//	ld x, y;
//	Pdd(ld X = 0, ld Y = 0) : x(X), y(Y) {}
//	Pdd operator - (const Pdd& p) const { return { x - p.x, y - p.y }; }
//	ld operator * (const Pdd& p) const { return { x * p.x + y * p.y }; }
//	ld operator / (const Pdd& p) const { return { x * p.y - y * p.x }; }
//	ld mag() const { return hypot(x, y); }
//};
//Pdd intersection(const Line& l1, const Line& l2) {
//	Vec v1 = l1.s, v2 = l2.s;
//	ld det = v1 / v2;
//	return {
//		(l1.c * v2.vx - l2.c * v1.vx) / det,
//		(l2.c * v1.vy - l1.c * v2.vy) / det,
//	};
//}
//ld get_width(ld the, const Line& B, const Line& C, const Line& A, const Pos& pb, const Pos& pc, const Pos& pa) {
//	Line base = rotate(B, pb, the);
//	Line lc = rotate(C, pc, the);
//	Line la = rotate(A, pa, the);
//	Pdd ba = intersection(base, la);
//	Pdd bc = intersection(base, lc);
//	return (ba - bc).mag();
//}
//
//ld ternary_search(const std::vector<Pos>& H, const int& i, const int& r, const int& l, const Line& B, const Line& C, const Line& A) {
//	int sz = H.size(), cnt = 50;
//	ld the, a, b, c;
//	Line tmp;
//	tmp = L(H[(i + 1) % sz], H[(i + 2) % sz]);
//	b = get_theta(B, tmp);
//	tmp = L(H[r], H[(r + 1) % sz]);
//	c = get_theta(C, tmp);
//	tmp = L(H[l], H[(l + 1) % sz]);
//	a = get_theta(A, tmp);
//	the = std::min({ a, b, c });
//	ld s = 0, e = the, m1 = 0, m2 = 0, lll = 0, rrr = 0;
//	while (cnt--) {
//		m1 = (s + s + e) / 3;
//		m2 = (s + e + e) / 3;
//		lll = get_width(m1, B, C, A, H[(i + 1) % sz], H[r], H[l]);
//		rrr = get_width(m2, B, C, A, H[(i + 1) % sz], H[r], H[l]);
//		if (lll > rrr) e = m2;
//		else s = m1;
//	}
//	return lll;
//}
//void init() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(15);
//	std::cin >> N >> R;
//	//len = R * 3.4641016151377545870548926830117447338856;
//	len = R * 2.0 * sqrt(3);
//	C.resize(N);
//	for (int i = 0; i < N; i++) std::cin >> C[i];
//	H = graham_scan(C);
//	return;
//}
//void rotating_calipers() {
//	N = H.size();
//	assert(N);
//	if (N == 1) {
//		int cnt = 2;
//		//while (cnt--) std::cout << 3.4641016151377545870548926830117447338856 * R << "\n";
//		while (cnt--) std::cout << len << "\n";
//		return;
//	}
//	if (N == 2) {
//		ld d = (H[0] - H[1]).mag();
//		ld r1 = d + len;
//		ld r2 = 2 * d / sqrtl(3) + len;
//		std::cout << r1 << "\n" << r2 << "\n";
//		return;
//	}
//
//	//for (const Pos& p : H) std::cout << p << "\n";
//	//auto CROSS = [&](const int& i, const int& f) -> ll {
//	//	return (H[(i + 1) % N] - H[i]) / (H[(f + 1) % N] - H[f]);
//	//	};
//	auto r_side = [&](const int& i, const int& j) -> bool {
//		int CCW = ccw(H[i], H[(i + 1) % N], H[j], H[(j + 1) % N]);
//		ld proj = projection(H[i], H[(i + 1) % N], H[j], H[(j + 1) % N]);
//		return CCW >= 0 && (proj > 0 || proj > -((H[j] - H[(j + 1) % N]).mag() * .5 + TOL));
//		};
//	auto l_side = [&](const int& i, const int& j) -> bool {
//		int CCW = ccw(H[i], H[(i + 1) % N], H[j], H[(j + 1) % N]);
//		ld proj = projection(H[i], H[(i + 1) % N], H[j], H[(j + 1) % N]);
//		return CCW > 0 || proj < -((H[j] - H[(j + 1) % N]).mag() * .5 + TOL);
//		};
//
//	ld MIN = INF, MAX = -INF;
//	int r = 0;
//	while (r_side(0, r)) r = (r + 1) % N;
//	//while (r_side(0, r)) {
//	//	r = (r + 1) % N;
//	//	std::cout << projection(H[0], H[(0 + 1) % N], H[r], H[(r + 1) % N]) << "\n";
//	//	std::cout << (H[r] - H[(r + 1) % N]).mag() * .5 + TOL << "\n";
//	//	std::cout << H[r] << "\n";
//	//}
//	//std::cout << "DEBUG::r_find\n";
//	int l = r;
//	while (l_side(0, l)) l = (l + 1) % N;
//	//std::cout << "DEBUG::l_find\n";
//
//	for (int i = 0; i < N; i++) {
//		while (r_side(i, r)) r = (r + 1) % N;
//		while (l_side(i, l)) l = (l + 1) % N;
//		//std::cout << "DEBUG::\n";
//		//std::cout << "idx: " << i << " coord: " << H[i] << "\n\n";
//		//std::cout << "idx: " << (i + 1) % N << " coord: " << H[(i + 1) % N] << "\n";
//		//std::cout << "idx: " << r << " coord: " << H[r] << "\n";
//		//std::cout << "idx: " << l << " coord: " << H[l] << "\n";
//		//std::cout << "DEBUG::\n";
//
//		Line B = L(H[i], H[(i + 1) % N]);//base
//		Line C = rotate(B, H[r], PI2);
//		Line A = rotate(B, H[l], PI4);
//		Pdd pa = intersection(B, A), pc = intersection(B, C);
//		ld d = (pa - pc).mag();
//		MIN = std::min(MIN, d + len);
//		d = ternary_search(H, i, r, l, B, C, A);
//		MAX = std::max(MAX, d + len);
//		//std::cout << MIN << " " << MAX << "\n";
//	}
//	std::cout << MIN << "\n" << MAX << "\n";
//	return;
//}
//void solve() { init(); rotating_calipers(); return; }
//int main() { solve(); return 0; }//boj24554 dissonance
