#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
typedef long long ll;
typedef long double ld;
//typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-6;
const ld PI = acos(-1);
const int LEN = 2e3 + 5;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
struct II { int i1, i2; };

int Q, N;
ld ANS[LEN];
int ans = 0;
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
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos unit() const { return *this / mag(); }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	ld rad() const { return atan2(y, x); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
struct Linear {//ps[0] -> ps[1] ::
	Pos ps[2];
	Pos dir_;
	const Pos& operator [] (const int& i) const { return ps[i]; }
	const Pos& dir() const { return dir_; }
	Linear(Pos a = Pos(0, 0), Pos b = Pos(0, 0)) {
		ps[0] = a;
		ps[1] = b;
		dir_ = (ps[1] - ps[0]).unit();
	}
	bool include(const Pos& p) const { return sign(dir_ / (p - ps[0])) > 0; }
	friend bool parallel(const Linear& l0, const Linear& l1) { return zero(l0.dir() / l1.dir()); }
	friend bool same_dir(const Linear& l0, const Linear& l1) { return parallel(l0, l1) && l0.dir() * l1.dir() > 0; }
	bool operator < (const Linear& l0) const {
		if (same_dir(*this, l0)) return l0.include(ps[0]);
		else return cmpq(this->dir(), l0.dir());
	}
};
typedef std::vector<Linear> Planes;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
Pos intersection(const Linear& l1, const Linear& l2) { return intersection(l1[0], l1[1], l2[0], l2[1]); }
Polygon half_plane_intersection(std::vector<Linear>& HP) {
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
	Polygon HPI;
	for (int i = 0; i < sz; ++i) HPI.push_back(intersection(dq[i], dq[(i + 1) % sz]));
	return HPI;
}
ld area(const Polygon& H) {
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret * .5;
}
ld round(const Polygon& H) {
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += (H[i] - H[(i + 1) % sz]).mag();
	return ret;
}
Pos mirror(const Linear& l, const Pos& p) {
	Pos s = l[0], e = l[1];
	if (!ccw(s, e, p)) return p;
	Pos v = ~(e - s);
	Pos m = intersection(s, e, p, p + v);
	v = m - p;
	return p + v + v;
}
int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
	int sz = H.size();
	//if (!sz) return -1;
	//if (sz == 1) return p == H[0] ? 0 : -1;
	//if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
	assert(sz >= 3);
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
	int s = 1, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return 1;
	else if (on_seg_strong(H[s], H[e], p)) return 0;
	else return -1;
}
int inner_check_bi_search_2(const std::vector<Pos>& H, const Pos& p) {//convex
	int sz = H.size();
	//if (!sz) return -1;
	//if (sz == 1) return p == H[0] ? 0 : -1;
	//if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
	assert(sz >= 3);
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (p == H[0] || p == H[1] || p == H[sz - 1] ) return 1;
	if (on_seg_weak(H[0], H[1], p) || on_seg_weak(H[0], H[sz - 1], p)) return 0;
	//if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
	int s = 1, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return 2;
	else if (H[s] == p ||  H[e] == p) return 1;
	else if (on_seg_weak(H[s], H[e], p)) return 0;
	//else if (on_seg_strong(H[s], H[e], p)) return 0;
	else return -1;
}
int inner_check(const Polygon& H, const Pos& p) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		int i1 = (i + 1) % sz;
		if (H[i] == p) return 1;
		if (H[i1] == p) return 1;
		if (on_seg_weak(H[i], H[i1], p)) return 0;
		if (ccw(H[i], H[i1], p) < 0) return -1;
	}
	return 2;
}
II find_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
	int sz = H.size();
	//if (!sz) return -1;
	//if (sz == 1) return p == H[0] ? 0 : -1;
	//if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
	assert(sz >= 3);
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return { -1, -1 };
	if (p == H[0]) return { 0, -1 };
	if (p == H[1]) return { 1, -1 };
	if (p == H[sz - 1]) return { sz - 1, -1 };
	if (on_seg_strong(H[0], H[1], p)) return { 0, 1 };
	if (on_seg_strong(H[0], H[sz - 1], p)) return { 0, sz - 1 };
	int s = 1, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return { -1, -1 };
	else if (H[s] == p) return { s, -1 };
	else if (H[e] == p) return { e, -1 };
	else if (on_seg_strong(H[s], H[e], p)) return { s, e };
	else return { -1, -1 };
}
II find(const Polygon& H, const Pos& p) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		int i1 = (i + 1) % sz;
		if (H[i] == p) return { i, -1 };
		if (H[i1] == p) return { i1, -2 };
		if (on_seg_weak(H[i], H[i1], p)) return { i, i1 };
		if (ccw(H[i], H[i1], p) < 0) return { -1, -1 };
	}
	return { -1, -1 };
}
bool query() {
	std::cin >> N;
	if (!N) return 0;
	ld ret = 0;
	int V = 0;
	Polygon H(N);
	for (Pos& p : H) std::cin >> p;
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			Pos m = (H[i] + H[j]) * .5;
			Pos v = ~(H[i] - H[j]);
			Linear I = Linear(m, m + v);
			Linear J = Linear(m, m - v);
			Planes P0, Pi, Pj;
			for (int k = 0; k < N; k++) P0.push_back(Linear(H[k], H[(k + 1) % N]));
			Pi = P0;
			Pi.push_back(I);
			Polygon HI = half_plane_intersection(Pi);
			Pj = P0;
			Pj.push_back(J);
			Polygon HJ = half_plane_intersection(Pj);
			for (Pos& p : HJ) p = mirror(J, p);
			std::reverse(HJ.begin(), HJ.end());
			P0.clear();
			int sz = HI.size();
			for (int k = 0; k < sz; k++) P0.push_back(Linear(HI[k], HI[(k + 1) % sz]));
			sz = HJ.size();
			for (int k = 0; k < sz; k++) P0.push_back(Linear(HJ[k], HJ[(k + 1) % sz]));
			Polygon hpi = half_plane_intersection(P0);
			ld R = round(HI) + round(HJ) - round(hpi);
			int ni = HI.size(), nj = HJ.size();
			int ij = 0;
			for (const Pos& p : HI)
				if (inner_check_bi_search(hpi, p) >= 0 ||
					inner_check_bi_search(HJ, p) >= 0) ni--;
			for (const Pos& p : HJ)
				if (inner_check_bi_search(hpi, p) >= 0 ||
					inner_check_bi_search(HI, p) >= 0) nj--;
			for (const Pos& p : hpi) {
				//int ii = inner_check_bi_search_2(HI, p);
				//int jj = inner_check_bi_search_2(HJ, p);
				int ii = inner_check(HI, p);
				int jj = inner_check(HJ, p);
				//assert(ii != -1 && jj != -1);
				//if (ii == -1 || jj == -1) {
				//	std::cout << "DEBUG:: FUCK::\n";
				//	std::cout << "H = [\n";
				//	for (Pos& p : H) std::cout << "(" << p.x << ", " << p.y << "),\n";
				//	std::cout << "]\n";
				//	std::cout << "hpi = [\n";
				//	for (Pos& p : hpi) std::cout << "(" << p.x << ", " << p.y << "),\n";
				//	std::cout << "]\n";
				//	std::cout << "HI = [\n";
				//	for (Pos& p : HI) std::cout << "(" << p.x << ", " << p.y << "),\n";
				//	std::cout << "]\n";
				//	std::cout << "HJ = [\n";
				//	for (Pos& p : HJ) std::cout << "(" << p.x << ", " << p.y << "),\n";
				//	std::cout << "]\n";
				//	std::cout << "DEBUG:: FUCK::\n";
				//}
				if (ii == 1 && jj == 1) {
					II iii = find(HI, p);
					II jjj = find(HJ, p);
					int i1 = iii.i1, i2 = (i1 + 1) % HI.size(), i0 = (i1 - 1 + HI.size()) % HI.size();
					int j1 = jjj.i1, j2 = (j1 + 1) % HJ.size(), j0 = (j1 - 1 + HJ.size()) % HJ.size();
					//std::cout << i0 << " " << i1 << " " << i2 << "\n";
					//std::cout << j0 << " " << j1 << " " << j2 << "\n";
					if (!ccw(HI[i0], HI[i1], HJ[j2]) || !ccw(HJ[j0], HJ[j1], HI[i2])) ij++;
				}
				else if (ii == 0 && jj == 0) continue;
				else if (ii == 0 && jj == 1) {
					II iii = find(HI, p);
					II jjj = find(HJ, p);
					int i1 = iii.i1, i2 = iii.i2;
					int j1 = jjj.i1, j2 = (j1 + 1) % HJ.size(), j0 = (j1 - 1 + HJ.size()) % HJ.size();
					int c1 = ccw(HI[i1], HI[i2], HJ[j0]);
					int c2 = ccw(HI[i1], HI[i2], HJ[j2]);
					if (!c1 || !c2 || c1 * c2 == 1) ij++;
				}
				else if (ii == 1 && jj == 0) {
					II iii = find(HI, p);
					II jjj = find(HJ, p);
					int j1 = jjj.i1, j2 = jjj.i2;
					int i1 = iii.i1, i2 = (i1 + 1) % HI.size(), i0 = (i1 - 1 + HI.size()) % HI.size();
					int c1 = ccw(HJ[j1], HJ[j2], HI[i0]);
					int c2 = ccw(HJ[j1], HJ[j2], HI[i2]);
					if (!c1 || !c2 || c1 * c2 == 1) ij++;
				}
				else if (ii == 2 || jj == 2) ij++;
			}
			int n = hpi.size() + ni + nj - ij;
			if (V < n) {
				V = n;
				ret = R;
			}
			else if (V == n) {
				V = n;
				ret = std::max(ret, R);
			}//TOTAL O(T * 20 * 20 * 20 * 20) ~= O(T * 16,0000)
		}
	}
	//std::cout << ret << "\n";
	ANS[ans++] = ret;
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	while (query());
	for (int i = 0; i < ans; i++) std::cout << ANS[i] << "\n";
	return;
}
int main() { solve(); return 0; }//boj11044
