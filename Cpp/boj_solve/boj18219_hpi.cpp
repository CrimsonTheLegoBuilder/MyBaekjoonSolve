#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 2e3 + 5;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }

int N;
struct Pos {
	ld x, y;
	inline Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	inline bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	inline bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	inline bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	inline Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	inline Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	inline ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	inline ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	inline Pos operator - () const { return { -x, -y }; }
	inline Pos operator ~ () const { return { -y, x }; }
	inline Pos unit() const { return *this / mag(); }
	inline ld Euc() const { return x * x + y * y; }
	inline ld mag() const { return sqrt(Euc()); }
	inline ld rad() const { return atan2(y, x); }
	inline int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	inline friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	inline friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygonf;
struct Linear {//ps[0] -> ps[1] ::
	Pos ps[2];
	Pos dir_;
	inline const Pos& operator [] (const int& i) const { return ps[i]; }
	inline const Pos& dir() const { return dir_; }
	inline Linear(Pos a = Pos(0, 0), Pos b = Pos(0, 0)) {
		ps[0] = a;
		ps[1] = b;
		dir_ = (ps[1] - ps[0]).unit();
	}
	inline bool include(const Pos& p) const { return sign(dir_ / (p - ps[0])) > 0; }
	inline friend bool parallel(const Linear& l0, const Linear& l1) { return zero(l0.dir() / l1.dir()); }
	inline friend bool same_dir(const Linear& l0, const Linear& l1) { return parallel(l0, l1) && l0.dir() * l1.dir() > 0; }
	inline bool operator < (const Linear& l0) const {
		if (same_dir(*this, l0)) return l0.include(ps[0]);
		else return cmpq(this->dir(), l0.dir());
	}
};
typedef std::vector<Linear> VHP;
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
inline Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
inline Pos intersection(const Linear& l1, const Linear& l2) { return intersection(l1[0], l1[1], l2[0], l2[1]); }
inline std::vector<Pos> half_plane_intersection(std::vector<Linear>& HP) {
	auto check = [&](Linear& u, Linear& v, Linear& w) -> bool {
		return w.include(intersection(u, v));
		};
	std::sort(HP.begin(), HP.end());
	std::deque<Linear> dq;
	int sz = HP.size();
	for (int i = 0; i < sz; ++i) {
		if (i && same_dir(HP[i], HP[(i - 1) % sz])) continue;
		while (dq.size() > 1 && !check(dq[dq.size() - 2], dq[dq.size() - 1], HP[i])) dq.pop_back();
		while (dq.size() > 1 && !check(dq[1], dq[0], HP[i])) dq.pop_front();
		dq.push_back(HP[i]);
	}
	while (dq.size() > 2 && !check(dq[dq.size() - 2], dq[dq.size() - 1], dq[0])) dq.pop_back();
	while (dq.size() > 2 && !check(dq[1], dq[0], dq[dq.size() - 1])) dq.pop_front();
	sz = dq.size();
	if (sz < 3) return {};
	std::vector<Pos> HPI;
	for (int i = 0; i < sz; ++i) HPI.push_back(intersection(dq[i], dq[(i + 1) % sz]));
	return HPI;
}
inline ld area(const Polygonf& H) {
	Pos pivot = Pos(0, 0);
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret * .5;
}
struct Pii {
	int x, y;
	int i;
	inline Pii(int X = 0, int Y = 0) : x(X), y(Y) { i = -1; }
	inline bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
	inline bool operator != (const Pii& p) const { return x != p.x || y != p.y; }
	inline bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
	inline Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
	inline Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
	inline Pii operator * (const int& n) const { return { x * n, y * n }; }
	inline Pii operator / (const int& n) const { return { x / n, y / n }; }
	inline ll operator * (const Pii& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	inline ll operator / (const Pii& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	inline friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
};
typedef std::vector<Pii> Polygon;
inline ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
inline ll dot(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d2); }
inline int ccw(const Pii& d1, const Pii& d2, const Pii& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
inline bool on_seg_strong(const Pii& d1, const Pii& d2, const Pii& d3) { ll ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && ret >= 0; }
inline bool inner_check(Pii d1, Pii d2, Pii d3, const Pii& t) {
	if (ccw(d1, d2, d3) < 0) std::swap(d2, d3);
	return ccw(d1, d2, t) > 0 && ccw(d2, d3, t) > 0 && ccw(d3, d1, t) > 0;
}
inline ll area(const Polygon& H) {
	Pii pivot = Pii(0, 0);
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
inline void norm(Polygon& H, const bool& f = 1) {
	if (f && area(H) < 0) std::reverse(H.begin(), H.end());//ccw
	if (!f && area(H) > 0) std::reverse(H.begin(), H.end());//cw
	return;
}
inline Pos P(const Pii& p) { return Pos(p.x, p.y); }
inline Pii P(const Pos& p) { return Pii(p.x, p.y); }
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N;
	Polygon H(N);
	for (Pii& p : H) std::cin >> p;
	norm(H, 0);//cw

	//convex
	bool f = 1;
	for (int i = 0; i < N; i++) {
		Pii& pre = H[(i - 1 + N) % N], & cur = H[i], & nxt = H[(i + 1) % N];
		int CCW = ccw(pre, cur, nxt);
		assert(CCW);
		if (CCW > 0) f = 0;
	}
	if (f) { std::cout << std::abs(area(H)) * .5 << "\n"; return; }
	//convex

	//concave
	for (int i = 0; i < N; i++) {
		Pii& pre = H[(i - 1 + N) % N], & cur = H[i], & nxt = H[(i + 1) % N];
		if (ccw(pre, cur, nxt) == -1) continue;
		//if (ccw(pre, cur, nxt) > 0) ::
		Pii& nnxt = H[(i + 2) % N];
		if (ccw(cur, nxt, nnxt) == 1) continue;
		int j;
		for (j = (i + 2) % N; j != (i - 1 + N) % N; j = (j + 1) % N) {
			int j2 = (j + 1) % N;
			if (!ccw(cur, nxt, H[j2]) ||
				(ccw(cur, nxt, H[j]) != ccw(cur, nxt, H[j2]))) {
				break;
			}
		}
		cur.i = (j + 1) % N;
		for (int k = (i + 2) % N; k != (j + 1) % N; k = (k + 1) % N) {
			int k1 = (k + 1) % N;
			if (ccw(cur, nxt, H[k]) > 0 ||
				on_seg_strong(cur, H[k], nxt) ||
				inner_check(cur, H[k], H[k1], nxt)) {
				cur.i = -1;
				break;
			}
		}
	}
	VHP vhp;
	for (int i = 0; i < N; i++) {
		if (H[i].i == -1) continue;
		Pii& cur = H[i], & nxt = H[(i + 1) % N];
		vhp.push_back(Linear(P(nxt), P(cur)));
		int j, k = -1;
		for (j = (i + 2) % N; j != cur.i; j = (j + 1) % N) {
			if (H[j].i != -1) {
				k = j;
				break;
			}
		}
		if (k == -1) {
			for (j = (i + 1) % N; j != cur.i; j = (j + 1) % N) {
				int j2 = (j + 1) % N;
				vhp.push_back(Linear(P(H[j2]), P(H[j])));
			}
		}
		else {
			int k1 = (k + 1) % N;
			for (j = (i + 1) % N; j != k; j = (j + 1) % N) {
				assert(H[j].i == -1);
				int j2 = (j + 1) % N;
				int ccw1 = ccw(H[k], H[k1], H[j]);
				int ccw2 = ccw(H[k], H[k1], H[j2]);
				if (ccw1 == -1 && ccw2 == -1) {
					vhp.push_back(Linear(P(H[j2]), P(H[j])));
				}
				else if ((ccw1 == -1 || ccw2 == -1) && inner_check(H[k1], H[j], H[j2], H[k])) {
					vhp.push_back(Linear(P(H[j2]), P(H[j])));
					break;
				}
			}
		}
	}
	Polygonf hpi = half_plane_intersection(vhp);
	std::cout << area(hpi) << "\n";
	return;
}
int main() { solve(); return 0; }//boj18219 ICPC 2019 Asia Yokohama Regional J Fun Region

