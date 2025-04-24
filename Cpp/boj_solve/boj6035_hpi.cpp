#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <deque>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-8;
const ld PI = acos(-1);
const ld limit = 100;
const int LEN = 1e3;
int N, Q;
char JB;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }

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
	friend std::istream& operator >> (std::istream& is, Pos& p) {
		is >> p.x >> p.y;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
		os << p.x << " " << p.y << "\n";
		return os;
	}
};
const Pos O = { 0, 0 };
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	Vec operator ~ () const { return { -vx, vy }; }
	Vec& operator *= (const ld& scalar) { vy *= scalar; vx *= scalar; return *this; }
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
	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
	Line& operator *= (const ld& scalar) { s *= scalar; c *= scalar; return *this; }
	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	friend std::ostream& operator << (std::ostream& os, const Line& l) {
		os << l.s.vy << " " << l.s.vx << " " << l.c << "\n";
		return os;
	}
};
const Line X_axis = { { 0, -1 }, 0 };
const Line Y_axis = { { 1, 0 }, 0 };
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return { { dy, dx } , c };
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
std::vector<Pos> lower_hull(std::vector<Pos>& C) {//lower monotone chain
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
std::vector<Pos> upper_hull(std::vector<Pos>& C) {//upper monotone_chain
	std::vector<Pos> H;
	//std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (int i = C.size() - 1; i >= 0; i--) H.push_back(C[i]);
		return H;
	}
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
bool CW(const Line& l1, const Line& l2, const Line& target) {
	if (l1.s / l2.s < TOL) return 0;
	Pos p = intersection(l1, l2);
	return target.s.vy * p.x + target.s.vx * p.y > target.c - TOL;
}
bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& hull) {
	std::deque<Line> dq;
	std::sort(HP.begin(), HP.end());
	for (const Line& l : HP) {
		if (!dq.empty() && zero(dq.back() / l)) continue;
		while (dq.size() >= 2 && CW(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
		while (dq.size() >= 2 && CW(l, dq.front(), dq[1])) dq.pop_front();
		dq.push_back(l);
	}
	while (dq.size() >= 3 && CW(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
	while (dq.size() >= 3 && CW(dq.back(), dq.front(), dq[1])) dq.pop_front();
	for (int i = 0; i < dq.size(); i++) {
		Line cur = dq[i], nxt = dq[(i + 1) % dq.size()];
		if (cur / nxt < TOL) {
			hull.clear();
			return 0;
		}
		hull.push_back(intersection(cur, nxt));
	}
	return 1;
}
std::vector<Line> HP;
std::vector<Pos> HPI, LO, UP;
Pos ternary_search(std::vector<Pos>& H, const Line& hp, bool f = 0) {
	int s = 0, e = H.size() - 1, l, r;
	ld MAX = INF;
	while (e - s > 2) {
		l = (s + s + e) / 3;
		r = (s + e + e) / 3;
		ld cl = hp.above(H[l]) * (f ? -1 : 1);
		ld cr = hp.above(H[r]) * (f ? -1 : 1);
		//std::cout << cl << " " << cr << "\n";
		if (cl > cr) s = l;
		else e = r;
	}
	Pos EX;
	for (int i = s; i <= e; i++) {
		//std::cout << hp.above(H[i]) * (f ? -1 : 1) << "\n";
		ld D = hp.above(H[i]) * (f ? -1 : 1);
		if (D < MAX) {
			MAX = D;
			EX = H[i];
		}
	}
	return EX;
}
void query() {
	ld jx, jy, jz, bx, by, bz;
	std::cin >> jy >> jx >> jz >> by >> bx >> bz;
	Line l = Line({ (jy - by), (jx - bx) }, bz - jz);
	
	//O(QlogN)
	Line k = Y_axis;
	bool f = zero(k / l) ? k * l < 0 : k / l > 0;
	if (!f) l *= -1;
	Pos lo = ternary_search(LO, l);
	Pos hi = ternary_search(UP, l, 1);
	if (!f) l *= -1;
	ld d1 = l.above(lo);
	ld d2 = l.above(hi);
	//std::cout << "lo hi\n" << lo << hi;
	//std::cout << "d1, d2 : " << d1 << " " << d2 << "\n";
	bool f1 = d1 > -TOL || d2 > -TOL;
	bool f2 = d1 < TOL || d2 < TOL;
	if (f1 && f2) std::cout << "U\n";
	else if (f1) std::cout << "J\n";
	else if (f2) std::cout << "B\n";

	//O(QN)
	//bool f1 = 0, f2 = 0;
	//for (int i = 0; i < HPI.size(); i++) {
	//	if (l.above(HPI[i]) > -TOL) f1 = 1;
	//	if (l.above(HPI[i]) < TOL) f2 = 1;
	//	//std::cout << l.above(HPI[i]) << "\n";
	//}
	//if (f1 && f2) std::cout << "U\n";
	//else if (f1) std::cout << "J\n";
	//else if (f2) std::cout << "B\n";
	return;
}
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("../../../input_data/starc/starc.15.IN", "r", stdin);
	//freopen("../../../input_data/starc/boj6035_starc_out.txt", "w", stdout);
	std::cin >> N >> Q;
	HP = {
		Line({1, 0}, limit),
		Line({0, 1}, limit),
		Line({-1, 0}, -1 / limit),
		Line({0, -1}, -1 / limit),
		Line({1, -limit}, 0),
		Line({-limit, 1}, 0)
	};
	return;
}
void solve() {
	init();
	ld jx, jy, jz, bx, by, bz;
	for (int i = 0; i < N; i++) {
		std::cin >> JB >> jy >> jx >> jz >> by >> bx >> bz;
		Line hp = Line({ (jy - by), (jx - bx) }, bz - jz);
		if (JB == 'J') hp *= -1;
		HP.push_back(hp);
	}
	bool f = half_plane_intersection(HP, HPI);
	//std::cout << "f, HPI: " << f << " " << HPI.size() << "\n";
	//for (const Pos& p : HPI) std::cout << p; std::cout << "\n;
	LO = lower_hull(HPI);
	UP = upper_hull(HPI);
	while (Q--) query();
	return;
}
int main() { solve(); return 0; }//boj6034 StarCowraft


//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <cassert>
//#include <vector>
//#include <queue>
//#include <deque>
//#include <random>
//typedef long long ll;
////typedef long double ld;
//typedef double ld;
//const ld INF = 1e17;
//const ld TOL = 1e-7;
//const ld PI = acos(-1);
//const ld limit = 100;
//const int LEN = 1e3;
//int N, Q;
//char JB;
//bool zero(const ld& x) { return std::abs(x) < TOL; }
//int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
//int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
//ld norm(ld& th) {
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
//struct Seq { int x, y; Seq(int X = 0, int Y = 0) : x(X), y(Y) {} };
//std::vector<Seq> seq;
//
////2D============================================================================//
////2D============================================================================//
////2D============================================================================//
//struct Pos {
//	ld x, y;
//	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
//	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
//	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
//	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
//	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
//	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
//	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
//	Pos operator ~ () const { return { -y, x }; }
//	ld operator ! () const { return x * y; }
//	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
//	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
//	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
//	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
//	ld Euc() const { return x * x + y * y; }
//	ld mag() const { return hypot(x, y); }
//	friend std::istream& operator >> (std::istream& is, Pos& p) {
//		is >> p.x >> p.y;
//		return is;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
//		os << p.x << " " << p.y << "\n";
//		return os;
//	}
//};
//const Pos O = { 0, 0 };
//struct Vec {
//	ld vy, vx;
//	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
//	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
//	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
//	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
//	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
//	Vec operator ~ () const { return { -vx, vy }; }
//	Vec& operator *= (const ld& scalar) { vy *= scalar; vx *= scalar; return *this; }
//}; const Vec Zero = { 0, 0 };
//struct Line {
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
//	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
//	Line& operator *= (const ld& scalar) { s *= scalar; c *= scalar; return *this; }
//	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
//	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
//	friend std::ostream& operator << (std::ostream& os, const Line& l) {
//		os << l.s.vy << " " << l.s.vx << " " << l.c << "\n";
//		return os;
//	}
//};
//const Line X_axis = { { 0, -1 }, 0 };
//const Line Y_axis = { { 1, 0 }, 0 };
//Line L(const Pos& s, const Pos& e) {
//	ld dy, dx, c;
//	dy = e.y - s.y;
//	dx = s.x - e.x;
//	c = dy * s.x + dx * s.y;
//	return { { dy, dx } , c };
//}
////Line rotate90(const Line& l, const Pos& p) {
////	Vec s = ~l.s;
////	ld c = s.vy * p.x + s.vx * p.y;
////	return { s, c };
////}
//Pos intersection(const Line& l1, const Line& l2) {
//	Vec v1 = l1.s, v2 = l2.s;
//	ld det = v1 / v2;
//	return {
//		(l1.c * v2.vx - l2.c * v1.vx) / det,
//		(l2.c * v1.vy - l1.c * v2.vy) / det,
//	};
//}
//ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ld ret = cross(d1, d2, d3);
//	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
//}
//ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
//std::vector<Pos> lower_hull(std::vector<Pos>& C) {//lower monotone chain
//	std::vector<Pos> H;
//	std::sort(C.begin(), C.end());
//	if (C.size() <= 2) {
//		for (const Pos& pos : C) H.push_back(pos);
//		return H;
//	}
//	for (int i = 0; i < C.size(); i++) {
//		while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
//			H.pop_back();
//		H.push_back(C[i]);
//	}
//	return H;
//}
//std::vector<Pos> upper_hull(std::vector<Pos>& C) {//upper monotone_chain
//	std::vector<Pos> H;
//	//std::sort(C.begin(), C.end());
//	if (C.size() <= 2) {
//		for (int i = C.size() - 1; i >= 0; i--) H.push_back(C[i]);
//		return H;
//	}
//	for (int i = C.size() - 1; i >= 0; i--) {
//		while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
//			H.pop_back();
//		H.push_back(C[i]);
//	}
//	return H;
//}
//bool CW(const Line& l1, const Line& l2, const Line& target) {
//	if (l1.s / l2.s < TOL) return 0;
//	Pos p = intersection(l1, l2);
//	return target.s.vy * p.x + target.s.vx * p.y > target.c - TOL;
//	//return target.s.vy * p.x + target.s.vx * p.y > target.c;
//}
//bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& hull) {
//	std::deque<Line> dq;
//	std::sort(HP.begin(), HP.end());
//	for (const Line& l : HP) {
//		if (!dq.empty() && zero(dq.back() / l)) continue;
//		while (dq.size() >= 2 && CW(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
//		while (dq.size() >= 2 && CW(l, dq.front(), dq[1])) dq.pop_front();
//		dq.push_back(l);
//	}
//	while (dq.size() >= 3 && CW(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
//	while (dq.size() >= 3 && CW(dq.back(), dq.front(), dq[1])) dq.pop_front();
//	for (int i = 0; i < dq.size(); i++) {
//		Line cur = dq[i], nxt = dq[(i + 1) % dq.size()];
//		if (cur / nxt < TOL) {
//			hull.clear();
//			return 0;
//		}
//		hull.push_back(intersection(cur, nxt));
//	}
//	return 1;
//}
//std::vector<Line> HP;
//std::vector<Pos> HPI, LO, UP;
//Pos ternary_search(std::vector<Pos>& H, const Line& hp, bool f = 0) {
//	//std::cout << "l: " << hp;
//	int s = 0, e = H.size() - 1, l, r;
//	ld MAX = INF;
//	while (e - s > 2) {
//		l = (s + s + e) / 3;
//		r = (s + e + e) / 3;
//		//std::cout << "l  r  " << l << " " << r << "\n";
//		//std::cout << H[l] << H[r];
//		ld cl = hp.above(H[l]) * (f ? -1 : 1);
//		ld cr = hp.above(H[r]) * (f ? -1 : 1);
//		//std::cout << "cl cr  " << cl << " " << cr << "\n";
//		if (cl > cr) s = l;
//		else e = r;
//	}
//	Pos EX;
//	for (int i = s; i <= e; i++) {
//		ld D = hp.above(H[i]) * (f ? -1 : 1);
//		if (hp.above(H[i]) < MAX) {
//			MAX = D;
//			EX = H[i];
//		}
//	}
//	return EX;
//}
//void query() {
//	ld jx, jy, jz, bx, by, bz;
//	std::cin >> jy >> jx >> jz >> by >> bx >> bz;
//	Line l = Line({ (jy - by), (jx - bx) }, bz - jz);
//
//	//Line k = Y_axis;
//	//bool f = zero(k / l) ? k * l < 0 : k / l > 0;
//	//if (!f) l *= -1;
//	//Pos lo = ternary_search(LO, l);
//	//Pos hi = ternary_search(UP, l, 1);
//	//if (!f) l *= -1;
//	//ld d1 = l.above(lo);
//	//ld d2 = l.above(hi);
//	////std::cout << lo << hi;
//	////std::cout << "d1, d2 : " << d1 << " " << d2 << "\n";
//	//bool f1 = d1 > -TOL || d2 > -TOL;
//	//bool f2 = d1 < TOL || d2 < TOL;
//	//if (f1 && f2) std::cout << "U\n";
//	//else if (f1) std::cout << "J\n";
//	//else if (f2) std::cout << "B\n";
//
//	bool f1 = 0, f2 = 0;
//	for (int i = 0; i < HPI.size(); i++) {
//		if (l.above(HPI[i]) > -TOL) f1 = 1;
//		if (l.above(HPI[i]) < TOL) f2 = 1;
//	}
//	//std::cout << f1 << " " << f2 << "\n";
//	if (f1 && f2) std::cout << "U\n";
//	else if (f1) std::cout << "J\n";
//	else if (f2) std::cout << "B\n";
//	return;
//}
//void init() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	freopen("starc/starc.15.IN", "r", stdin);
//	freopen("starc/boj6035_starc_out.txt", "w", stdout);
//	std::cin >> N >> Q;
//	HP = {
//		Line({1, 0}, limit),
//		Line({0, 1}, limit),
//		Line({-1, 0}, -1 / limit),
//		Line({0, -1}, -1 / limit),
//		Line({1, -limit}, 0),
//		Line({-limit, 1}, 0)
//	};
//	return;
//}
//void solve() {
//	init();
//	//std::cout << HP.size() << "\n";
//	//bool x = half_plane_intersection(HP, HPI);
//	//std::cout << x << " " << HPI.size() << "\n";
//	//for (const Pos& p : HPI) std::cout << p;
//	//HPI.clear();
//	ld jx, jy, jz, bx, by, bz;
//	for (int i = 0; i < N; i++) {
//		std::cin >> JB >> jy >> jx >> jz >> by >> bx >> bz;
//		Line hp = Line({ (jy - by), (jx - bx) }, bz - jz);
//		if (JB == 'J') hp *= -1;
//		HP.push_back(hp);
//	}
//	bool f = half_plane_intersection(HP, HPI);
//	//for (const Pos& p : HPI) std::cout << p;
//	//std::cout << "f, HPI: " << f << " " << HPI.size() << "\n";
//	LO = lower_hull(HPI);
//	UP = upper_hull(HPI);
//	//std::cout << "LO\n";
//	//for (const Pos& p : LO) std::cout << p;
//	//std::cout << "HI\n";
//	//for (const Pos& p : UP) std::cout << p;
//	while (Q--) query();
//	return;
//}
//int main() { solve(); return 0; }