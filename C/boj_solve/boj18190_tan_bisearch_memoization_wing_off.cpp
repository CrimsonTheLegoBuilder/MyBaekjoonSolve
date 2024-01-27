#include <iostream>
#include <algorithm>
#include <cmath>
//#include <cstring>
//#include <vector>
typedef long long ll;
typedef long double ld;
//typedef double ld;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ll SCALE = 100;
int N, M, Q;
ll memo_n[LEN]{ 0 }, memo_m[LEN]{ 0 };

bool z(ld x) { return std::abs(x) < TOL; }
struct Pos {
	ll x, y;
	Pos(ll X, ll Y) : x(X), y(Y) {}
	Pos() : x(0), y(0) {}
	Pos operator * (const ll& n) const { return { x * n, y * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n }; }
	Pos& operator *= (const ll scale) {
		x *= scale; y *= scale;
		return *this;
	}
	Pos& operator /= (const ll scale) {
		x /= scale; y /= scale;
		return *this;
	}
} NH[LEN], MH[LEN], q[LEN];
const Pos O = { 0, 0 };
struct Info { ll area, l, r; };
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
}
void get_area_memo(Pos H[], ll memo[], const int& sz) {
	memo[0] = 0;
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		memo[i + 1] = cross(O, cur, nxt) + memo[i];
	}
	return;
}
int inner_check_bi_search(Pos H[], const int& sz, const Pos& p) {
	if (sz < 3 || cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (on_seg(H[0], H[1], p) || on_seg(H[0], H[sz - 1], p)) return 0;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return 1;
	else if (on_seg(H[s], H[e], p)) return 0;
	else return -1;
}
Info find_tangent_bi_search(Pos H[], const int& sz, const Pos& p) {
	int i1{ 0 }, i2{ 0 };
	int ccw1 = ccw(p, H[0], H[1]), ccwN = ccw(p, H[0], H[sz - 1]);
	if (ccw1 * ccwN >= 0) {
		i1 = 0;
		if (!ccw1 && dot(p, H[1], H[0]) > 0) i1 = 1;
		if (!ccwN && dot(p, H[sz - 1], H[0]) > 0) i1 = sz - 1;
		int s = 0, e = sz - 1, m;
		if (!ccw1) s += 1;
		if (!ccwN) e -= 1;
		bool f = ccw(p, H[s], H[s + 1]) >= 0;
		while (s < e) {
			m = s + e >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % sz];
			if (!f) std::swap(p1, cur);
			if (ccw(p1, cur, nxt) > 0) s = m + 1;
			else e = m;
		}
		i2 = s;
		if (!ccw(p, H[i2], H[(i2 + 1)]) && dot(p, H[(i2 + 1)], H[i2]) > 0) i2 = (i2 + 1) % sz;
	}
	else {
		int s = 0, e = sz - 1, k, m;
		bool f = ccw1 > 0 && ccwN < 0;// if H[k] is between H[0] && p
		while (s + 1 < e) {
			k = s + e >> 1;
			int CCW = ccw(H[0], H[k], p);
			if (!f) CCW *= -1;
			if (CCW > 0) s = k;
			else e = k;
		}
		int s1 = 0, e1 = s;
		while (s1 < e1) {
			m = s1 + e1 >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % sz];
			if (!f) std::swap(p1, cur);
			if (ccw(p1, cur, nxt) > 0) s1 = m + 1;
			else e1 = m;
		}
		i1 = s1;
		if (!ccw(p, H[i1], H[(i1 + 1) % sz]) && dot(p, H[(i1 + 1) % sz], H[i1]) > 0) i1 = (i1 + 1) % sz;
		int s2 = e, e2 = sz - 1;
		while (s2 < e2) {
			m = s2 + e2 >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % sz];
			if (!f) std::swap(p1, cur);
			if (ccw(p1, cur, nxt) < 0) s2 = m + 1;
			else e2 = m;
		}
		i2 = s2;
		if (!ccw(p, H[i2], H[(i2 + 1) % N]) && dot(p, H[(i2 + 1) % sz], H[i2]) > 0) i2 = (i2 + 1) % sz;
	}
	if (i2 < i1) std::swap(i2, i1);
	return { 0, i2, i1 };
}
Info get_inner_area(Pos H[], ll memo[], const int& sz, const Pos& p) {
	Info tangent = find_tangent_bi_search(H, sz, p);
	ll i1 = tangent.r, i2 = tangent.l;
	ll tri = cross(O, H[i1], H[i2]);
	ll area = memo[i2] - memo[i1] - tri;
	if (cross(p, H[i1], H[i2]) < 0) area = memo[sz] - area;
	area += std::abs(cross(p, H[i1], H[i2]));
	bool f = cross(p, H[i1], H[i2]) > 0;
	if (!f) std::swap(i1, i2);
	return { area, i2, i1 };
}
ld find_inx_get_area_bi_search(Pos H_in[], ll memo_in[], const int& sz_in, Pos H_out[], ll memo_out[], const int& sz_out, const Pos& p) {
	Info info = get_inner_area(H_in, memo_in, sz_in, p);
	Pos vr = H_in[info.r], vl = H_in[info.l], ip;
	ld wing_r{ 0 }, wing_l{ 0 };
	//find_crossing_point
	int ir, il, s = 0, e = sz_out - 1, k, m;
	while (s + 1 < e) {
		k = s + e >> 1;
		int CCW = ccw(H_out[0], H_out[k], p);
		if (CCW > 0) s = k;
		else e = k;
	}
	Pos S = H_out[s], E = H_out[e];
	//find_r-intersection
	int sr{ 0 }, er{ 0 };
	if (ccw(p, S, vr) >= 0 && ccw(p, E, vr) <= 0) {//if vr is in p-S-E tri.
		ir = e;
		sr = s, er = e;
		ll tri = std::abs(cross(p, S, E));
		ll a = std::abs(cross(p, vr, S)); 
		ll b = std::abs(cross(p, vr, E));
		wing_r = (ld)tri * (ld)b / (a + b);
	}
	else {
		if (ccw(H_out[0], p, vr) > 0) sr = e, er = sz_out;
		if (ccw(H_out[0], p, vr) < 0) sr = 0, er = s;
		while (sr + 1 < er) {
			m = sr + er >> 1;
			int CCW = ccw(p, H_out[m % sz_out], vr);
			if (CCW > 0) sr = m;
			else er = m;
		}
		Pos SR = H_out[sr % sz_out], ER = H_out[er % sz_out];
		ir = er % sz_out;
		ll tri = std::abs(cross(p, SR, ER));
		ll a = std::abs(cross(p, vr, SR));
		ll b = std::abs(cross(p, vr, ER));
		wing_r = (ld)tri * (ld)b / (a + b);
	}
	//find_l-intersection
	int sl{ 0 }, el{ 0 };
	if (ccw(p, S, vl) >= 0 && ccw(p, E, vl) <= 0) {//if vl is in p-S-E tri.
		il = s;
		sl = s, el = e;
		ll tri = std::abs(cross(p, S, E));
		ll a = std::abs(cross(p, vl, S)); 
		ll b = std::abs(cross(p, vl, E));
		wing_l = (ld)tri * (ld)a / (a + b);
	}
	else {
		if (ccw(H_out[0], p, vl) > 0) sl = e, el = sz_out;
		if (ccw(H_out[0], p, vl) < 0) sl = 0, el = s;
		while (sl + 1 < el) {
			m = sl + el >> 1;
			int CCW = ccw(p, H_out[m % sz_out], vl);
			if (CCW > 0) sl = m;
			else el = m;
		}
		Pos SL = H_out[sl % sz_out], EL = H_out[el % sz_out];
		il = sl % sz_out;
		ll tri = std::abs(cross(p, SL, EL));
		ll a = std::abs(cross(p, vl, SL));
		ll b = std::abs(cross(p, vl, EL));
		wing_l = (ld)tri * (ld)a / (a + b);
	}
	//std::cout << wing_r << " " << wing_l << "\n";
	//get_area
	//if (wing_l < wing_r) std::swap(wing_l, wing_r);
	ld area{ 0 };
	if (sr == sl) {//if 2 intersections on the same segment
		//area = -(ld)info.area - (ld)std::abs(cross(p, H_out[ir], H_out[il])) + wing_r + wing_l;
		area = -(info.area + std::abs(cross(p, H_out[ir], H_out[il]))) + (wing_r + wing_l);
	}
	else {
		bool f = ir > il;
		if (ir > il) std::swap(ir, il);
		ll tri = cross(O, H_out[ir], H_out[il]);
		ll tmp = memo_out[il] - memo_out[ir] - tri;
		if (f) tmp = memo_out[sz_out] - tmp;
		//area = -(ld)info.area + (ld)tmp + (ld)std::abs(cross(p, H_out[ir], H_out[il])) + wing_r + wing_l;
		area = -(info.area - tmp - std::abs(cross(p, H_out[ir], H_out[il]))) + (wing_r + wing_l);
	}
	return area * .5;
	//return std::max(area1, area2) * .5;
}
void query(const int& i) {
	Pos candle = q[i];
	int f1, f2;
	f1 = inner_check_bi_search(MH, M, candle) > -1;
	f2 = inner_check_bi_search(NH, N, candle) < 1;
	if (f1) std::cout << "IN\n";
	else if (f2) std::cout << "OUT\n";
	else std::cout << find_inx_get_area_bi_search(MH, memo_m, M, NH, memo_n, N, candle) / (SCALE * SCALE) << "\n";
	//else std::cout << find_inx_get_area_bi_search(MH, memo_m, M, NH, memo_n, N, candle) << "\n";
	return;
}
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	std::cin >> N >> M >> Q;
	for (int i = 0; i < N; i++) std::cin >> NH[i].x >> NH[i].y, NH[i] *= SCALE;
	for (int j = 0; j < M; j++) std::cin >> MH[j].x >> MH[j].y, MH[j] *= SCALE;
	for (int k = 0; k < Q; k++) std::cin >> q[k].x >> q[k].y, q[k] *= SCALE;
	get_area_memo(NH, memo_n, N);
	get_area_memo(MH, memo_m, M);
	return;
}
void solve() { init(); for (int i = 0; i < Q; i++) query(i); return; }
int main() { solve(); return 0; }//boj18190




/*

4 4 14
-4 -4
4 -4
4 4
-4 4
-1 -1
1 -1
1 1
-1 1
0 0
3 3
3 -3
-3 3
-3 -3
0 2
-2 0
0 -2
2 0
0 3
-3 0
0 -3
3 0
6 6

4 4 42
-4 -4
4 -4
4 4
-4 4
-1 -1
1 -1
1 1
-1 1
0 0
5 5
0 2
2 0
0 -2
-2 0
3 0
-3 0
0 3
0 -3
2 1
2 -1
-2 1
-2 -1
1 2
1 -2
-1 2
-1 -2
2 2
2 -2
-2 2
-2 -2
3 1
3 -1
-3 1
-3 -1
1 3
1 -3
-1 3
-1 -3
2 3
2 -3
-2 3
-2 -3
3 2
3 -2
-3 2
-3 -2
3 3
3 -3
-3 3
-3 -3

8 8 53
10 5
5 10
-5 10
-10 5
-10 -5
-5 -10
5 -10
10 -5
-2 -4
2 -4
4 -2
4 2
2 4
-2 4
-4 2
-4 -2
3 4
4 3
-3 -4
-4 -3
3 -4
4 -3
-3 4
-4 -3
4 9
9 4
-4 -9
-9 -4
-4 9
-9 4
4 -9
9 -4
0 5
5 0
0 -5
-5 0
7 0
0 7
-7 0
0 -7
1 6
1 -6
6 1
6 -1
-1 6
-1 -6
-6 1
-6 -1
3 3
-3 -3
3 -3
-3 3
4 4
-4 -4
4 -4
-4 4
5 5
-5 -5
5 -5
-5 5
6 6
-6 -6
6 -6
-6 6
7 7
-7 -7
-7 7
7 -7
8 8

4 4 1
-1000000 -1000000
-999996 -1000000
1000000 1000000
-1000000 -999996

-999999 -999999
-999998 -999999
999998 999998
-999999 -999998

999999 999999


4 4 5
-1000000 -1000000
-999996 -1000000
1000000 1000000
-1000000 -999996

-999999 -999999
-999998 -999999
999994 999994
-999999 -999998

999999 999999
999998 999998
999997 999997
999996 999996
999995 999995

4 4 21
-1000000 1000000
-1000000 -1000000
1000000 -1000000
1000000 1000000
-999999 -999999
-999998 -999999
-999998 -999998
-999999 -999998
999999 999999
999999 999998
999998 999999
999999 -999999
-999999 999999
999999 -999998
-999998 999999
999999 -999997
-999997 999999
999999 -999996
-999996 999999
999999 -999995
-999995 999999
999999 -999994
-999994 999999
999999 -999993
-999993 999999
0 999999
999999 0
-999999 0
0 -999999

4 4 1
-1000000 -1000000
-999996 -1000000
1000000 1000000
-1000000 -999996

-999999 -999999
-999998 -999999
999998 999998
-999999 -999998

999999 999999

8 8 16
1000000 500000
500000 1000000
-500000 1000000
-1000000 500000
-1000000 -500000
-500000 -1000000
500000 -1000000
1000000 -500000
-200000 -400000
200000 -400000
400000 -200000
400000 200000
200000 400000
-200000 400000
-400000 200000
-400000 -200000

999999 0
0 999999
-999999 0
0 -999999
999999 1
1 999999
-999999 1
1 -999999
999999 -1
-1 999999
-999999 -1
-1 -999999
400001 0
0 400001
-400001 0
0 -400001

4 4 56
-1000000 1000000
-1000000 -1000000
1000000 -1000000
1000000 1000000
-999999 -999999
-999998 -999999
-999998 -999998
-999999 -999998

999999 999999

999999 999998
999998 999999
999998 999998

999999 999997
999997 999999
999997 999997

999996 999999
999999 999996
999996 999996

999995 999999
999999 999995
999995 999995

-999997 -999997
-999996 -999996
-999995 -999995
-999994 -999994

0 999999
999999 0
0 999998
999998 0
0 999997
999997 0
0 999996
999996 0

0 0
1 -1
-1 1
2 -2
-2 2
3 -3
-3 3

-999985 0
0 -999985
-999999 0
-999999 -950000
0 -999999
-950000 -999999
-999998 0
0 -999998
-999997 0
0 -999997
-999996 0
0 -999996
-999995 0
0 -999995
-999994 0
0 -999994
-999993 0
0 -999993
-999992 0
0 -999992
-999991 0
0 -999991
-999990 0
0 -999990


4 4 20
-1000000 1000000
-1000000 -1000000
1000000 -1000000
1000000 1000000
-999999 -999999
-999998 -999999
-999998 -999998
-999999 -999998

-999985 0
0 -999985
-999999 -900000
-900000 -999999
-999999 0
0 -999999
-999998 0
0 -999998
-999997 0
0 -999997
-999996 0
0 -999996
-999995 0
0 -999995
-999994 0
0 -999994
-999993 0
0 -999993
-999992 0
0 -999992
-999991 0
0 -999991
-999990 0
0 -999990

*/
