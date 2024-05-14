#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
typedef long long ll;
typedef double ld;
//typedef long double ld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 25;
int N, M, T, Q;
ld A;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
ld norm(ld th) {
	while (th < 0) th += PI * 2;
	while (th > PI * 2 - TOL) th -= PI * 2;
	return th;
}

//#define DEBUG
//#define ASSERT

struct Pos {
	ld x, y;
	ld t;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) { t = norm(atan2(y, x)); }
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld xy() const { return x * y; }
	Pos rot(ld the) const { return Pos(x * cos(the) - y * sin(the), x * sin(the) + y * cos(the)); }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	//ld mag() const { return hypotl(x, y); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return norm(atan2l(y, x)); }
	friend ld rad(const Pos& p1, const Pos& p2) { return norm(atan2l(p1 / p2, p1 * p2)); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
bool cmpt(const Pos& p, const Pos& q) { return p.t < q.t; }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
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
};
bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
struct Arc {
	ld lo, hi;// [lo, hi] - radian range of arc, 0 ~ 2pi
	Circle cen;
	Arc(ld LO = 0, ld HI = 0, Circle CEN = Circle(Pos(0, 0), 0)) : lo(LO), hi(HI), cen(CEN) {}
	bool operator < (const Arc& a) const {
		return !sign(hi - a.hi) ?
			lo < a.lo :
			hi < a.hi;
	}
	ld area() const { return (hi - lo) * cen.r * cen.r; }
	friend std::ostream& operator << (std::ostream& os, const Arc& l) { os << l.lo << " " << l.hi << " " << l.cen.r; return os; }
};
std::vector<Arc> arcs[LEN], valid_arcs[LEN];
Circle disks[LEN];
struct Seg {
	Pos s, e;
	int i;
	Seg(Pos x, Pos y, int I = 0) : s(x), e(y), i(I) {}
	bool operator < (const Seg& S) const {
		return
			!sign((s - S.s).Euc()) ?
			!sign((e - S.e).Euc()) ?
			i < S.i :
			e < S.e :
			s < S.s;
	}
	friend std::ostream& operator << (std::ostream& os, const Seg& S) {
		os << "Seg[" << S.i + 1 << "] :: Seg.s : " << S.s << " Seg.e : " << S.e;
		return os;
	}
	bool connectable(const Seg& S) const {
		if (s == S.e && e == S.s) return 0;
		return s == S.e || e == S.s;
	}
};
Seg make_seg(const ld& lo, const ld& hi, const Circle& c, const int& i) {
	Pos LO = Pos(1, 0).rot(lo) * c.r + c.c;
	Pos HI = Pos(1, 0).rot(hi) * c.r + c.c;
	return Seg(LO, HI, i);
}
typedef std::vector<Seg> Segs;
void query() {
	for (int i = 0; i < LEN; i++)
		arcs[i].clear(), valid_arcs[i].clear();
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> disks[i];
	if (N <= 2) {
		std::cout << "0\n";
		return;
	}
	Segs segs;
	for (int i = 0; i < N; i++) {
		Circle& disk = disks[i];
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			Pos& ca = disks[i].c, cb = disks[j].c;
			ll ra = disks[i].r, rb = disks[j].r;
			Pos vec = cb - ca;//vec a -> b
			ld distance = vec.mag();
			ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance);
			if (X < 0 || X > ra) continue;
			Pos w = vec * X / distance;
			ld ratio = sqrt(ra * ra - X * X);
			Pos h = ~vec.unit() * ratio;
			Pos LO = w - h;
			Pos HI = w + h;
			ld lo = LO.rad();
			ld hi = HI.rad();
			Arc a1, a2;
			if (lo <= hi) {
				a1 = Arc(lo, hi, disk);
				arcs[i].push_back(a1);
			}
			else {
				a1 = Arc(lo, PI * 2, disk);
				a2 = Arc(0, hi, disk);
				arcs[i].push_back(a1);
				arcs[i].push_back(a2);
			}
		}
		std::sort(arcs[i].begin(), arcs[i].end());
		int sza = arcs[i].size();
		//for (Arc& a : arcs[i]) {//sweep circle
		for (int k  = 0; k < sza; k++) {
			Arc& a = arcs[i][k];
			ld lo = a.lo;
			ld hi = a.hi;
			while (valid_arcs[i].size()) {
				if (sign(valid_arcs[i].back().hi - a.lo) <= 0) break;
				else {
					lo = valid_arcs[i].back().hi;
					break;
				}
			}
			valid_arcs[i].push_back(Arc(lo, hi, disks[i]));
		}
		int sz = valid_arcs[i].size();
		for (int k = 0; k < sz; k++) {
			Arc& cur = valid_arcs[i][k], nxt = valid_arcs[i][(k + 1) % sz];
			//std::cout << norm(cur.hi) - norm(nxt.lo) << " norm rad\n";
			//std::cout << cur.hi << " " << nxt.lo << " rad\n";
			//std::cout << norm(cur.hi) << " " << norm(nxt.lo) << " rad\n";
			segs.push_back(make_seg(cur.hi, nxt.lo, disk, i));
		}
	}

	int sz = segs.size();
	T = 0;
	int cnt = 0;
	bool f = 0;
	std::deque<Seg> dq;
	std::vector<bool> V(sz, 0);
	M = 0;
	while (T < sz) {
		f = 0;
		for (int i = 0; i < sz; i++) {
			if (V[i]) continue;
			if (!dq.size()) {
				dq.push_back(segs[i]);
				V[i] = 1;
				f = 1;
				T++;
				break;
			}
			else if (dq[0].s == segs[i].e && dq[0].i != segs[i].i) {
				dq.push_front(segs[i]);
				V[i] = 1;
				f = 1;
				T++;
				break;
			}
			else if (dq.back().e == segs[i].s && dq.back().i != segs[i].i) {
				dq.push_back(segs[i]);
				V[i] = 1;
				f = 1;
				T++;
				break;
			}
		}
		if (dq[0].s == dq.back().e) {
			//std::cout << "polygon composed\n";
			ld ret = 0;
			int szq = dq.size();
			for (int k = 0; k < szq; k++) {
				ret += cross(O, dq[k].s, dq[k].e);
			}
			cnt += sign(ret) < 0;
			dq.clear();
		}
		if (f == 0) {
			//std::cout << "cleared\n";
			dq.clear();
		}
		//M++;
		//if (M > sz << 1) break;
		//std::cout << "DEBUG:: sz : " << sz << " T : " << T << " f : " << f << "\n";
		//for (Seg& S : dq) std::cout << S.s << " " << S.e << " ";
		//std::cout << "\n";
	}

	std::cout << cnt << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	std::cin >> Q;
	while (Q--) query();
	return;
}
int main() { solve(); return 0; }// Tehran Site 2000 H boj7333
