#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
#include <queue>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 100;//7 * 7 * 2 
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
bool zero(const ld& x) { return !sign(x); }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}

int N, M, T;
int pt;
bool V[LEN];
struct Info {
	int i;
	ld c;
	Info(int I = 0, ld co = 0) : i(I), c(co) {}
	bool operator < (const Info& x) const { return c > x.c; }
};
std::priority_queue<Info> Q;
std::vector<Info> G[LEN];
ld C[LEN];
ld dijkstra(int v, int e) {
	for (int i = 0; i < LEN; i++) C[i] = INF;
	Q.push({ v, 0 });
	C[v] = 0;
	while (!Q.empty()) {
		Info p = Q.top(); Q.pop();
		if (p.c > C[p.i]) continue;
		if (p.i == e) return C[e];
		for (const Info& w : G[p.i]) {
			ld cost = p.c + w.c;
			if (C[w.i] > cost) {
				C[w.i] = cost;
				Q.push(Info(w.i, cost));
			}
		}
	}
	return C[e];
}
struct Pos {
	ld x, y;
	int i;
	ld th;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) { i = 0; th = 0; }
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
	friend ld rad(const Pos& p1, const Pos& p2) { return norm(atan2l(p1 / p2, p1 * p2)); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} poles[10], nodes[LEN]; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
Polygon rev[10];
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ld ret = cross(d1, d2, d3, d4); return sign(ret); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3) / (d2 - d1).mag(); }
ld dist(const Pos& d1, const Pos& d2, const Pos& t, const int f = 0) {
	ld ret = cross(d1, d2, t) / (d1 - d2).mag();
	if (f == 1) return std::abs(ret);
	if (f == 2) {
		if (sign(dot(d1, d2, t)) <= 0 &&
			sign(dot(d2, d1, t)) <= 0)
			return std::abs(cross(d1, d2, t)) / (d1 - d2).mag();
		return std::min((d1 - t).mag(), (d2 - t).mag());
	}
	return ret;
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}

bool between(const Pos& d1, const Pos& d2, const Pos& target) {
	ld dot1 = dot(d1, d2, target), dot2 = dot(d2, d1, target);
	return ((zero(dot1) || dot1 < 0) && (zero(dot2) || dot2 < 0));
}
ld dist(const Pos& d1, const Pos& d2) {
	return hypot((d1.x - d2.x), (d1.y - d2.y));
}
//ld dist(const Pos& d1, const Pos& d2, const Pos& target) {
//	return std::abs(cross(d1, d2, target) / dist(d1, d2));
//}
bool close(const Pos& d, const ld& R, const Pos& target) {
	return dist(d, target) < R - TOL;
}
bool close(const Pos& d1, const Pos& d2, const ld& R, const Pos& target) {
	if (between(d1, d2, target)) return dist(d1, d2, target) < R - TOL;
	else return close(d1, R, target) || close(d2, R, target);
}
ld get_theta(const Pos& d1, const Pos& d2, const ld& R) {
	ld w = dist(d1, d2);
	return asin(R / w);
}
Pos rotate(const Pos& pivot, const Pos& p, ld theta, int i) {
	Pos v = p - pivot;
	ld ratio = cos(theta);
	ld x = v.x * cos(theta) - v.y * sin(theta);
	ld y = v.x * sin(theta) + v.y * cos(theta);
	return Pos(x, y) * ratio + pivot;
}
void connect_node(int n1, int n2, const ld& R) {
	Pos d1 = nodes[n1], d2 = nodes[n2];
	if (!d1.i || d1.i != d2.i) {
		for (int i = 1; i < N + 1; i++) if (close(d1, d2, R, poles[i])) return;
		G[n1].push_back({ n2, dist(d1, d2) });
		G[n2].push_back({ n1, dist(d1, d2) });
	}
	return;
}

std::vector<Pos> circle_line_intersection(const Pos& o, const ld& r, const Pos& p1, const Pos& p2) {
	ld d = dist(p1, p2, o);
	if (std::abs(d) > r) return {};
	Pos vec = p2 - p1;
	Pos m = intersection(p1, p2, o, o + ~vec);
	ld distance = vec.mag();
	ld ratio = sqrt(r * r - d * d);
	Pos m1 = m - vec * ratio / distance;
	Pos m2 = m + vec * ratio / distance;
	if (dot(p1, p2, m1, m2) < 0) std::swap(m1, m2);
	return { m1, m2 };//p1->p2
}
struct Circle {
	Pos c;
	int r;
	Circle(Pos C = Pos(0, 0), int R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator < (const Circle& q) const {
		ld dist = (c - q.c).mag();
		return r < q.r && dist + r < q.r + TOL;
	}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	ld A() const { return 1. * r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
} INVAL = { { 0, 0 }, -1 };
bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
std::vector<Circle> disks;
ld circle_cut(const Circle& c, const Pos& p1, const Pos& p2) {
	Pos v1 = p1 - c.c, v2 = p2 - c.c;
	ld r = c.r;
	std::vector<Pos> inx = circle_line_intersection(O, r, v1, v2);
	if (inx.empty()) return r * r * rad(v1, v2) * .5;
	Pos m1 = inx[0], m2 = inx[1];
	bool d1 = dot(m1, v1, m2) > -TOL, d2 = dot(m1, v2, m2) > -TOL;
	if (d1 && d2) return (v1 / v2) * .5;
	else if (d1) return (v1 / m2 + r * r * rad(m2, v2)) * .5;
	else if (d2) return (r * r * rad(v1, m1) + m1 / v2) * .5;
	else if (dot(v1, m1, v2) > 0 && dot(v1, m2, v2) > 0)
		return (r * r * (rad(v1, m1) + rad(m2, v2)) + m1 / m2) * .5;
	else return (r * r * rad(v1, v2)) * .5;
}
bool query() {
	std::cin >> N;
	if (!N) return 0;
	int r, R;
	ld I;
	Pos s, t;
	std::cin >> r >> R >> s >> t;
	assert(s.x * t.x < 0);
	if (s.x < 0) std::swap(s, t);
	s.i = 0;
	t.i = 1;
	pt = 2;
	ld the = PI / N;
	I = r / sin(the);
	//if (sign(I + r - R) > 0) { std::cout << "-1\n"; return 1; }
	if (sign(I + r - R) > 0) { std::cout << "DEBUG:: answer:: -1\n"; return 1; }
	Pos p1, p2, p3, p4;
	p1 = Pos(1, 0).rot(the * .5) * R;
	p2 = Pos(1, 0).rot(PI - the * .5) * R;
	p3 = Pos(1, 0).rot(PI + the * .5) * R;
	p4 = Pos(1, 0).rot(-the * .5) * R;
	if (sign(dist(s, t, O, 1) - I) >= 0 &&
		sign(dist(s, t, p1) - R) >= 0 &&
		sign(dist(s, t, p2) + R) <= 0 &&
		sign(dist(s, t, p3) + R) <= 0 &&
		sign(dist(s, t, p4) - R) >= 0) {
		//std::cout << (s - t).mag() << "\n";
		std::cout << "DEBUG:: direct:: " << (s - t).mag() << "\n";
		return 1;
	}

	Pos p11 = p1 + (p1 - O).unit() * r;
	p11.i = pt++;
	Pos p1_ = p1 + ~(p11 - p1).unit() * r;
	rev[1].push_back(p11);

	Pos p21 = p1 + (p2 - O).unit() * r;
	p21.i = pt++;
	Pos p2_ = p2 + ~(p21 - p2).unit() * -r;
	rev[2].push_back(p21);

	Pos p31 = p1 + (p3 - O).unit() * r;
	p31.i = pt++;
	rev[3].push_back(p31);
	Pos p3_ = p3 + ~(p31 - p3).unit() * r;

	Pos p41 = p1 + (p4 - O).unit() * r;
	p41.i = pt++;
	rev[4].push_back(p41);
	Pos p4_ = p4 + ~(p41 - p4).unit() * -r;

	if (sign(dist(s, p21, O) - R) >= 0 && sign(dist(s, p21, p3) - r) >= 0) {
		G[0].push_back(Info(p21.i, (s - p21).mag()));
		G[p21.i].push_back(Info(0, (s - p21).mag()));
	}
	if (sign(dist(s, p31, O) - R) >= 0 && sign(dist(s, p31, p2) - r) >= 0) {
		G[0].push_back(Info(p31.i, (s - p31).mag()));
		G[p31.i].push_back(Info(0, (s - p31).mag()));
	}
	if (sign(dist(t, p11, O) - R) >= 0 && sign(dist(t, p11, p4) - r) >= 0) {
		G[1].push_back(Info(p11.i, (t - p11).mag()));
		G[p11.i].push_back(Info(1, (t - p11).mag()));
	}
	if (sign(dist(t, p41, O) - R) >= 0 && sign(dist(t, p41, p1) - r) >= 0) {
		G[1].push_back(Info(p41.i, (t - p41).mag()));
		G[p41.i].push_back(Info(1, (t - p41).mag()));
	}

	ld w = R + r;
	ld a = (O - s).mag();
	ld the = std::min(1., std::max(-1., w / a));
	ld gam = acos(the);
	//s -> p2
	if (ccw(p2, p21, s) < 0 && sign(dist(p2, p2_, s) - r) < 0) {
		Pos p22 = Pos(R + r, 0).rot(s.rad() + gam);
		p22.i = pt++;
		G[0].push_back(Info(p22.i, (s - p22).mag()));
		G[p22.i].push_back(Info(0, (s - p22).mag()));
		G[p21.i].push_back(Info(p22.i, std::abs(rad(p21, p22)) * w));
		G[p22.i].push_back(Info(p21.i, std::abs(rad(p21, p22)) * w));
	}
	//s -> p3
	if (ccw(p3, p31, s) > 0 && sign(dist(p3_, p3, s) - r) < 0) {
		Pos p32 = Pos(R + r, 0).rot(s.rad() + gam);
		p32.i = pt++;
		G[0].push_back(Info(p32.i, (s - p32).mag()));
		G[p32.i].push_back(Info(0, (s - p32).mag()));
		G[p31.i].push_back(Info(p32.i, std::abs(rad(p31, p32)) * w));
		G[p32.i].push_back(Info(p31.i, std::abs(rad(p31, p32)) * w));
	}

	a = (O - t).mag();
	the = std::min(1., std::max(-1., w / a));
	gam = acos(the);
	//t -> p4
	if (ccw(p4, p41, t) < 0 && sign(dist(p4, p4_, t) - r) < 0) {
		Pos p42 = Pos(R + r, 0).rot(t.rad() + gam);
		p42.i = pt++;
		G[1].push_back(Info(p42.i, (t - p42).mag()));
		G[p42.i].push_back(Info(1, (t - p42).mag()));
		G[p41.i].push_back(Info(p42.i, std::abs(rad(p41, p42)) * w));
		G[p42.i].push_back(Info(p41.i, std::abs(rad(p41, p42)) * w));
	}
	//t -> p1
	if (ccw(p1, p11, t) > 0 && sign(dist(p1_, p1, t) - r) < 0) {
		Pos p12 = Pos(R + r, 0).rot(t.rad() + gam);
		p12.i = pt++;
		G[1].push_back(Info(p12.i, (t - p12).mag()));
		G[p12.i].push_back(Info(1, (t - p12).mag()));
		G[p11.i].push_back(Info(p12.i, std::abs(rad(p11, p12)) * w));
		G[p12.i].push_back(Info(p11.i, std::abs(rad(p11, p12)) * w));
	}

	//s -> p1, p2, p3, p4
	Pos vec;
	vec = s - p2;
	a = vec.mag();
	the = std::min(1., std::max(-1., r / a));
	gam = acos(the);
	Pos p23 = p2 + vec.unit().rot(gam) * r;
	if (sign(dist(s, p23, O, 2) - R) >= 0 && sign(dist(s, p23, p3, 2) - r) >= 0) {
		p23.i = pt++;
		G[0].push_back(Info(p23.i, (s - p23).mag()));
		G[p23.i].push_back(Info(0, (s - p23).mag()));
		rev[2].push_back(p23);
	}
	vec = s - p3;
	a = vec.mag();
	the = std::min(1., std::max(-1., r / a));
	gam = acos(the);
	Pos p33 = p3 + vec.unit().rot(-gam) * r;
	if (sign(dist(s, p33, O, 2) - R) >= 0 && sign(dist(s, p33, p2, 2) - r) >= 0) {
		p33.i = pt++;
		G[0].push_back(Info(p33.i, (s - p33).mag()));
		G[p33.i].push_back(Info(0, (s - p33).mag()));
		rev[3].push_back(p33);
	}
	vec = s - p4;
	a = vec.mag();
	the = std::min(1., std::max(-1., r / a));
	gam = acos(the);
	Pos p43 = p4 + vec.unit().rot(gam) * r;
	if (sign(dist(s, p43, O, 2) - I) >= 0 &&
		sign(dist(s, p43, p2, 2) - r) >= 0 &&
		sign(dist(s, p43, p3, 2) - r) >= 0) {
		p43.i = pt++;
		G[0].push_back(Info(p43.i, (s - p43).mag()));
		G[p43.i].push_back(Info(0, (s - p43).mag()));
		rev[4].push_back(p43);
	}
	vec = s - p1;
	a = vec.mag();
	the = std::min(1., std::max(-1., r / a));
	gam = acos(the);
	Pos p13 = p1 + vec.unit().rot(-gam) * r;
	if (sign(dist(s, p13, O, 2) - I) >= 0 &&
		sign(dist(s, p13, p2, 2) - r) >= 0 &&
		sign(dist(s, p13, p3, 2) - r) >= 0) {
		p13.i = pt++;
		G[0].push_back(Info(p13.i, (s - p13).mag()));
		G[p13.i].push_back(Info(0, (s - p13).mag()));
		rev[1].push_back(p13);
	}
	//s -> p1, p2, p3, p4

	//t -> p1, p2, p3, p4
	vec = t - p4;
	a = vec.mag();
	the = std::min(1., std::max(-1., r / a));
	gam = acos(the);
	Pos p44 = p4 + vec.unit().rot(gam) * r;
	if (sign(dist(t, p44, O, 2) - R) >= 0 && sign(dist(t, p44, p1, 2) - r) >= 0) {
		p44.i = pt++;
		G[1].push_back(Info(p44.i, (t - p44).mag()));
		G[p44.i].push_back(Info(1, (t - p44).mag()));
		rev[4].push_back(p44);
	}
	vec = t - p1;
	a = vec.mag();
	the = std::min(1., std::max(-1., r / a));
	gam = acos(the);
	Pos p14 = p1 + vec.unit().rot(-gam) * r;
	if (sign(dist(t, p14, O, 2) - R) >= 0 && sign(dist(t, p14, p4, 2) - r) >= 0) {
		p14.i = pt++;
		G[1].push_back(Info(p14.i, (t - p14).mag()));
		G[p14.i].push_back(Info(1, (t - p14).mag()));
		rev[1].push_back(p14);
	}
	vec = s - p2;
	a = vec.mag();
	the = std::min(1., std::max(-1., r / a));
	gam = acos(the);
	Pos p24 = p2 + vec.unit().rot(gam) * r;
	if (sign(dist(t, p24, O, 2) - I) >= 0 &&
		sign(dist(t, p24, p1, 2) - r) >= 0 &&
		sign(dist(t, p24, p4, 2) - r) >= 0) {
		p24.i = pt++;
		G[1].push_back(Info(p24.i, (t - p24).mag()));
		G[p24.i].push_back(Info(0, (t - p24).mag()));
		rev[2].push_back(p24);
	}
	vec = s - p3;
	a = vec.mag();
	the = std::min(1., std::max(-1., r / a));
	gam = acos(the);
	Pos p34 = p3 + vec.unit().rot(-gam) * r;
	if (sign(dist(t, p34, O, 2) - I) >= 0 &&
		sign(dist(t, p34, p1, 2) - r) >= 0 &&
		sign(dist(t, p34, p4, 2) - r) >= 0) {
		p34.i = pt++;
		G[1].push_back(Info(p34.i, (t - p34).mag()));
		G[p34.i].push_back(Info(0, (t - p34).mag()));
		rev[2].push_back(p34);
	}
	//t -> p1, p2, p3, p4

	//s -> O
	if (ccw(O, p2, s) > 0 && ccw(O, p3, s) < 0) {
		vec = s - O;
		a = vec.mag();
		the = std::min(1., std::max(-1., I / a));
		gam = acos(the);
		Pos p02 = O + vec.unit().rot(gam) * r;
		if (sign(dist(s, p02, p2, 2) - r) >= 0 && sign(dist(s, p02, p3, 2) - r) >= 0) {
			p02.i = pt++;
			G[0].push_back(Info(p02.i, (s - p02).mag()));
			G[p02.i].push_back(Info(0, (s - p02).mag()));
			rev[0].push_back(p02);
		}
		Pos p03 = O + vec.unit().rot(-gam) * r;
		if (sign(dist(s, p03, p2, 2) - r) >= 0 && sign(dist(s, p03, p3, 2) - r) >= 0) {
			p03.i = pt++;
			G[0].push_back(Info(p03.i, (s - p03).mag()));
			G[p03.i].push_back(Info(0, (s - p03).mag()));
			rev[0].push_back(p03);
		}
	}
	//s -> O
	 
	//t -> O
	if (ccw(O, p4, s) > 0 && ccw(O, p1, s) < 0) {
		vec = t - O;
		a = vec.mag();
		the = std::min(1., std::max(-1., I / a));
		gam = acos(the);
		Pos p04 = O + vec.unit().rot(gam) * r;
		if (sign(dist(s, p04, p4, 2) - r) >= 0 && sign(dist(s, p04, p1, 2) - r) >= 0) {
			p04.i = pt++;
			G[1].push_back(Info(p04.i, (t - p04).mag()));
			G[p04.i].push_back(Info(1, (t - p04).mag()));
			rev[0].push_back(p04);
		}
		Pos p01 = O + vec.unit().rot(-gam) * r;
		if (sign(dist(s, p01, p4, 2) - r) >= 0 && sign(dist(s, p01, p1, 2) - r) >= 0) {
			p01.i = pt++;
			G[1].push_back(Info(p01.i, (t - p01).mag()));
			G[p01.i].push_back(Info(1, (t - p01).mag()));
			rev[0].push_back(p01);
		}
	}
	//t -> O

	//Pos p24 = p2 + Pos(0, -r);
	//Pos p14 = p1 + Pos(0, -r);
	//if (sign(dist(p24, p14, O, 1) - I) >= 0) {
	//	p24.i = pt++;
	//	p14.i = pt++;
	//	rev[2].push_back(p24);
	//	rev[1].push_back(p14);
	//}
	//Pos p34 = p3 + Pos(0, r);
	//Pos p44 = p4 + Pos(0, r);
	//if (sign(dist(p34, p44, O, 1) - I) >= 0) {
	//	p34.i = pt++;
	//	p44.i = pt++;
	//	rev[3].push_back(p34);
	//	rev[4].push_back(p44);
	//}
	


	ld ans = dijkstra(0, 1);
	//std::cout << ans << "\n";
	std::cout << "DEBUG:: answer::" << ans << "\n";
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	while (query());
	return;
}
int main() { solve(); return 0; }
