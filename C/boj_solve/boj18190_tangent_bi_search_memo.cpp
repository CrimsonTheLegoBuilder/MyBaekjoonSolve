#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
//#include <vector>
typedef long long ll;
typedef long double ld;
const int LEN = 1e5;
const ld TOL = 1e-7;
int N, M, Q;
ll memo_n[LEN + 1]{ 0 }, memo_m[LEN + 1]{ 0 };

bool z(ld x) { return std::abs(x) < TOL; }
struct Pos {
	ll x, y;
	ld xf, yf;
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y, xf + p.xf, yf + p.yf }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y, xf - p.xf, yf - p.yf }; }
	Pos operator * (const ll& n) const { return { x * n, y * n, xf * n, yf * n }; }
	Pos operator * (const ld& n) const { return { 0, 0, xf * n, yf * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n, xf / n, yf / n }; }
	Pos operator / (const ld& n) const { return { 0, 0, xf / n, yf / n }; }
	Pos operator * (const Pos& p) const { return { x * p.x + y * p.y, 0, xf * p.xf + yf * p.yf, 0.0 }; }
	Pos operator / (const Pos& p) const { return { x * p.y - y * p.x, 0, xf * p.yf - yf * p.xf, 0.0 }; }
} NH[LEN], MH[LEN], O = { 0, 0 };//O = origin
struct Vec {
	ld vy, vx;
	bool operator < (const Vec& v) const { return z(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	bool operator == (const Vec& v) const { return (z(vy - v.vy) && z(vx - v.vx)); }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }//cross
};
const Vec Zero = { 0, 0 };
struct Line {
	Vec s;
	ld c;
	bool operator < (const Line& l) const {//sort ccw
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ld ccw = s / l.s;
		return z(ccw) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : ccw > 0;
	}
	ld operator / (const Line& l) const { return s / l.s; }//cross
};
struct Info {
	ll area;
	ll l, r;
};
Line L(const Pos& p1, const Pos& p2) {
	ld dy, dx, c;
	dy = p2.yf - p1.yf;
	dx = p1.xf - p2.xf;
	c = dy * p1.xf + dx * p1.yf;
	return { {dy, dx} , c, };
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		0, 0,
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det
	};
}
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ld cross_f(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.xf - d1.xf) * (d3.yf - d2.yf) - (d2.yf - d1.yf) * (d3.xf - d2.xf);
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
	//for (int i = 0; i <= N; i++) std::cout << "memo[" << i << "] " << memo[i] << "\n";
	return;
}
int inner_check_bi_search(const Pos& p, Pos H[], const int& sz) {
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
Pos find_tangent_bi_search(const Pos& p, Pos H[], const int& sz) {
	int i1{ 0 }, i2{ 0 };
	int ccw1 = ccw(p, H[0], H[1]), ccwN = ccw(p, H[0], H[sz - 1]);
	//ll ccw_1 = cross(p, H[0], H[1]), ccw_N = cross(p, H[0], H[N - 1]);
	//std::cout << "DEBUG p: " << p.x << " " << p.y << " H[0]: " << H[0].x << " " << H[0].y << " H[1]:" << H[1].x << " " << H[1].y << " H[N-1]:" << H[N - 1].x << " " << H[N - 1].y << "\n";
	if (ccw1 * ccwN >= 0) {
		//std::cout << "tangent!! " << ccw_1 << " " << ccw_N << "\n";
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
		//std::cout << "tangent?? " << ccw_1 << " " << ccw_N << "\n";
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
	if (i1 > i2) std::swap(i1, i2);
	return { i1, i2, .0, .0 };
}
Info get_inner_area(Pos H[], ll memo[], const int& sz, const Pos& p) {
	ll area{ 0 };
	Pos tangent = find_tangent_bi_search(p, H, sz);
	//std::cout << "OUT tangent.. " << tangent.x << " " << tangent.y << "\n";
	ll i1 = tangent.x, i2 = tangent.y;
	ll tri = cross(O, H[i1], H[i2]);
	area = memo[i2] - memo[i1] - tri;
	//std::cout << memo[sz] << " " << memo[i2] << " " << memo[i1] << " " << cross(O, H[i1], H[i2]) << " " << area << "\n";
	if (cross(p, H[i1], H[i2]) < 0) area = memo[sz] - area;
	area += std::abs(cross(p, H[i1], H[i2]));
	bool f = cross(p, H[i1], H[i2]) > 0;
	if (f) std::swap(i1, i2);
	//std::cout << memo[sz] << " " << memo[i2] << " " << memo[i1] << " " << cross(O, H[i1], H[i2]) << " " << area << "\n";
	return { area, i1, i2 };
}
ld find_intersections_get_area_bi_search(Pos H_in[], const int& sz_in, Pos H_out[], const int& sz_out, ll memo[], const Pos& p) {//memo_n
	Info info = get_inner_area(MH, memo_m, M, p);
	ld area = -(ld)info.area;
	//std::cout << "area: " << area << "\n";
	int ir, il;
	Pos vr = H_in[info.r], vl = H_in[info.l], ip;
	int s = 0, e = sz_out - 1, k, m;
	while (s + 1 < e) {
		k = s + e >> 1;
		int CCW = ccw(H_out[0], H_out[k], p);
		if (CCW > 0) s = k;
		else e = k;
	}
	Pos S = H_out[s], E = H_out[e];
	//find_r_intersection
	int sr{ 0 }, er{ 0 };
	if (ccw(p, S, vr) >= 0 && ccw(p, E, vr) <= 0) {
		ir = e;
		sr = s, er = e;
		ip = intersection(L(S, E), L(p, vr));
		area += std::abs(cross_f(p, ip, E));
		//std::cout << sr << " " << er << "\n";
		//std::cout << std::abs(cross_f(p, ip, E)) << " area a\n";
		//std::cout << ip.xf << " " << ip.yf << "\n";
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
		ip = intersection(L(SR, ER), L(p, vr));
		area += std::abs(cross_f(p, ip, ER));
		//std::cout << sr << " " << er << "\n";
		//std::cout << std::abs(cross_f(p, ip, ER)) << " area b\n";
		//std::cout << ip.xf << " " << ip.yf << "\n";
	}
	//find_l_intersection
	int sl{ 0 }, el{ 0 };
	if (ccw(p, S, vl) >= 0 && ccw(p, E, vl) <= 0) {
		il = s;
		sl = s, el = e;
		ip = intersection(L(S, E), L(p, vl));
		area += std::abs(cross_f(p, ip, S));
		//std::cout << sl << " " << el << "\n";
		//std::cout << std::abs(cross_f(p, ip, S)) << " area a\n";
		//std::cout << ip.xf << " " << ip.yf << "\n";
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
		ip = intersection(L(SL, EL), L(p, vl));
		area += std::abs(cross_f(p, ip, SL));
		//std::cout << sl << " " << el << "\n";
		//std::cout << std::abs(cross_f(p, ip, SL)) << " area b\n";
		//std::cout << ip.xf << " " << ip.yf << "\n";
	}
	//total
	//std::cout << "OUT_idx: ir - " << ir << " il - " << il << " "  << area << "\n";
	bool f = ir <= il;
	//std::cout << il << " " << ir << "\n";
	if (il < ir) std::swap(ir, il);
	ll tri = cross(O, H_out[il], H_out[ir]);
	//std::cout << "tri " << tri << "\n";
	//std::cout << "memo " << memo[il] - memo[ir] << " " << memo[sz_out] << "\n";
	ll tmp = memo[il] - memo[ir] + tri;
	if (!f) tmp = memo[sz_out] - tmp;
	//std::cout << "tmp : " << tmp << "\n";
	area += tmp;
	area += std::abs(cross(p, H_out[il], H_out[ir]));
	//std::cout << sr << " " << sl << " " << area << "\n";
	if (sr == sl) area -= tmp + 2 * std::abs(cross(p, H_out[il], H_out[ir]));
	//std::cout << area << "\n";
	return area * .5;
}
void query() {
	Pos candle;
	std::cin >> candle.x >> candle.y;
	candle.xf = (ld)candle.x;
	candle.yf = (ld)candle.y;
	int f1, f2;
	f1 = inner_check_bi_search(candle, MH, M) > -1;
	f2 = inner_check_bi_search(candle, NH, N) < 1;
	if (f1) std::cout << "IN\n";
	else if (f2) std::cout << "OUT\n";
	else std::cout << find_intersections_get_area_bi_search(MH, M, NH, N, memo_n, candle) << "\n";
	return;
}
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N >> M >> Q;
	for (int i = 0; i < N; i++) {
		std::cin >> NH[i].x >> NH[i].y;
		NH[i].xf = (ld)NH[i].x;
		NH[i].yf = (ld)NH[i].y;
	}
	for (int j = 0; j < M; j++) {
		std::cin >> MH[j].x >> MH[j].y;
		MH[j].xf = (ld)MH[j].x;
		MH[j].yf = (ld)MH[j].y;
	}
	get_area_memo(NH, memo_n, N);
	get_area_memo(MH, memo_m, M);
	return;
}
void solve() { init(); while (Q--) query(); return; }
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

*/

