//#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld TOL = 1e-8;
const ld INF = 1e17;
const int LEN = 101;
int N, T;

bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	int i;
	Pos(ld X = 0, ld Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	bool operator == (const Pos& p) const { return z(x - p.x) && z(y - p.y); }
	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y, 0 }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y, 0 }; }
	Pos operator * (const ld& n) const { return { x * n, y * n, 0 }; }
	Pos operator / (const ld& n) const { return { x / n, y / n, 0 }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x, 0 }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	ld mag() const { return hypot(x, y); }
}; const Pos O = { 0, 0, -1 };
std::vector<Pos> H, HPI;
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator < (const Vec& v) const { return z(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	bool operator == (const Vec& v) const { return (z(vy - v.vy) && z(vx - v.vx)); }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
}; const Vec Zero = { 0, 0 };
struct Line {
	Vec s;
	ld c;
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ld CCW = s / l.s;
		return z(CCW) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : CCW > 0;
	}
	ld operator / (const Line& l) const { return s / l.s; }
};
std::vector<Line> HP;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return z(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return z(cross(d1, d2, d3)) && (ret > 0 || z(ret));
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return z(cross(d1, d2, d3)) && ret > 0;
}
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return { { dy, dx } , c };
}
Line L_TOL(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y + hypot(dy, dx) * TOL;
	return { { dy, dx } , c };
}
//Line rotate90(const Line& l, const Pos& p) {
//	Vec s = ~l.s;
//	ld c = s.vy * p.x + s.vx * p.y;
//	return { s, c };
//}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
		0
	};
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
		if (!dq.empty() && z(dq.back() / l)) continue;
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
ld get_area(std::vector<Pos>& H) {
	int sz = H.size();
	ld a{ 0 };
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		a += cross(O, cur, nxt);
	}
	return a * .5;
}
bool inner_check(Pos H[], const int& sz, const Pos& p) {
	int cnt{ 0 };
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (z(cur.y - nxt.y)) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return cnt & 1;
}
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	return f1 && f2;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	//freopen("boj6740_E.in", "r", stdin);
	//freopen("boj6740_E_out.txt", "w", stdout);
	std::cin >> T;
	while (T--) {
		std::cin >> N;
		H.resize(N);
		for (int i = 0; i < N; i++) std::cin >> H[i].x >> H[i].y;
		if (get_area(H) < 0) std::reverse(H.begin(), H.end());
		HP.clear(); HPI.clear();
		int sz = H.size();
		for (int i = 0; i < sz; i++) {
			Pos cur = H[i], nxt = H[(i + 1) % sz];
			//HP.push_back(L(cur, nxt));
			HP.push_back(L_TOL(cur, nxt));
		}
		if (!half_plane_intersection(HP, HPI)) std::cout << "NO\n";
		else std::cout << "YES\n";
	}
	return;
}
int main() { solve(); return 0; }//boj6740

/*
//cpp98GCC
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
typedef long long ll;
typedef double ld;
const ld TOL = 1e-8;
const ld INF = 1e17;
const int LEN = 101;
int N, T;

bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
    ld x, y;
    int i;
    Pos(ld X = 0, ld Y = 0, int I = 0) : x(X), y(Y), i(I) {}
    bool operator == (const Pos& p) const { return z(x - p.x) && z(y - p.y); }
    bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
    Pos operator + (const Pos& p) const { return Pos(x + p.x, y + p.y, 0); }
    Pos operator - (const Pos& p) const { return Pos(x - p.x, y - p.y, 0); }
    Pos operator * (const ld& n) const { return Pos(x * n, y * n, 0); }
    Pos operator / (const ld& n) const { return Pos(x / n, y / n, 0); }
    ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
    ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
    Pos operator ~ () const { return Pos(-y, x, 0); }
    ld operator ! () const { return x * y; }
    Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
    Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
    ld mag() const { return hypot(x, y); }
};
const Pos O = Pos(0, 0, -1);

std::vector<Pos> H, HPI;

struct Vec {
    ld vy, vx;
    Vec(ld VY = 0, ld VX = 0) : vy(VY), vx(VX) {}
    bool operator < (const Vec& v) const { return z(vy - v.vy) ? vx < v.vx : vy < v.vy; }
    bool operator == (const Vec& v) const { return (z(vy - v.vy) && z(vx - v.vx)); }
    ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
    Vec operator ~ () const { return Vec(-vx, vy); }
};
const Vec Zero = Vec(0, 0);

struct Line {
    Vec s;
    ld c;
    Line(const Vec& S = Vec(), ld C = 0) : s(S), c(C) {}
    bool operator < (const Line& l) const {
        bool f1 = Zero < s;
        bool f2 = Zero < l.s;
        if (f1 != f2) return f1;
        ld CCW = s / l.s;
        return z(CCW) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : CCW > 0;
    }
    ld operator / (const Line& l) const { return s / l.s; }
};

std::vector<Line> HP;

ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }

int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
    ld ret = cross(d1, d2, d3);
    return z(ret) ? 0 : ret > 0 ? 1 : -1;
}

ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }

bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
    ld ret = dot(d1, d3, d2);
    return z(cross(d1, d2, d3)) && (ret > 0 || z(ret));
}

bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
    ld ret = dot(d1, d3, d2);
    return z(cross(d1, d2, d3)) && ret > 0;
}

Line L(const Pos& s, const Pos& e) {
    ld dy, dx, c;
    dy = e.y - s.y;
    dx = s.x - e.x;
    c = dy * s.x + dx * s.y;
    return Line(Vec(dy, dx), c);
}

Line L_TOL(const Pos& s, const Pos& e) {
    ld dy, dx, c;
    dy = e.y - s.y;
    dx = s.x - e.x;
    c = dy * s.x + dx * s.y + hypot(dy, dx) * TOL;
    return Line(Vec(dy, dx), c);
}

Pos intersection(const Line& l1, const Line& l2) {
    Vec v1 = l1.s, v2 = l2.s;
    ld det = v1 / v2;
    return Pos((l1.c * v2.vx - l2.c * v1.vx) / det, (l2.c * v1.vy - l1.c * v2.vy) / det, 0);
}

bool CW(const Line& l1, const Line& l2, const Line& target) {
    if (l1.s / l2.s < TOL) return 0;
    Pos p = intersection(l1, l2);
    return target.s.vy * p.x + target.s.vx * p.y > target.c - TOL;
}

bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& hull) {
    std::deque<Line> dq;
    std::sort(HP.begin(), HP.end());
    for (size_t i = 0; i < HP.size(); ++i) {
        const Line& l = HP[i];
        if (!dq.empty() && z(dq.back() / l)) continue;
        while (dq.size() >= 2 && CW(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
        while (dq.size() >= 2 && CW(l, dq.front(), dq[1])) dq.pop_front();
        dq.push_back(l);
    }
    while (dq.size() >= 3 && CW(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
    while (dq.size() >= 3 && CW(dq.back(), dq.front(), dq[1])) dq.pop_front();
    for (size_t i = 0; i < dq.size(); ++i) {
        const Line& cur = dq[i];
        const Line& nxt = dq[(i + 1) % dq.size()];
        if (cur / nxt < TOL) {
            hull.clear();
            return false;
        }
        hull.push_back(intersection(cur, nxt));
    }
    return true;
}

ld get_area(const std::vector<Pos>& H) {
    int sz = H.size();
    ld a = 0;
    for (int i = 0; i < sz; ++i) {
        const Pos& cur = H[i];
        const Pos& nxt = H[(i + 1) % sz];
        a += cross(O, cur, nxt);
    }
    return a * 0.5;
}

bool inner_check(const Pos H[], const int& sz, const Pos& p) {
    int cnt = 0;
    for (int i = 0; i < sz; ++i) {
        Pos cur = H[i];
        Pos nxt = H[(i + 1) % sz];
        if (on_seg_strong(cur, nxt, p)) return true;
        if (z(cur.y - nxt.y)) continue;
        if (nxt.y < cur.y) std::swap(cur, nxt);
        if (nxt.y - TOL < p.y || cur.y > p.y) continue;
        cnt += ccw(cur, nxt, p) > 0;
    }
    return cnt & 1;
}

bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
    bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
    bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
    return f1 && f2;
}

void init() {
    std::cin.tie(0);
    std::ios_base::sync_with_stdio(0);
    std::cout << std::fixed;
    std::cout.precision(7);
    std::cin >> T;
    while (T--) {
        std::cin >> N;
        H.resize(N);
        for (int i = 0; i < N; ++i) std::cin >> H[i].x >> H[i].y;
        if (get_area(H) < 0) std::reverse(H.begin(), H.end());
        HP.clear(); HPI.clear();
        int sz = H.size();
        for (int i = 0; i < sz; ++i) {
            const Pos& cur = H[i];
            const Pos& nxt = H[(i + 1) % sz];
            HP.push_back(L_TOL(cur, nxt));
        }
        if (!half_plane_intersection(HP, HPI)) std::cout << "NO\n";
        else std::cout << "YES\n";
    }
    return;
}

int main() {
    init();
    return 0;
}
*/