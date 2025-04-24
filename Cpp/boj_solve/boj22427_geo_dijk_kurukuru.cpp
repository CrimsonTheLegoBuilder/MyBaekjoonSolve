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
typedef long double ld;
//typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 300;
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
ld dijkstra(int v, int e, int pt) {
	for (int i = 0; i < LEN; i++) C[i] = INF;

	//for (int u = 0; u < pt; u++) {
	//	std::cout << "G[" << u + (u < 2 ? 1 : 5) << "]:: ";
	//	for (Info& ii : G[u]) std::cout << ii.i + (ii.i < 2 ? 1 : 5) << " ";
	//	std::cout << "\n";
	//}

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
	Pos s() const { return { y, -x }; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	Pos rot(ld the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} poles[10], nodes[LEN]; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
bool cmpt(const Pos& p, const Pos& q) { return p.th < q.th; }
Polygon rev[10];
Polygon node;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ld ret = cross(d1, d2, d3, d4); return sign(ret); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) > 0; }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3) / (d2 - d1).mag(); }
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	//return f1 && f2;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
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
void back_trace() {
	// fuck
	int source = 0;
	int sink = 1;
	int cur = sink;
	while (cur != source) {
		std::cout << cur + (cur < 2 ? 1 : 5) << ' ' << C[cur] << " coord: " << node[cur + +(cur < 2 ? 0 : 4)] << '\n';
		ld cur_cost = C[cur];
		for (const Info& w : G[cur]) {
			ld before_cost = C[w.i];
			if (before_cost + w.c > cur_cost) continue;
			else {
				cur = w.i;
				break;
			}
		}
	}
	std::cout << cur << "\n";
	std::cout << (node[11] - node[0]).mag() << "\n";
	// fuck
}
bool query() {
	for (int i = 0; i < LEN; i++) G[i].clear();
	for (int i = 0; i < 10; i++) rev[i].clear();
	while (Q.size()) Q.pop();
	std::cin >> N;
	if (!N) return 0;
	int r, R;
	ld I, the, gam, a, b, c, w;
	Pos s, t, m;
	std::cin >> r >> R >> s >> t;
	assert(s.x * t.x < 0);
	if (s.x > 0) std::swap(s, t);
	assert(s.x < 0 && t.x > 0);
	//std::cout << "DEBUG:: s:: " << s << " t:: " << t << "\n";
	s.i = 0;
	t.i = 1;
	pt = 2;
	the = PI / N, gam;
	I = r / sin(the * .5);

	//std::cout << "DEBUG:: the:: " << the * 180 / PI << " I:: " << I << "\n";

	if (sign(I + r - R) > 0) { std::cout << "-1\n"; return 1; }
	//if (sign(I + r - R) > 0) { std::cout << "DEBUG:: answer:: -1\n"; return 1; }
	assert(R >= r + I);

	Pos p1, p2, p3, p4;
	p1 = Pos(1, 0).rot(the * .5) * R;
	p2 = Pos(1, 0).rot(PI - the * .5) * R;
	p3 = Pos(1, 0).rot(PI + the * .5) * R;
	p4 = Pos(1, 0).rot(-the * .5) * R;

	//std::cout << (s - t).mag() << "\n";
	//std::cout << dist(s, t, O, 1) << "\n";
	//std::cout << I << "\n";
	//std::cout << r << "\n";
	//std::cout << dist(s, t, p1) << "\n";
	//std::cout << dist(s, t, p2) << "\n";
	//std::cout << dist(s, t, p3) << "\n";
	//std::cout << dist(s, t, p4) << "\n";

	if (sign(dist(s, t, O, 1) - I) >= 0 &&
		sign(dist(s, t, p1) - r) >= 0 &&
		sign(dist(s, t, p2) - r) >= 0 &&
		sign(dist(s, t, p3) + r) <= 0 &&
		sign(dist(s, t, p4) + r) <= 0) {
		std::cout << (s - t).mag() << "\n";
		//std::cout << "DEBUG:: direct:: " << (s - t).mag() << "\n";
		return 1;
	}

	Pos p1_, p2_, p3_, p4_, p11, p21, p31, p41, p1t, p2t, p3t, p4t;

	p11 = p1 + (p1 - O).unit() * r;
	p1t = p1 + (p1 - O).unit().s() * r;
	p1_ = p1 + ~(p11 - p1).unit() * r;
	p11.i = pt++;
	p1t.i = pt++;
	rev[1].push_back(p11);
	rev[1].push_back(p1t);

	p21 = p2 + (p2 - O).unit() * r;
	p2t = p2 + (p2 - O).unit().s() * -r;
	p2_ = p2 + ~(p21 - p2).unit() * -r;
	p21.i = pt++;
	p2t.i = pt++;
	rev[2].push_back(p21);
	rev[2].push_back(p2t);

	p31 = p3 + (p3 - O).unit() * r;
	p3t = p3 + (p3 - O).unit().s() * r;
	p3_ = p3 + ~(p31 - p3).unit() * r;
	p31.i = pt++;
	p3t.i = pt++;
	rev[3].push_back(p31);
	rev[3].push_back(p3t);

	p41 = p4 + (p4 - O).unit() * r;
	p4t = p4 + (p4 - O).unit().s() * -r;
	p4_ = p4 + ~(p41 - p4).unit() * -r;
	p41.i = pt++;
	p4t.i = pt++;
	rev[4].push_back(p41);
	rev[4].push_back(p4t);

	node = { s, t, p1, p2, p3, p4, p11, p1t, p21, p2t, p31, p3t, p41, p4t };

	//std::cout << "DEBUG:: 1:: p1 ::" << p1 << " p1_:: " << p1_ << "\n";
	//std::cout << "DEBUG:: 1:: vec:: p1_ - p1:: " << p1_ - p1 << "\n";
	//std::cout << "DEBUG:: 2:: p2 ::" << p2 << " p2_:: " << p2_ << "\n";
	//std::cout << "DEBUG:: 2:: vec:: p2_ - p2:: " << p2_ - p2 << "\n";
	//std::cout << "DEBUG:: 3:: p3 ::" << p3 << " p3_:: " << p31 << "\n";
	//std::cout << "DEBUG:: 3:: vec:: p31 - p3:: " << p31 - p3 << "\n";
	//std::cout << "DEBUG:: 4:: p4 ::" << p4 << " p4_:: " << p4_ << "\n";
	//std::cout << "DEBUG:: 4:: vec:: p4_ - p4:: " << p4_ - p4 << "\n";

	bool fs = 1, ft = 1;

	w = R + r;
	a = (O - s).mag();
	the = std::min((ld)1., std::max(-(ld)1., w / a));
	gam = acos(the);
	//std::cout << "DEBUG:: gam " << gam * 180 / PI << "\n";
	//s -> R
	Pos p22 = Pos(w, 0).rot(s.rad() + gam);
	if (ccw(p2, p21, s) < 0 &&
		sign(dist(p2, p2_, s) - r) < 0) {
		fs = 0;
		//std::cout << "DEBUG:: tangent2 " << " " << "\n";
		p22.i = pt++;
		node.push_back(p22);
		G[0].push_back(Info(p22.i, (s - p22).mag()));
		G[p22.i].push_back(Info(0, (s - p22).mag()));
		G[p21.i].push_back(Info(p22.i, std::abs(rad(p21, p22)) * w));
		G[p22.i].push_back(Info(p21.i, std::abs(rad(p21, p22)) * w));
		//std::cout << "DEBUG:: arc:: " << std::abs(rad(p21, p22)) * 180 / PI << "\n";
	}
	Pos p32 = Pos(w, 0).rot(s.rad() - gam);
	if (ccw(p3, p31, s) > 0 &&
		sign(dist(p3_, p3, s) - r) < 0) {
		fs = 0;
		//std::cout << "DEBUG:: tangent3 " << " " << "\n";
		//std::cout << "DEBUG:: tangent3 Pos s " << s << " p3 " << p3 << " p31 " << p31 << "\n";
		//std::cout << ccw(p3, p31, s) << " " << sign(dist(p3_, p3, s) - r) << "\n";
		p32.i = pt++;
		node.push_back(p32);
		G[0].push_back(Info(p32.i, (s - p32).mag()));
		G[p32.i].push_back(Info(0, (s - p32).mag()));
		G[p31.i].push_back(Info(p32.i, std::abs(rad(p31, p32)) * w));
		G[p32.i].push_back(Info(p31.i, std::abs(rad(p31, p32)) * w));
		//std::cout << "DEBUG:: arc:: " << std::abs(rad(p31, p32)) * 180 / PI << "\n";
	}

	a = (O - t).mag();
	the = std::min((ld)1., std::max(-(ld)1., w / a));
	gam = acos(the);
	//std::cout << "DEBUG:: gam " << gam * 180 / PI << "\n";
	//t -> R
	Pos p42 = Pos(w, 0).rot(t.rad() + gam);
	if (ccw(p4, p41, t) < 0 &&
		sign(dist(p4, p4_, t) - r) < 0) {
		ft = 0;
		//std::cout << "DEBUG:: tangent4 " << " " << "\n";
		p42.i = pt++;
		node.push_back(p42);
		G[1].push_back(Info(p42.i, (t - p42).mag()));
		G[p42.i].push_back(Info(1, (t - p42).mag()));
		G[p41.i].push_back(Info(p42.i, std::abs(rad(p41, p42)) * w));
		G[p42.i].push_back(Info(p41.i, std::abs(rad(p41, p42)) * w));
		//std::cout << "DEBUG:: arc:: " << std::abs(rad(p41, p42)) * 180 / PI << "\n";
	}
	Pos p12 = Pos(w, 0).rot(t.rad() - gam);
	if (ccw(p1, p11, t) > 0 &&
		sign(dist(p1_, p1, t) - r) < 0) {
		ft = 0;
		//std::cout << "DEBUG:: tangent1 " << " " << "\n";
		p12.i = pt++;
		node.push_back(p12);
		G[1].push_back(Info(p12.i, (t - p12).mag()));
		G[p12.i].push_back(Info(1, (t - p12).mag()));
		G[p11.i].push_back(Info(p12.i, std::abs(rad(p11, p12)) * w));
		G[p12.i].push_back(Info(p11.i, std::abs(rad(p11, p12)) * w));
		//std::cout << "DEBUG:: arc:: " << std::abs(rad(p11, p12)) * 180 / PI << "\n";
	}

	//std::cout << "DEBUG:: p12 :: " << p12 << " p22:: " << p22 << "\n";
	//std::cout << "DEBUG:: p32 :: " << p32 << " p42:: " << p42 << "\n";

	//s -> p1, p2, p3, p4
	Pos vec;
	vec = s - p2;
	a = vec.mag();
	the = std::min((ld)1., std::max(-(ld)1., r / a));
	gam = acos(the);
	Pos p23 = p2 + vec.unit().rot(gam) * r;
	//std::cout << "DEBUG:: p23 :: " << p23 << "\n";
	if (fs &&
		sign(dist(s, p23, p3, 2) - r) >= 0 &&
		(sign(dist(p3, p2, s) - r) >= 0 || sign(dist(p2, p2_, s) - r) >= 0)) {
		p23.i = pt++;
		node.push_back(p23);
		G[0].push_back(Info(p23.i, (s - p23).mag()));
		G[p23.i].push_back(Info(0, (s - p23).mag()));
		rev[2].push_back(p23);
		//std::cout << "p23:: " << p23 << "\n";
		//std::cout << p23.i << "\n";
	}
	vec = s - p3;
	a = vec.mag();
	the = std::min((ld)1., std::max(-(ld)1., r / a));
	gam = acos(the);
	Pos p33 = p3 + vec.unit().rot(-gam) * r;
	if (fs && 
		sign(dist(s, p33, p2, 2) - r) >= 0 &&
		(sign(dist(p3, p2, s) - r) >= 0 || sign(dist(p3_, p3, s) - r) >= 0)) {
		p33.i = pt++;
		node.push_back(p33);
		G[0].push_back(Info(p33.i, (s - p33).mag()));
		G[p33.i].push_back(Info(0, (s - p33).mag()));
		rev[3].push_back(p33);
	}
	vec = s - p4;
	a = vec.mag();
	the = std::min((ld)1., std::max(-(ld)1., r / a));
	gam = acos(the);
	Pos p43 = p4 + vec.unit().rot(-gam) * r;
	if (fs && (sign(dist(s, p43, O, 2) - I) >= 0 &&
		sign(dist(s, p43, p2) - r) >= 0 &&
		sign(dist(s, p43, p3) + r) <= 0)) {
		p43.i = pt++;
		node.push_back(p43);
		G[0].push_back(Info(p43.i, (s - p43).mag()));
		G[p43.i].push_back(Info(0, (s - p43).mag()));
		rev[4].push_back(p43);
	}
	vec = s - p1;
	a = vec.mag();
	the = std::min((ld)1., std::max(-(ld)1., r / a));
	gam = acos(the);
	Pos p13 = p1 + vec.unit().rot(gam) * r;
	if (fs && (sign(dist(s, p13, O, 2) - I) >= 0 &&
		sign(dist(s, p13, p2) - r) >= 0 &&
		sign(dist(s, p13, p3) + r) <= 0)) {
		p13.i = pt++;
		node.push_back(p13);
		G[0].push_back(Info(p13.i, (s - p13).mag()));
		G[p13.i].push_back(Info(0, (s - p13).mag()));
		rev[1].push_back(p13);
	}
	//s -> p1, p2, p3, p4

	//t -> p1, p2, p3, p4
	vec = t - p4;
	a = vec.mag();
	the = std::min((ld)1., std::max(-(ld)1., r / a));
	gam = acos(the);
	Pos p44 = p4 + vec.unit().rot(gam) * r;
	if (ft && 
		sign(dist(p44, t, p1, 2) - r) >= 0 &&
		(sign(dist(p1, p4, t) - r) >= 0 || sign(dist(p4, p4_, t) - r) >= 0)) {
		p44.i = pt++;
		node.push_back(p44);
		G[1].push_back(Info(p44.i, (t - p44).mag()));
		G[p44.i].push_back(Info(1, (t - p44).mag()));
		rev[4].push_back(p44);
	}
	vec = t - p1;
	a = vec.mag();
	the = std::min((ld)1., std::max(-(ld)1., r / a));
	gam = acos(the);
	Pos p14 = p1 + vec.unit().rot(-gam) * r;
	if (ft && 
		sign(dist(p14, t, p4, 2) - r) >= 0 &&
		(sign(dist(p1, p4, t) - r) >= 0 || sign(dist(p1_, p1, t) - r) >= 0)) {
		p14.i = pt++;
		node.push_back(p14);
		G[1].push_back(Info(p14.i, (t - p14).mag()));
		G[p14.i].push_back(Info(1, (t - p14).mag()));
		rev[1].push_back(p14);
	}
	vec = t - p2;
	a = vec.mag();
	the = std::min((ld)1., std::max(-(ld)1., r / a));
	gam = acos(the);
	Pos p24 = p2 + vec.unit().rot(-gam) * r;
	if (ft && (sign(dist(t, p24, O, 2) - I) >= 0 &&
		sign(dist(p24, t, p1) - r) >= 0 &&
		sign(dist(p24, t, p4) + r) <= 0)) {
		p24.i = pt++;
		node.push_back(p24);
		G[1].push_back(Info(p24.i, (t - p24).mag()));
		G[p24.i].push_back(Info(1, (t - p24).mag()));
		rev[2].push_back(p24);
	}
	vec = t - p3;
	a = vec.mag();
	the = std::min((ld)1., std::max(-(ld)1., r / a));
	gam = acos(the);
	Pos p34 = p3 + vec.unit().rot(gam) * r;
	if (ft && (sign(dist(t, p34, O, 2) - I) >= 0 &&
		sign(dist(p34, t, p1) - r) >= 0 &&
		sign(dist(p34, t, p4) + r) <= 0)) {
		p34.i = pt++;
		node.push_back(p34);
		G[1].push_back(Info(p34.i, (t - p34).mag()));
		G[p34.i].push_back(Info(1, (t - p34).mag()));
		rev[3].push_back(p34);
	}
	//t -> p1, p2, p3, p4

	//s -> O
	vec = s - O;
	a = vec.mag();
	the = std::min((ld)1., std::max(-(ld)1., I / a));
	gam = acos(the);
	Pos p02 = O + vec.unit().rot(gam) * I;
	if (fs && (sign(dist(s, p02, p2) - r) >= 0 && sign(dist(s, p02, p3) + r) <= 0)) {
		//std::cout << "DEBUG:: so2\nso2:: " << p02 << "\n";
		p02.i = pt++;
		node.push_back(p02);
		G[0].push_back(Info(p02.i, (s - p02).mag()));
		G[p02.i].push_back(Info(0, (s - p02).mag()));
		rev[0].push_back(p02);
	}
	Pos p03 = O + vec.unit().rot(-gam) * I;
	if (fs && (sign(dist(s, p03, p2) - r) >= 0 && sign(dist(s, p03, p3) + r) <= 0)) {
		//std::cout << "DEBUG:: so3\nso3:: " << p03 << "\n";
		p03.i = pt++;
		node.push_back(p03);
		G[0].push_back(Info(p03.i, (s - p03).mag()));
		G[p03.i].push_back(Info(0, (s - p03).mag()));
		rev[0].push_back(p03);
	}
	//s -> O

	//t -> O
	vec = t - O;
	a = vec.mag();
	the = std::min((ld)1., std::max(-(ld)1., I / a));
	gam = acos(the);
	Pos p04 = O + vec.unit().rot(gam) * I;
	if (ft && (sign(dist(p04, t, p1) - r) >= 0 && sign(dist(p04, t, p4) + r) <= 0)) {
		//std::cout << "DEBUG:: to4\nto4:: " << p04 << "\n";
		p04.i = pt++;
		node.push_back(p04);
		G[1].push_back(Info(p04.i, (t - p04).mag()));
		G[p04.i].push_back(Info(1, (t - p04).mag()));
		rev[0].push_back(p04);
	}
	Pos p01 = O + vec.unit().rot(-gam) * I;
	if (ft && (sign(dist(p01, t, p1) - r) >= 0 && sign(dist(p01, t, p4) + r) <= 0)) {
		//std::cout << "DEBUG:: to1\nto1:: " << p01 << "\n";
		p01.i = pt++;
		node.push_back(p01);
		G[1].push_back(Info(p01.i, (t - p01).mag()));
		G[p01.i].push_back(Info(1, (t - p01).mag()));
		rev[0].push_back(p01);
	}
	//t -> O

	//p2 - p1
	Pos p25 = p2 + Pos(0, -r);
	Pos p15 = p1 + Pos(0, -r);
	if (sign(dist(p25, p15, O, 1) - I) >= 0) {
		p25.i = pt++;
		p15.i = pt++;
		node.push_back(p25);
		node.push_back(p15);
		rev[2].push_back(p25);
		rev[1].push_back(p15);
		G[p25.i].push_back(Info(p15.i, (p25 - p15).mag()));
		G[p15.i].push_back(Info(p25.i, (p25 - p15).mag()));
	}
	//p3 - p4
	Pos p35 = p3 + Pos(0, r);
	Pos p45 = p4 + Pos(0, r);
	if (sign(dist(p35, p45, O, 1) - I) >= 0) {
		p35.i = pt++;
		p45.i = pt++;
		node.push_back(p35);
		node.push_back(p45);
		rev[3].push_back(p35);
		rev[4].push_back(p45);
		G[p35.i].push_back(Info(p45.i, (p35 - p45).mag()));
		G[p45.i].push_back(Info(p35.i, (p35 - p45).mag()));
	}
	
	Pos p20, p30, p10, p40;
	Pos p26, p36, p46, p16, p00;
	Vint v = { 1, -1 };

	//p2 - p3
	for (const int& i : v) {
		vec = p2 - p3;
		a = vec.mag();
		b = r * 2;
		c = sqrt(a * a - b * b);
		the = std::min((ld)1., std::max(-(ld)1., b / a));
		gam = acos(the);
		vec = vec.unit().rot(gam * i);
		p30 = p3 + vec * r;
		p20 = p2 - vec * r;
		p30.i = pt++;
		p20.i = pt++;
		node.push_back(p30);
		node.push_back(p20);
		rev[3].push_back(p30);
		rev[2].push_back(p20);
		G[p30.i].push_back(Info(p20.i, c));
		G[p20.i].push_back(Info(p30.i, c));
	}

	//p1 - p4
	for (const int& i : v) {
		vec = p1 - p4;
		a = vec.mag();
		b = r * 2;
		c = sqrt(a * a - b * b);
		the = std::min((ld)1., std::max(-(ld)1., b / a));
		gam = acos(the);
		vec = vec.unit().rot(gam * i);
		p40 = p4 + vec * r;
		p10 = p1 - vec * r;
		p40.i = pt++;
		p10.i = pt++;
		node.push_back(p40);
		node.push_back(p10);
		rev[4].push_back(p40);
		rev[1].push_back(p10);
		G[p40.i].push_back(Info(p10.i, c));
		G[p10.i].push_back(Info(p40.i, c));
	}

	//p2 - O
	for (const int& i : v) {
		vec = p2 - O;
		a = vec.mag();
		b = I + r * i;
		c = sqrt(a * a - b * b);
		the = std::min((ld)1., std::max(-(ld)1., b / a));
		gam = acos(the);
		vec = vec.unit().rot(-gam * i);
		p00 = vec * I;
		p26 = p00 + ~vec * i * c;
		if (sign(dist(p26, p00, p3, 2) - r) < 0) continue;
		p00.i = pt++;
		p26.i = pt++;
		node.push_back(p00);
		node.push_back(p26);
		rev[0].push_back(p00);
		rev[2].push_back(p26);
		G[p00.i].push_back(Info(p26.i, c));
		G[p26.i].push_back(Info(p00.i, c));
	}

	//p3 - O
	for (const int& i : v) {
		vec = p3 - O;
		a = vec.mag();
		b = I + r * i;
		c = sqrt(a * a - b * b);
		the = std::min((ld)1., std::max(-(ld)1., b / a));
		gam = acos(the);
		vec = vec.unit().rot(gam * i);
		p00 = vec * I;
		p36 = p00 + vec.s() * i * c;
		if (sign(dist(p36, p00, p2, 2) - r) < 0) continue;
		p00.i = pt++;
		p36.i = pt++;
		node.push_back(p00);
		node.push_back(p36);
		rev[0].push_back(p00);
		rev[3].push_back(p36);
		G[p00.i].push_back(Info(p36.i, c));
		G[p36.i].push_back(Info(p00.i, c));
	}

	//p4 - O
	for (const int& i : v) {
		vec = p4 - O;
		a = vec.mag();
		b = I + r * i;
		c = sqrt(a * a - b * b);
		the = std::min((ld)1., std::max(-(ld)1., b / a));
		gam = acos(the);
		vec = vec.unit().rot(-gam * i);
		p00 = vec * I;
		p46 = p00 + ~vec * i * c;
		if (sign(dist(p00, p46, p1, 2) - r) < 0) continue;
		p00.i = pt++;
		p46.i = pt++;
		node.push_back(p00);
		node.push_back(p46);
		rev[0].push_back(p00);
		rev[4].push_back(p46);
		G[p00.i].push_back(Info(p46.i, c));
		G[p46.i].push_back(Info(p00.i, c));
	}

	//p1 - O
	for (const int& i : v) {
		vec = p1 - O;
		a = vec.mag();
		b = I + r * i;
		c = sqrt(a * a - b * b);
		the = std::min((ld)1., std::max(-(ld)1., b / a));
		gam = acos(the);
		vec = vec.unit().rot(gam * i);
		p00 = vec * I;
		p16 = p00 + vec.s() * i * c;
		if (sign(dist(p00, p16, p4, 2) - r) < 0) continue;
		p00.i = pt++;
		p16.i = pt++;
		node.push_back(p00);
		node.push_back(p16);
		rev[0].push_back(p00);
		rev[1].push_back(p16);
		G[p00.i].push_back(Info(p16.i, c));
		G[p16.i].push_back(Info(p00.i, c));
	}
	
	Polygon pivot = { O, p1, p2, p3, p4 };
	for (int i = 0; i < 5; i++) {
		for (Pos& p : rev[i]) p.th = (p - pivot[i]).rad();
		std::sort(rev[i].begin(), rev[i].end(), cmpt);
	}

	int sz;
	Polygon& r0 = rev[0];
	sz = r0.size();
	for (int i = 0; i < sz; i++) {
		Pos& q1 = r0[i], & q2 = r0[(i + 1) % sz];
		Pos v1 = r0[i] - O, v2 = r0[(i + 1) % sz] - O;
		c = std::abs(rad(v1, v2)) * I;
		G[q1.i].push_back(Info(q2.i, c));
		G[q2.i].push_back(Info(q1.i, c));
	}
	Polygon& r1 = rev[1];
	sz = r1.size();
	for (int i = 0; i < sz; i++) {
		Pos& q1 = r1[i], & q2 = r1[(i + 1) % sz];
		Pos v1 = r1[i] - p1, v2 = r1[(i + 1) % sz] - p1;
		if (intersect(p1, p1_, q1, q2)) continue;
		c = std::abs(rad(v1, v2)) * r;
		G[q1.i].push_back(Info(q2.i, c));
		G[q2.i].push_back(Info(q1.i, c));
	}
	Polygon& r2 = rev[2];
	sz = r2.size();
	for (int i = 0; i < sz; i++) {
		Pos& q1 = r2[i], & q2 = r2[(i + 1) % sz];
		Pos v1 = r2[i] - p2 , v2 = r2[(i + 1) % sz] - p2;
		if (intersect(p2, p2_, q1, q2)) continue;
		c = std::abs(rad(v1, v2)) * r;
		G[q1.i].push_back(Info(q2.i, c));
		G[q2.i].push_back(Info(q1.i, c));
	}
	Polygon& r3 = rev[3];
	sz = r3.size();
	for (int i = 0; i < sz; i++) {
		Pos& q1 = r3[i], & q2 = r3[(i + 1) % sz];
		Pos v1 = r3[i] - p3, v2 = r3[(i + 1) % sz] - p3;
		if (intersect(p3, p3_, q1, q2)) continue;
		c = std::abs(rad(v1, v2)) * r;
		G[q1.i].push_back(Info(q2.i, c));
		G[q2.i].push_back(Info(q1.i, c));
	}
	Polygon& r4 = rev[4];
	sz = r4.size();
	for (int i = 0; i < sz; i++) {
		Pos& q1 = r4[i], & q2 = r4[(i + 1) % sz];
		Pos v1 = r4[i] - p4, v2 = r4[(i + 1) % sz] - p4;
		if (intersect(p4, p4_, q1, q2)) continue;
		c = std::abs(rad(v1, v2)) * r;
		G[q1.i].push_back(Info(q2.i, c));
		G[q2.i].push_back(Info(q1.i, c));
	}

	//for (int i = 0; i < pt; i++) {
	//	std::cout << "DEBUG:: G[" << i << "]\n";
	//	for (Info& ii : G[i]) std::cout << "(" << ii.i << " " << ii.c << "\n";
	//}
	//std::cout << "points = [\n";
	//for (Pos& p : node) std::cout << "(" << p.x << ", " << p.y << "),\n";
	//std::cout << "]\n";
	
	ld ans = dijkstra(0, 1, pt);
	std::cout << ans << "\n";
	//std::cout << "DEBUG:: answer::" << ans << "\n";

	//back_trace();

	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	//freopen("door2.in", "r", stdin);
	//freopen("door2_my.out", "w", stdout);
	
	while (query());
	return;
}
int main() { solve(); return 0; }//boj22427 JAG Domestic Contest 2015 G Kuru Kuru Door

/*

7
1 8
-78 -822
645 418
0

1604.6449978126

2
1 3
-9 -9
3 3
0

18.5182185793


2
1 10
-2 11
12 0
0

29.0480784147

2
1 10
-12 1
12 0
0

2
1 10
-12 -1
12 0
0

2
1 10
-12 0
12 1
0

24.0905866413
24.0905866413

10
1 8
-652 -810
666 326
0

6
1 10
-390 371
99 393
0

2
1 10
-521 2
512 3
0

*/