#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <deque>
#include <random>
#include <array>
#include <tuple>
#include <complex>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 3e3 + 5;
int N, M, T, Q;
bool V[LEN];
bool zero(const ld& x) { return std::abs(x) < TOL; }
//int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
//int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
//ld norm(ld th) {
//	while (th < -TOL) th += PI * 2;
//	while (th > PI * 2) th -= PI * 2;
//	return th;
//}
//ld flip(ld lat) {
//	if (zero(lat - PI * .5) || zero(lat + PI * .5)) return 0;
//	if (zero(lat)) return PI * .5;
//	if (lat > 0) return PI * .5 - lat;
//	if (lat < 0) return -(PI * .5) - lat;
//	return INF;
//}
//ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
	ld ang() const { return atan2(y, x); }
	Pos unit() const { return *this / mag(); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
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
	ld operator * (const Line& l) const { return s * l.s; }
	ld operator / (const Line& l) const { return s / l.s; }
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
std::vector<Pos> pd[LEN];//power diagram (Laguerre-Voronoi diagram)
//const Line Xaxis = { { 0, -1 }, 0 };
//const Line Yaxis = { { 1, 0 }, 0 };
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return Line(Vec(dy, dx), c);
}
Line L(const Vec& s, const Pos& p) {
	ld c = s.vy * p.x + s.vx * p.y;
	return Line(s, c);
}
//Line rotate(const Line& l, const Pos& p, ld the) {
//	Vec s = l.s;
//	ld x = -s.vx, y = s.vy;
//	ld vx = -(x * cos(the) - y * sin(the));
//	ld vy = x * sin(the) + y * cos(the);
//	ld c = vy * p.x + vx * p.y;
//	return Line(Vec(vy, vx), c);
//}
//Pos rotate(Pos c, ld R, ld the) {
//	Pos p = Pos(R, 0);
//	ld vx = p.x * cos(the) - p.y * sin(the);
//	ld vy = p.x * sin(the) + p.y * cos(the);
//	return c + Pos(vx, vy);
//}
//Line rotate90(const Line& l, const Pos& p) {
//	Vec s = ~l.s;
//	ld c = s.vy * p.x + s.vx * p.y;
//	return Line(s, c);
//}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return Pos(
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det
	);
}
ld ang(const Pos& b, const Pos& l) { return atan2(b / l, b * l); }
ld ang(const Line& b, const Line& l) { return atan2(b / l, b * l); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dist(const Pos& d1, const Pos& d2, const Pos& t) {
	return cross(d1, d2, t) / (d1 - d2).mag();
}
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && ret > 0;
}
//int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
//	int sz = H.size();
//	if (!sz) return -1;
//	if (sz == 1) return p == H[0] ? 0 : -1;
//	if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
//	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[(sz - 1) % sz], p) > 0) return -1;
//	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[(sz - 1) % sz], p)) return 0;
//	int s = 0, e = sz - 1, m;
//	while (s + 1 < e) {
//		m = s + e >> 1;
//		if (cross(H[0], H[m], p) > 0) s = m;
//		else e = m;
//	}
//	if (cross(H[s], H[e], p) > 0) return 1;
//	else if (on_seg_strong(H[s], H[e], p)) return 0;
//	else return -1;
//}
//ld area(const std::vector<Pos>& H) {
//	Pos P = { 0, 0 };
//	ld ret = 0;
//	int h = H.size();
//	for (int i = 0; i < h; i++) {
//		Pos cur = H[i], nxt = H[(i + 1) % h];
//		ret += cross(P, cur, nxt);
//	}
//	//return ret / 2;
//	return ret;
//}
bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& hull) {
	auto cw = [&](const Line& l1, const Line& l2, const Line& target) -> bool {
		if (l1.s / l2.s < TOL) return 0;
		return target.above(intersection(l1, l2)) > -TOL;
		};
	std::deque<Line> dq;
	std::sort(HP.begin(), HP.end());
	for (const Line& l : HP) {
		if (!dq.empty() && zero(dq.back() / l)) continue;
		while (dq.size() >= 2 && cw(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
		while (dq.size() >= 2 && cw(l, dq.front(), dq[1])) dq.pop_front();
		dq.push_back(l);
	}
	while (dq.size() >= 3 && cw(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
	while (dq.size() >= 3 && cw(dq.back(), dq.front(), dq[1])) dq.pop_front();
	for (int i = 0; i < dq.size(); i++) {
		Line cur = dq[i], nxt = dq[(i + 1) % (int)dq.size()];
		if (cur / nxt < TOL) {
			hull.clear();
			return 0;
		}
		hull.push_back(intersection(cur, nxt));
	}
	return 1;
}
std::vector<Pos> cut(const std::vector<Pos>& C, const Pos& b1, const Pos& b2) {
	std::vector<Pos> ret;
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = C[i], nxt = C[(i + 1) % sz];
		int c1 = ccw(b1, b2, cur), c2 = ccw(b1, b2, nxt);
		if (c1 >= 0) ret.push_back(cur);
		if (c1 * c2 < 0) ret.push_back(intersection(L(b1, b2), L(cur, nxt)));
	}
	return ret;
}
std::vector<Pos> sutherland_hodgman(const std::vector<Pos>& C, const std::vector<Pos>& clip) {
	int sz = clip.size();
	std::vector<Pos> ret = C;
	for (int i = 0; i < sz; i++) {
		Pos b1 = clip[i], b2 = clip[(i + 1) % sz];
		ret = cut(ret, b1, b2);
	}
	return ret;
}
struct Circle {
	Pos c;
	ld r;
	Circle(Pos C = Pos(0, 0), ld R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator < (const Circle& q) const {
		ld dist = (c - q.c).mag();
		return r < q.r && dist + r < q.r + TOL;
		//return r < q.r && dist + r <= q.r;
	}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	ld A() const { return r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c.x >> c.c.y >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c.x << " " << c.c.y << " " << c.r; return os; }
} INVAL = { { 0, 0 }, -1 };
std::vector<Circle> disks;
bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	std::cin >> N >> Q;
	std::vector<Circle> tmp(N);
	for (Circle& c : tmp) std::cin >> c;
	std::sort(tmp.begin(), tmp.end(), cmpr);
	memset(V, 0, sizeof V);
	for (int i = 0; i < N; i++) {//remove duplicates
		if (V[i]) continue;
		for (int j = i + 1; j < N; j++) {
			if (tmp[j] == tmp[i]) V[j] = 1;
			if (tmp[j] < tmp[i]) V[j] = 1;
		}
	}
	for (int i = 0; i < N; i++) if (!V[i]) disks.push_back(tmp[i]);
	N = disks.size();
	for (int i = 0; i < N; i++) {//compose power diagram
		std::vector<Line> HP;
		HP.push_back(Line(Vec(1, 0), 3e6));
		HP.push_back(Line(Vec(-1, 0), 3e6));
		HP.push_back(Line(Vec(0, 1), 3e6));
		HP.push_back(Line(Vec(0, -1), 3e6));
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			Pos& ca = disks[i].c, cb = disks[j].c;
			ld& ra = disks[i].r, rb = disks[j].r;
			Pos vec = cb - ca;//vec a -> b
			ld distance = vec.mag();
			ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance);
			Pos m = ca + vec * X / distance;
			HP.push_back(L(m, m + ~vec));
		}
		if (!half_plane_intersection(HP, pd[i])) continue;
	}
	return;
}
ld valid_area(const Circle& disk, const std::vector<Pos> HPI) {
	int sz = HPI.size();
	ld r = disk.r;
	Pos c = disk.c;
	ld a = 0;
	for (int i = 0; i < sz; i++) {
		Pos p1 = HPI[i], p2 = HPI[(i + 1) % sz];
		Pos v1 = p1 - c, v2 = p2 - c, vec = p2 - p1;
		ld d = dist(v1, v2, O);
		if (zero(d)) continue;
		if (std::abs(d) > r - TOL) { a += r * r * ang(v1, v2); continue; }
		Pos m = intersection(L(v1, v2), L(O, ~vec));
		ld distance = vec.mag();
		ld ratio = sqrtl(r * r - d * d);
		Pos m1 = m - vec * ratio / distance;
		Pos m2 = m + vec * ratio / distance;

		bool f1 = v1.Euc() > r * r, f2 = v2.Euc() > r * r;
		if (f1 && f2) {
			if (on_seg_weak(v1, v2, m1)) a += r * r * (ang(v1, m1) + ang(m2, v2)) + m1 / m2;
			//if (dot(v1, m1, v2) > 0) a += r * r * (ang(v1, m1) + ang(m2, v2)) + m1 / m2;
			else a += r * r * ang(v1, v2);
		}
		else if (f1) a += r * r * ang(v1, m1) + m1 / v2;
		else if (f2) a += v1 / m2 + r * r * ang(m2, v2);
		else a += v1 / v2;
	}
	return a * .5;
}
ld query(ld A, const std::vector<Pos> box) {
	ld a = 0;
	for (int i = 0; i < N; i++) {
		int sz = pd[i].size();
		if (!sz) continue;
		std::vector<Line> HP;
		std::vector<Pos> HPI;
		for (int j = 0; j < 4; j++) HP.push_back(L(box[j], box[(j + 1) % 4]));
		for (int j = 0; j < sz; j++) HP.push_back(L(pd[i][j], pd[i][(j + 1) % sz]));
		if (!half_plane_intersection(HP, HPI)) continue;
		a += valid_area(disks[i], HPI);

		//std::vector<Pos> HPI = sutherland_hodgman(pd[i], box);
		//if (HPI.size() < 3) continue;
		//a += valid_area(disks[i], HPI);
	}
	return a * 100. / A;
}
void solve() {
	init();
	ld x, y, w, h;
	std::vector<Pos> box;
	while (Q--) {
		std::cin >> x >> y >> w >> h;
		ld A = w * h;
		box = { Pos(x, y), Pos(x + w, y), Pos(x + w, y + h), Pos(x, y + h) };
		std::cout << query(A, box) << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj22923 NAC 2021 B Apple Orchard

//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cstring>
//#include <cassert>
//#include <cmath>
//#include <vector>
//#include <queue>
//#include <deque>
//#include <tuple>
//typedef long long ll;
//typedef double ld;
//typedef long double lld;
//const ld INF = 1e18;
//const ld TOL = 1e-7;
//const ld PI = acos(-1);
//const int LEN = 3e3 + 5;
//int N, M, T, Q;
//bool V[LEN];
//bool zero(const ld& x) { return std::abs(x) < TOL; }
//int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
//int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
//ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
//ld norm(ld th) {
//	while (th < -TOL) th += PI * 2;
//	while (th > PI * 2) th -= PI * 2;
//	return th;
//}
//ld flip(ld lat) {
//	if (zero(lat - PI * .5) || zero(lat + PI * .5)) return 0;
//	if (zero(lat)) return PI * .5;
//	if (lat > 0) return PI * .5 - lat;
//	if (lat < 0) return -(PI * .5) - lat;
//	return INF;
//}
//
////O(N log N) Delaunay_triangulation && Voronoi_diagram solver
////https://cp-algorithms.com/geometry/delaunay.html
//struct Pii {
//	int x, y, i;
//	Pii(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
//	bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
//	bool operator != (const Pii& p) const { return x != p.x || y != p.y; }
//	bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
//	bool operator <= (const Pii& p) const { return x == p.x ? y <= p.y : x <= p.x; }
//	Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
//	Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
//	Pii operator * (const int& n) const { return { x * n, y * n }; }
//	Pii operator / (const int& n) const { return { x / n, y / n }; }
//	ll operator * (const Pii& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
//	ll operator / (const Pii& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
//	Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
//	Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
//	Pii& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
//	Pii& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
//	Pii operator ~ () const { return { -y, x }; }
//	ll operator ! () const { return (ll)x * y; }
//	ll Euc() const { return (ll)x * x + (ll)y * y; }
//	int Man() const { return std::abs(x) + std::abs(y); }
//	ld mag() const { return hypot(x, y); }
//	friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
//};
//const Pii Oii = { 0, 0 };
//const Pii INF_PT = { (int)INF, (int)INF };
//ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
//ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) / (d4 - d3); }
//ll dot(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d2); }
//ll dot(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) * (d4 - d3); }
//int ccw(const Pii& d1, const Pii& d2, const Pii& d3) {
//	ll ret = cross(d1, d2, d3);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
//struct QuadEdge {
//	Pii origin;
//	QuadEdge* rot = nullptr;
//	QuadEdge* onext = nullptr;
//	bool used = 0;
//	QuadEdge* rev() const { return rot->rot; }//reverse
//	QuadEdge* lnext() const { return rot->rev()->onext->rot; }
//	QuadEdge* oprev() const { return rot->onext->rot; }
//	Pii dest() const { return rev()->origin; }//destination
//};
//QuadEdge* make_edge(Pii from, Pii to) {
//	QuadEdge* e1 = new QuadEdge;
//	QuadEdge* e2 = new QuadEdge;
//	QuadEdge* e3 = new QuadEdge;
//	QuadEdge* e4 = new QuadEdge;
//	e1->origin = from;
//	e2->origin = to;
//	e3->origin = e4->origin = INF_PT;
//	e1->rot = e3;
//	e2->rot = e4;
//	e3->rot = e2;
//	e4->rot = e1;
//	e1->onext = e1;
//	e2->onext = e2;
//	e3->onext = e4;
//	e4->onext = e3;
//	return e1;
//}
//void splice(QuadEdge* a, QuadEdge* b) {
//	std::swap(a->onext->rot->onext, b->onext->rot->onext);
//	std::swap(a->onext, b->onext);
//}
//void delete_edge(QuadEdge* e) {
//	splice(e, e->oprev());
//	splice(e->rev(), e->rev()->oprev());
//	delete e->rev()->rot;
//	delete e->rev();
//	delete e->rot;
//	delete e;
//}
//QuadEdge* connect(QuadEdge* a, QuadEdge* b) {
//	QuadEdge* e = make_edge(a->dest(), b->origin);
//	splice(e, a->lnext());
//	splice(e->rev(), b);
//	return e;
//}
//bool left_of(Pii p, QuadEdge* e) { return cross(p, e->origin, e->dest()) > 0; }
//bool right_of(Pii p, QuadEdge* e) { return cross(p, e->origin, e->dest()) < 0; }
//template <class T> T det3(T a1, T a2, T a3, T b1, T b2, T b3, T c1, T c2, T c3) {
//	return a1 * (b2 * c3 - c2 * b3)
//		- a2 * (b1 * c3 - c1 * b3)
//		+ a3 * (b1 * c2 - c1 * b2);
//}
//bool in_circle(Pii a, Pii b, Pii c, Pii d) {
//	lld det = -det3<lld>(b.x, b.y, b.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
//	det += det3<lld>(a.x, a.y, a.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
//	det -= det3<lld>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), d.x, d.y, d.Euc());
//	det += det3<lld>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), c.x, c.y, c.Euc());
//	if (fabs(det) > 1e18) return det > 0;//overflow prevention (refer to koosaga)
//	else {
//		ll det = -det3<ll>(b.x, b.y, b.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
//		det += det3<ll>(a.x, a.y, a.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
//		det -= det3<ll>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), d.x, d.y, d.Euc());
//		det += det3<ll>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), c.x, c.y, c.Euc());
//		return det > 0;
//	}
//}
//std::pair<QuadEdge*, QuadEdge*> build_tr(int l, int r, std::vector<Pii>& C) {
//	if (r - l + 1 == 2) {
//		QuadEdge* res = make_edge(C[l], C[r]);
//		return std::make_pair(res, res->rev());
//	}
//	if (r - l + 1 == 3) {
//		QuadEdge* a = make_edge(C[l], C[l + 1]), * b = make_edge(C[l + 1], C[r]);
//		splice(a->rev(), b);
//		int sign = ccw(C[l], C[l + 1], C[r]);
//		if (!sign) return std::make_pair(a, b->rev());
//		QuadEdge* c = connect(b, a);
//		if (sign == 1) return std::make_pair(a, b->rev());
//		else return std::make_pair(c->rev(), c);
//	}
//	int m = l + r >> 1;
//	QuadEdge* ldo, * ldi, * rdo, * rdi;
//	std::tie(ldo, ldi) = build_tr(l, m, C);
//	std::tie(rdi, rdo) = build_tr(m + 1, r, C);
//	while (1) {
//		if (left_of(rdi->origin, ldi)) {
//			ldi = ldi->lnext();
//			continue;
//		}
//		if (right_of(ldi->origin, rdi)) {
//			rdi = rdi->rev()->onext;//rnext
//			continue;
//		}
//		break;
//	}
//	QuadEdge* basel = connect(rdi->rev(), ldi);
//	auto valid = [&basel](QuadEdge* e) { return right_of(e->dest(), basel); };
//	if (ldi->origin == ldo->origin) ldo = basel->rev();
//	if (rdi->origin == rdo->origin) rdo = basel;
//	while (1) {
//		QuadEdge* lcand = basel->rev()->onext;
//		if (valid(lcand)) {
//			while (in_circle(basel->dest(), basel->origin, lcand->dest(), lcand->onext->dest())) {
//				QuadEdge* t = lcand->onext;
//				delete_edge(lcand);
//				lcand = t;
//			}
//		}
//		QuadEdge* rcand = basel->oprev();
//		if (valid(rcand)) {
//			while (in_circle(basel->dest(), basel->origin, rcand->dest(), rcand->oprev()->dest())) {
//				QuadEdge* t = rcand->oprev();
//				delete_edge(rcand);
//				rcand = t;
//			}
//		}
//		if (!valid(lcand) && !valid(rcand)) break;
//		if (!valid(lcand) || (valid(rcand) && in_circle(lcand->dest(), lcand->origin, rcand->origin, rcand->dest())))
//			basel = connect(rcand, basel->rev());
//		else
//			basel = connect(basel->rev(), lcand->rev());
//	}
//	return std::make_pair(ldo, rdo);
//}
//std::vector<std::tuple<Pii, Pii, Pii>> Delaunay_triangulation(std::vector<Pii> C) {
//	std::sort(C.begin(), C.end());
//	int sz = C.size();
//	auto ret = build_tr(0, sz - 1, C);
//	QuadEdge* e = ret.first;
//	std::vector<QuadEdge*> edges = { e };
//	while (cross(e->onext->dest(), e->dest(), e->origin) < 0) e = e->onext;
//	auto add = [&C, &e, &edges]() {
//		QuadEdge* cur = e;
//		do {
//			cur->used = 1;
//			C.push_back(cur->origin);
//			edges.push_back(cur->rev());
//			cur = cur->lnext();
//		} while (cur != e);
//		};
//	add();
//	C.clear();
//	int kek = 0;
//	while (kek < edges.size()) if (!(e = edges[kek++])->used) add();
//	sz = C.size();
//	std::vector<std::tuple<Pii, Pii, Pii>> triangles;
//	for (int i = 0; i < sz; i += 3) triangles.push_back(std::make_tuple(C[i], C[i + 1], C[i + 2]));
//	return triangles;
//}
//struct Pdd {
//	ld x, y;
//	Pdd(ld X = 0, ld Y = 0) : x(X), y(Y) {}
//	bool operator == (const Pdd& p) const { return zero(x - p.x) && zero(y - p.y); }
//	bool operator != (const Pdd& p) const { return !zero(x - p.x) || !zero(y - p.y); }
//	bool operator < (const Pdd& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
//	Pdd operator + (const Pdd& p) const { return { x + p.x, y + p.y }; }
//	Pdd operator - (const Pdd& p) const { return { x - p.x, y - p.y }; }
//	Pdd operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
//	Pdd operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
//	ld operator * (const Pdd& p) const { return { x * p.x + y * p.y }; }
//	ld operator / (const Pdd& p) const { return { x * p.y - y * p.x }; }
//	Pdd operator ~ () const { return { -y, x }; }
//	ld operator ! () const { return x * y; }
//	Pdd& operator += (const Pdd& p) { x += p.x; y += p.y; return *this; }
//	Pdd& operator -= (const Pdd& p) { x -= p.x; y -= p.y; return *this; }
//	Pdd& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
//	Pdd& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
//	ld Euc() const { return x * x + y * y; }
//	ld mag() const { return hypot(x, y); }
//	ld ang() const { return atan2(y, x); }
//	Pdd unit() const { return *this / mag(); }
//	friend std::istream& operator >> (std::istream& is, Pdd& p) { is >> p.x >> p.y; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Pdd& p) { os << p.x << " " << p.y; return os; }
//}; const Pdd Odd = { 0, 0 };
//Pdd P(const Pii& p) { return Pdd((ld)p.x, (ld)p.y); }
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
//	ld operator * (const Line& l) const { return s * l.s; }
//	ld operator / (const Line& l) const { return s / l.s; }
//	Line operator + (const ld& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
//	Line operator - (const ld& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
//	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
//	Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
//	Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
//	Line& operator *= (const ld& scalar) { s *= scalar, c *= scalar; return *this; }
//	ld dist(const Pdd& p) const { return s.vy * p.x + s.vx * p.y; }
//	ld above(const Pdd& p) const { return s.vy * p.x + s.vx * p.y - c; }
//	ld mag() const { return s.mag(); }
//	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
//};
//const Line Xaxis = { { 0, -1 }, 0 };
//const Line Yaxis = { { 1, 0 }, 0 };
//Line L(const Pdd& s, const Pdd& e) {
//	ld dy, dx, c;
//	dy = e.y - s.y;
//	dx = s.x - e.x;
//	c = dy * s.x + dx * s.y;
//	return { { dy, dx } , c };
//}
//Line L(const Vec& s, const Pdd& p) {
//	ld c = s.vy * p.x + s.vx * p.y;
//	return Line(s, c);
//}
//Line rotate(const Line& l, const Pdd& p, ld the) {
//	Vec s = l.s;
//	ld x = -s.vx, y = s.vy;
//	ld vx = -(x * cos(the) - y * sin(the));
//	ld vy = x * sin(the) + y * cos(the);
//	ld c = vy * p.x + vx * p.y;
//	return Line(Vec(vy, vx), c);
//}
//Line rotate90(const Line& l, const Pdd& p) {
//	Vec s = ~l.s;
//	ld c = s.vy * p.x + s.vx * p.y;
//	return { s, c };
//}
//Pdd intersection(const Line& l1, const Line& l2) {
//	Vec v1 = l1.s, v2 = l2.s;
//	ld det = v1 / v2;
//	return {
//		(l1.c * v2.vx - l2.c * v1.vx) / det,
//		(l2.c * v1.vy - l1.c * v2.vy) / det,
//	};
//}
//ld cross(const Pdd& d1, const Pdd& d2, const Pdd& d3) { return (d2 - d1) / (d3 - d2); }
//int ccw(const Pdd& d1, const Pdd& d2, const Pdd& d3) {
//	ld ret = cross(d1, d2, d3);
//	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
//}
//ld dot(const Pdd& d1, const Pdd& d2, const Pdd& d3) { return (d2 - d1) * (d3 - d2); }
////bool cw(const Line& l1, const Line& l2, const Line& target) {
////	if (l1.s / l2.s < TOL) return 0;
////	Pdd p = intersection(l1, l2);
////	//return target.s.vy * p.x + target.s.vx * p.y > target.c - TOL;
////	return target.above(p) > -TOL;
////}
//bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pdd>& hull) {
//	auto cw = [&](const Line& l1, const Line& l2, const Line& target) -> bool {
//		if (l1.s / l2.s < TOL) return 0;
//		return target.above(intersection(l1, l2)) > -TOL;
//		};
//	std::deque<Line> dq;
//	std::sort(HP.begin(), HP.end());
//	for (const Line& l : HP) {
//		if (!dq.empty() && zero(dq.back() / l)) continue;
//		while (dq.size() >= 2 && cw(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
//		while (dq.size() >= 2 && cw(l, dq.front(), dq[1])) dq.pop_front();
//		dq.push_back(l);
//	}
//	while (dq.size() >= 3 && cw(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
//	while (dq.size() >= 3 && cw(dq.back(), dq.front(), dq[1])) dq.pop_front();
//	for (int i = 0; i < dq.size(); i++) {
//		Line cur = dq[i], nxt = dq[(i + 1) % (int)dq.size()];
//		if (cur / nxt < TOL) {
//			hull.clear();
//			return 0;
//		}
//		hull.push_back(intersection(cur, nxt));
//	}
//	return 1;
//}
//struct Circle {
//	Pdd c;
//	ld r;
//	Circle(Pdd C = Pdd(0, 0), ld R = 0) : c(C), r(R) {}
//	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
//	bool operator != (const Circle& C) const { return !(*this == C); }
//	bool operator < (const Circle& q) const {
//		ld dist = (c - q.c).mag();
//		return r < q.r && dist + r < q.r + TOL;
//	}
//	bool operator > (const Pdd& p) const { return r > (c - p).mag(); }
//	bool operator >= (const Pdd& p) const { return r + TOL > (c - p).mag(); }
//	bool operator < (const Pdd& p) const { return r < (c - p).mag(); }
//	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
//	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
//	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
//	ld A() const { return r * r * PI; }
//	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c.x >> c.c.y >> c.r; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c.x << " " << c.c.y << " " << c.r; return os; }
//} INVAL = { { 0, 0 }, -1 };
//std::vector<Circle> disks;
//Circle enclose_circle(const Pdd& u, const Pdd& v) {
//	Pdd c = (u + v) * .5;
//	return Circle(c, (c - u).mag());
//}
//Circle enclose_circle(const Pdd& u, const Pdd& v, const Pdd& w) {
//	Line l1 = rotate90(L(u, v), (u + v) * .5);
//	Line l2 = rotate90(L(v, w), (v + w) * .5);
//	if (zero(l1 / l2)) return { { 0, 0 }, -1 };
//	Pdd c = intersection(l1, l2);
//	ld r = (c - u).mag();
//	return Circle(c, r);
//}
//struct Seg {
//	Pdd s, e;
//	Seg(Pdd S = Pdd(), Pdd E = Pdd()) : s(S), e(E) {}
//};
//std::vector<Seg> seg;
//struct Arc {
//	ld lo, hi;// [lo, hi] - radian range of arc
//	Circle c; // c.r - radius of arc
//	Arc(ld LO = 0, ld HI = 0, Circle C = Circle(O, 0)) : lo(LO), hi(HI), c(C) {}
//	bool operator < (const Arc& a) const { return lo == a.lo ? hi < a.hi : lo < a.lo; }
//};
//std::vector<Arc> arc[LEN];
//bool intersection(const std::vector<Circle>& C, int i, int j) {
//	Circle a = C[i], b = C[j];
//	Pdd ca = a.c, cb = b.c;
//	Pdd vec = cb - ca;//vec a -> b
//	ld ra = a.r, rb = b.r;
//	ld distance = vec.mag();
//
//	if (distance > ra + rb + TOL) return 0;
//	if (distance < std::abs(ra - rb) - TOL) return 0;
//
//	//2nd hyprblc law of cos
//	ld X = (ra * ra - rb * rb + distance * distance) / (2 * distance);
//	ld Y = sqrt(ra * ra - X * X);
//	Pdd m = ca + vec * (X / distance);
//	if (Y > 0) {
//		ld ratio = Y / distance;
//		Pdd l = m + ~vec * ratio, h = m - ~vec * ratio;
//		ld lo, hi;
//		lo = (l - ca).ang();
//		hi = (h - ca).ang();
//		if (lo > hi) {
//			arc[i].push_back(Arc(lo, PI, ca));
//			arc[i].push_back(Arc(-PI, hi, ca));
//		}
//		else arc[i].push_back(Arc(lo, hi, ca));
//		lo = (h - cb).ang();
//		hi = (l - cb).ang();
//		if (lo > hi) {
//			arc[j].push_back(Arc(lo, PI, cb));
//			arc[j].push_back(Arc(-PI, hi, cb));
//		}
//		else arc[j].push_back(Arc(lo, hi, cb));
//		return 1;
//	}
//	return 0;
//}
////struct Arc {
////	ld lo, hi;// [lo, hi] - radian range of arc
////	Circle c; // c.r - radius of arc
////	Arc(ld LO = 0, ld HI = 0, Circle C = Circle(O, 0)) : lo(LO), hi(HI), c(C) {}
////	bool operator < (const Arc& a) const { return lo == a.lo ? hi < a.hi : lo < a.lo; }
////};
////std::vector<Arc> arc[LEN];
////bool intersection(const std::vector<Circle>& C, int i, int j) {
////	Circle a = C[i], b = C[j];
////	Pos ca = a.c, cb = b.c;
////	Pos vec = cb - ca;//vec a -> b
////	ld ra = a.r, rb = b.r;
////	ld distance = vec.mag();
////
////	if (distance > ra + rb + TOL) return 0;
////	if (distance < std::abs(ra - rb) - TOL) return 0;
////
////	//2nd hyprblc law of cos
////	ld X = (ra * ra - rb * rb + distance * distance) / (2 * distance);
////	ld Y = sqrt(ra * ra - X * X);
////	Pos m = ca + vec * (X / distance);
////	if (Y > 0) {
////		ld ratio = Y / distance;
////		Pos l = m + ~vec * ratio, h = m - ~vec * ratio;
////		ld lo, hi;
////		lo = (l - ca).ang();
////		hi = (h - ca).ang();
////		if (lo > hi) {
////			arc[i].push_back(Arc(lo, PI, ca));
////			arc[i].push_back(Arc(-PI, hi, ca));
////		}
////		else arc[i].push_back(Arc(lo, hi, ca));
////		lo = (h - cb).ang();
////		hi = (l - cb).ang();
////		if (lo > hi) {
////			arc[j].push_back(Arc(lo, PI, cb));
////			arc[j].push_back(Arc(-PI, hi, cb));
////		}
////		else arc[j].push_back(Arc(lo, hi, cb));
////		return 1;
////	}
////	return 0;
////}
//std::vector<Pii> C;
//std::vector<Pdd> poly, vd[LEN];
//std::vector<int> seed[LEN];
//ld Voronoi_diagram(const ld& wl, const ld& wr, const ld& hd, const ld& hu, std::vector<Pii> C) {
//	int sz = C.size();
//	poly.resize(sz);
//	for (int i = 0; i < sz; i++) poly[i] = P(C[i]);
//	assert(sz);
//	if (sz == 1) {}
//	else if (sz == 2) {
//		seed[0].push_back(1);
//		seed[1].push_back(0);
//	}
//	else {
//		std::vector<std::tuple<Pii, Pii, Pii>> dt = Delaunay_triangulation(C);
//		for (const std::tuple<Pii, Pii, Pii>& tri : dt) {
//			int a = std::get<0>(tri).i;
//			int b = std::get<1>(tri).i;
//			int c = std::get<2>(tri).i;
//			seed[a].push_back(b);
//			seed[a].push_back(c);
//			seed[b].push_back(a);
//			seed[b].push_back(c);
//			seed[c].push_back(a);
//			seed[c].push_back(b);
//		}
//	}
//	ld ret = 0;
//	for (int i = 0; i < N; i++) {
//		std::sort(seed[i].begin(), seed[i].end());
//		//seed[i].erase(unique(seed[i].begin(), seed[i].end()), seed[i].end());
//		seed[i].resize(unique(seed[i].begin(), seed[i].end()) - seed[i].begin());
//		std::vector<Line> HP;
//		for (const int& j : seed[i]) {
//			//Line hp = rotate90(L(poly[i], poly[j]), (poly[i] + poly[j]) * .5);
//			Line hp = L(~L(poly[i], poly[j]).s, (poly[i] + poly[j]) * .5);
//			HP.push_back(hp);
//		}
//		HP.push_back(Line(Vec(0, 1), hu));
//		HP.push_back(Line(Vec(0, -1), -hd));
//		HP.push_back(Line(Vec(1, 0), wr));
//		HP.push_back(Line(Vec(-1, 0), -wl));
//		std::vector<Pdd> HPI = {};
//		if (!half_plane_intersection(HP, HPI)) continue;
//		//vd[i] = HPI;
//		for (int j = 0; j < HPI.size(); j++) {
//			ld dist = (HPI[j] - poly[i]).mag();
//			ret = std::max(ret, dist);
//		}
//	}
//	return ret;
//}
//void init() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(10);
//	std::cin >> N >> Q;
//	std::vector<Circle> tmp(N);
//	for (Circle& c : tmp) std::cin >> c;
//	memset(V, 0, sizeof V);
//	for (int i = 0; i < N; i++) {
//		if (V[i]) continue;
//		for (int j = i + 1; j < N; j++) {
//			if (tmp[i] == tmp[j]) V[j] = 1;
//			if (tmp[i] < tmp[j]) V[i] = 1;
//			if (tmp[j] < tmp[i]) V[j] = 1;
//		}
//	}
//	for (int i = 0; i < N; i++) if (!V[i]) disks.push_back(tmp[i]);
//	memset(V, 0, sizeof V);
//	N = disks.size();
//	for (int i = 0; i < N; i++) {
//		for (int j = i + 1; j < N; j++) {
//			if (intersection(disks, i, j)) V[i] = 1, V[j] = 1;
//		}
//		if (!V[i]) arc[i].push_back(Arc(-PI, PI, disks[i]));
//	}
//	return;
//}
//void solve() {
//	init();
//	
//	return;
//}
//int main() { solve(); return 0; }
