#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <deque>
typedef long long ll;
typedef long double ld;
//typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-12;
const ld EPS = 1e-7;
const ld limit = 1e4 + 1;
const ll SCALE = 1;
const int LEN = 100;
int N;
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
}; const Pos O = { 0, 0 };
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	Vec operator ~ () const { return { -vx, vy }; }
	Vec& operator *= (const ld& scalar) { vy *= scalar; vx *= scalar; return *this; }
	Vec& operator /= (const ld& scalar) { vy /= scalar; vx /= scalar; return *this; }
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
	Line& operator += (const ld& scalar) {
		ld tol = hypot(s.vy, s.vx) * scalar;
		c += tol;
		return *this;
	}
	Line& operator -= (const ld& scalar) {
		ld tol = hypot(s.vy, s.vx) * scalar;
		c -= tol;
		return *this;
	}
	Line& operator *= (const ld& scalar) { s *= scalar; c *= scalar; return *this; }
	Line& operator /= (const ld& scalar) { s /= scalar; c /= scalar; return *this; }
	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	friend std::ostream& operator << (std::ostream& os, const Line& l) {
		os << l.s.vy << " " << l.s.vx << " " << l.c << "\n";
		return os;
	}
};
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
std::vector<Pos> HPI;
struct Vel {
	ll v, u, w;
	bool operator == (const Vel& s) { return v == s.v && u == s.u && w == s.w; }
	bool operator <= (const Vel& s) { return v <= s.v && u <= s.u && w <= s.w; }
	bool operator >= (const Vel& s) { return v >= s.v && u >= s.u && w >= s.w; }
	Vel& operator *= (const ld& ratio) {
		v *= ratio; u *= ratio; w*= ratio;
		return *this;
	}
	Line L(const Vel& s, bool f = 0) const {
		ld vy = (v - s.v) * SCALE / (ld)(v * s.v);
		ld vx = (u - s.u) * SCALE / (ld)(u * s.u);
		ld c = (s.w - w) * SCALE / (ld)(w * s.w);
		if (f) vy *= -1, vx *= -1, c *= -1;
		return Line(Vec(vy, vx), c);
	}
	friend std::istream& operator >> (std::istream& is, Vel& s) {
		is >> s.v >> s.u >> s.w;
		return is;
	}
} seq[LEN];
bool init(const int& i) {
	HP.clear();
	HP = {
		Line({1, 0}, limit),
		Line({0, 1}, limit),
		Line({-1, 0}, 0),
		Line({0, -1}, 0)
	};
	for (int j = 0; j < N; j++) {
		if (j == i) continue;
		if (seq[i] <= seq[j]) { HP.clear(); return 0; }
		if (seq[i] >= seq[j]) continue;
		if (j != i) HP.push_back(seq[i].L(seq[j], 1));
	}
	for (Line& l : HP) l += EPS;
	HPI.clear();
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("../../../input_data/triathlon_tests/triath.20", "r", stdin);
	//freopen("../../../input_data/triathlon_tests/triathlon_out.txt", "w", stdout);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> seq[i];
	for (int i = 0; i < N; i++) {
		bool f = 0;
		if (init(i)) f = half_plane_intersection(HP, HPI);
		std::cout << (f ? "Yes\n" : "No\n");
	}
	return;
}
int main() { solve(); return 0; }//boj7427 Triathlon



//bool init(const int& i) {
//	HP.clear();
//	HP = {
//		Line({1, 0}, limit),
//		Line({0, 1}, limit),
//		Line({-1, 0}, 0),
//		Line({0, -1}, 0)
//	};
//	for (int j = 0; j < N; j++) {
//		if (j == i) continue;
//		if (seq[i] <= seq[j]) { HP.clear(); return 0; }
//		if (seq[i] >= seq[j]) { continue; }
//		if (j != i) HP.push_back(seq[i].L(seq[j], 1));
//		//std::cout << "DEBUG " << j << " : ";
//		//std::cout << seq[i].L(seq[j], 1);
//	}
//	//std::cout << "DEBUG Line\n";
//	//for (const Line& l : HP) std::cout << l;
//	//std::cout << "DEBUG Line\n\n";
//	for (Line& l : HP) l += EPS;
//	//std::cout << "DEBUG Line\n";
//	//for (const Line& l : HP) std::cout << l;
//	//std::cout << "DEBUG Line\n\n";
//	HPI.clear();
//	return 1;
//}
//void init(const int& i) {
//	//HP.clear();
// 	//HP = {
	//	Line({1, 0}, limit),
	//	Line({0, 1}, limit),
	//	Line({-1, 0}, -1 / limit),
	//	Line({0, -1}, -1 / limit),
	//	Line({1, -limit}, 0),
	//	Line({-limit, 1}, 0)
	//};
//	//HP = {
//	//	Line({1, 0}, limit),
//	//	Line({0, 1}, limit),
//	//	Line({-1, 0}, -1 / limit),
//	//	Line({0, -1}, -1 / limit),
//	//	Line({1, -limit}, 0),
//	//	Line({-limit, 1}, 0)
//	//};
//	//HP = {
//	//Line({1, 0}, limit + EPS),
//	//Line({0, 1}, limit + EPS),
//	//Line({-1, 0}, -1 / limit + EPS),
//	//Line({0, -1}, -1 / limit + EPS),
//	//Line({1, -limit}, 0 + hypot(1, limit) * EPS),
//	//Line({-limit, 1}, 0 + hypot(1, limit) * EPS)
//	//};
//	HP = {
//		Line({1, 0}, limit - EPS),
//		Line({0, 1}, limit - EPS),
//		Line({-1, 0}, -1 / limit - EPS),
//		Line({0, -1}, -1 / limit - EPS),
//		Line({1, -limit}, 0 - hypot(1, limit) * EPS),
//		Line({-limit, 1}, 0 - hypot(1, limit) * EPS)
//	};
//	for (int j = 0; j < N; j++) if (j != i) HP.push_back(seq[i].LD(seq[j], 1));
//	//std::cout << "DEBUG Line\n";
//	//for (const Line& l : HP) std::cout << l;
//	//std::cout << "DEBUG Line\n";
//	HPI.clear();
//	return;
//}
