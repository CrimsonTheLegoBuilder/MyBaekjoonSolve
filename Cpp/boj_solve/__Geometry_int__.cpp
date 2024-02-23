#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
#include <random>
#include <array>
#include <tuple>
typedef long long ll;
typedef long double ld;
typedef std::pair<int, int> pi;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
int N, M, Q;
bool zero(const ld& x) { return std::abs(x) < TOL; }
struct Info { ll area, l, r; };

//2D============================================================================//
//2D============================================================================//
//2D============================================================================//
struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ll& n) const { return { x * n, y * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
	Pos operator ~ () const { return { -y, x }; }
	ll operator ! () const { return x * y; }
	ll Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p);
	friend std::ostream& operator << (std::ostream& os, const Pos& p);
} NH[LEN], MH[LEN], seq[LEN]; const Pos O = { 0, 0 };
std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y << "\n"; return os; }
std::vector<Pos> C, H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0;
}
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	//bool f3 = on_seg_strong(s1, s2, p1) ||
	//	on_seg_strong(s1, s2, p2) ||
	//	on_seg_strong(p1, p2, s1) ||
	//	on_seg_strong(p1, p2, s2);
	return (f1 && f2);// || f3;
}
void get_area_memo(Pos H[], ll memo[], const int& sz) {
	memo[0] = 0;
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		memo[i + 1] = cross(O, cur, nxt) + memo[i];//memo[sz] == convex hull's area
	}
	return;
}
ll area(Pos H[], const int& sz) {
	ll ret = 0;
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(O, cur, nxt);
	}
	return ret;
}
ll area(std::vector<Pos>& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(O, cur, nxt);
	}
	return ret;
}
void norm(std::vector<Pos>& H) { if (area(H) < 0) std::reverse(H.begin(), H.end()); }
bool inner_check(Pos H[], const int& sz, const Pos& p) {//concave
	int cnt{ 0 };
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (cur.y == nxt.y) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y <= p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return cnt & 1;
}
int inner_check_bi_search(Pos H[], const int& sz, const Pos& p) {//convex
	if (!sz) return -1;
	if (sz == 1) return p == H[0] ? 0 : -1;
	if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return 1;
	else if (on_seg_strong(H[s], H[e], p)) return 0;
	else return -1;
}
int inner_check_bi_search(std::vector<Pos>& H, const Pos& p) {//convex
	int sz = H.size();
	if (!sz) return -1;
	if (sz == 1) return p == H[0] ? 0 : -1;
	if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return 1;
	else if (on_seg_strong(H[s], H[e], p)) return 0;
	else return -1;
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) { for (const Pos& pos : C) H.push_back(pos); }
	else {
		for (int i = 0; i < C.size(); i++) {
			while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
		int s = H.size() + 1;
		for (int i = C.size() - 1; i >= 0; i--) {
			while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
	}
	return H;
}
std::vector<Pos> graham_scan(std::vector<Pos>& C) {
	std::vector<Pos> H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	 }
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	//C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
ll ccw(const std::vector<Pos>& H, const int& i, const int& f) {
	int sz = H.size();
	return (H[(i + 1) % sz] - H[i]) / (H[(f + 1) % sz] - H[f]);
}
ll rotating_calipers(const std::vector<Pos>& H) {
	ll MD = 0;
	int f = 0, l = H.size();
	for (int i = 0; i < l; i++) {
		while (ccw(H, i, f) > 0) {
			MD = std::max(MD, (H[i] - H[f]).Euc());
			f = (f + 1) % l;
		}
		MD = std::max(MD, (H[i] - H[f]).Euc());
	}
	return MD;
}
void rotating_calipers() {
	std::swap(C[0], *min_element(C.begin(), C.end()));//graham scan
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) {
		ll ret = cross(C[0], p, q);
		if (ret != 0) return ret > 0;
		return (C[0] - p).Euc() < (C[0] - q).Euc();
		}
	);
	for (int i = 0; i < N; i++) {
		while (H.size() >= 2 && cross(H[H.size() - 2], H.back(), C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	N = H.size();
	if (N == 2) { std::cout << "0\n"; return; }

	int R{ 0 }, U{ 0 }, L{ 0 };
	for (int j = 0; j < N; j++) if (dot(H[0], H[1], H[j], H[(j + 1) % N]) <= 0) { R = j; break; }
	for (int k = R; k < N; k++) if (cross(H[0], H[1], H[k], H[(k + 1) % N]) <= 0) { U = k; break; }
	for (int l = U; l <= N; l++) if (dot(H[0], H[1], H[l % N], H[(l + 1) % N]) >= 0) { L = l; break; }

	ld MIN{ INF };
	for (int i = 0; i < N; i++) {
		while (dot(H[i], H[(i + 1) % N], H[R % N], H[(R + 1) % N]) > 0) R++;
		while (cross(H[i], H[(i + 1) % N], H[U % N], H[(U + 1) % N]) > 0) U++;
		while (dot(H[i], H[(i + 1) % N], H[L % N], H[(L + 1) % N]) < 0) L++;
		ld h = cross(H[i], H[(i + 1) % N], H[U % N]) / (H[i] - H[(i + 1) % N]).mag();
		ld w = dot(H[i], H[(i + 1) % N], H[L % N], H[R % N]) / (H[i] - H[(i + 1) % N]).mag();
		MIN = std::min(MIN, h * w);
	}
	//ll ans = MIN;
	//if (MIN - ans > .5) ans++;
	ll ans = MIN + .5;
	std::cout << ans << "\n";
	return;
}
Pos find_tangent_bi_search(Pos H[], const int& sz, const Pos& p) {
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
			if (!f) std::swap(p1, cur);//normalize
			if (ccw(p1, cur, nxt) > 0) s = m + 1;
			else e = m;
		}
		i2 = s;
		if (!ccw(p, H[i2], H[(i2 + 1) % sz]) && dot(p, H[(i2 + 1) % sz], H[i2]) > 0) i2 = (i2 + 1) % sz;
	}
	else {
		//divide hull
		int s = 0, e = sz - 1, k, m;
		bool f = ccw1 > 0 && ccwN < 0;//if H[k] is between H[0] && p
		while (s + 1 < e) {
			k = s + e >> 1;
			int CCW = ccw(H[0], H[k], p);
			if (!f) CCW *= -1;//normailze
			if (CCW > 0) s = k;
			else e = k;
		}

		//search lower hull
		int s1 = 0, e1 = s;
		while (s1 < e1) {
			m = s1 + e1 >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % sz];
			if (!f) std::swap(p1, cur);//normalize
			if (ccw(p1, cur, nxt) > 0) s1 = m + 1;
			else e1 = m;
		}
		i1 = s1;
		if (!ccw(p, H[i1], H[(i1 + 1) % sz]) && dot(p, H[(i1 + 1) % sz], H[i1]) > 0) i1 = (i1 + 1) % sz;

		//search upper hull
		int s2 = e, e2 = sz - 1;
		while (s2 < e2) {
			m = s2 + e2 >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % sz];
			if (!f) std::swap(p1, cur);//normalize
			if (ccw(p1, cur, nxt) < 0) s2 = m + 1;
			else e2 = m;
		}
		i2 = s2;
		if (!ccw(p, H[i2], H[(i2 + 1) % sz]) && dot(p, H[(i2 + 1) % sz], H[i2]) > 0) i2 = (i2 + 1) % sz;
	}
	if (i2 < i1) std::swap(i2, i1);//normalize
	return { i2, i1 };
}
Info get_inner_area(Pos H[], ll memo[], const int& sz, const Pos& p) {
	Info tangent = find_tangent_bi_search(H, sz, p);
	ll i1 = tangent.r, i2 = tangent.l;
	ll tri = cross(O, H[i1], H[i2]);
	ll area = memo[i2] - memo[i1] - tri;
	if (cross(p, H[i1], H[i2]) < 0) area = memo[sz] - area, std::swap(i1, i2);//normalize
	area += std::abs(cross(p, H[i1], H[i2]));
	return { area, i2, i1 };
}
ld find_inx_get_area_bi_search(Pos H_in[], ll memo_in[], const int& sz_in, Pos H_out[], ll memo_out[], const int& sz_out, const Pos& p) {
	Info info = get_inner_area(H_in, memo_in, sz_in, p);
	Pos vr = H_in[info.r], vl = H_in[info.l];
	int ir, il;
	ld wing_r{ 0 }, wing_l{ 0 };

	//divide hull
	int s = 0, e = sz_out - 1, k, m;
	while (s + 1 < e) {
		k = s + e >> 1;
		int CCW = ccw(H_out[0], H_out[k], p);
		if (CCW > 0) s = k;
		else e = k;
	}
	Pos S = H_out[s], E = H_out[e];

	//find r-intersection
	int sr{ 0 }, er{ 0 };
	Pos SR, ER;
	if (ccw(p, S, vr) >= 0 && ccw(p, E, vr) <= 0) sr = s, er = e;//if vr is in p-S-E tri.
	else {
		if (ccw(H_out[0], p, vr) > 0) sr = e, er = sz_out;
		if (ccw(H_out[0], p, vr) < 0) sr = 0, er = s;
		while (sr + 1 < er) {
			m = sr + er >> 1;
			int CCW = ccw(p, H_out[m % sz_out], vr);
			if (CCW > 0) sr = m;
			else er = m;
		}
	}
	SR = H_out[sr % sz_out], ER = H_out[er % sz_out];
	ir = er % sz_out;
	ll trir = std::abs(cross(p, SR, ER));
	ll ar = std::abs(cross(p, vr, SR));
	ll br = std::abs(cross(p, vr, ER));
	wing_r = trir * (ld)br / (ar + br);
	if (!cross(p, vr, H_out[er % sz_out])) wing_r = 0;

	//find l-intersection
	int sl{ 0 }, el{ 0 };
	Pos SL, EL;
	if (ccw(p, S, vl) >= 0 && ccw(p, E, vl) <= 0) sl = s, el = e;//if vl is in p-S-E tri.
	else {
		if (ccw(H_out[0], p, vl) > 0) sl = e, el = sz_out;
		if (ccw(H_out[0], p, vl) < 0) sl = 0, el = s;
		while (sl + 1 < el) {
			m = sl + el >> 1;
			int CCW = ccw(p, H_out[m % sz_out], vl);
			if (CCW > 0) sl = m;
			else el = m;
		}
	}
	SL = H_out[sl % sz_out], EL = H_out[el % sz_out];
	il = sl % sz_out;
	ll tril = std::abs(cross(p, SL, EL));
	ll al = std::abs(cross(p, vl, SL));
	ll bl = std::abs(cross(p, vl, EL));
	wing_l = tril * (ld)al / (al + bl);
	if (!cross(p, vl, H_out[sl % sz_out])) wing_l = 0;

	//DEBUG
	//std::cout << "in R: " << info.r << " in L: " << info.l << " out R: " << ir << " out L: " << il << "\n";
	//std::cout << "wing R: " << wing_r << " wing L: " << wing_l << "\n";
	//std::cout << "wing R: " << trir * (ld)ar / (ar + br) << " wing L: " << tril * (ld)bl / (al + bl) << "\n";
	//std::cout << "inner: " << info.area << "\n";

	//get_shadow
	ld area{ 0 };
	if (sr == sl) {//if 2 intersections on the same segment
		area = -(ld)(info.area + std::abs(cross(p, H_out[ir], H_out[il]))) + (wing_r + wing_l);
	}
	else {
		bool f = ir > il;
		if (ir > il) std::swap(ir, il);//normalize
		ll tri = cross(O, H_out[ir], H_out[il]);
		ll tmp = memo_out[il] - memo_out[ir] - tri;
		if (f) tmp = memo_out[sz_out] - tmp;
		area = -(ld)(info.area - tmp - std::abs(cross(p, H_out[ir], H_out[il]))) + (wing_r + wing_l);
	}
	return area * .5;
}
//3D============================================================================//
//3D============================================================================//
//3D============================================================================//
struct Pos3D {
	ll x, y, z;
	Pos3D(ll X = 0, ll Y = 0, ll Z = 0) : x(X), y(Y), z(Z) {}
	bool operator == (const Pos3D& p) const { return x == p.x && y == p.y && z == p.z; }
	bool operator != (const Pos3D& p) const { return x != p.x || y != p.y || z != p.z; }
	bool operator < (const Pos3D& p) const { return x == p.x ? y == p.y ? z < p.z : y < p.y : x < p.x; }
	ll operator * (const Pos3D& p) const { return x * p.x + y * p.y + z * p.z; }
	Pos3D operator / (const Pos3D& p) const {
		Pos3D ret;
		ret.x = y * p.z - z * p.y;
		ret.y = z * p.x - x * p.z;
		ret.z = x * p.y - y * p.x;
		return ret;
	}
	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
	Pos3D& operator += (const Pos3D& p) { x + p.x; y + p.y; z + p.z; return *this; }
	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pos3D operator * (const ll& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Pos3D& operator *= (const ll& scalar) { x* scalar; y* scalar; z* scalar; return *this; }
	ll Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos3D& p);
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p);
} candi[LEN], willy, MAXP{ INF, INF, INF };
std::istream& operator >> (std::istream& is, Pos3D& p) { is >> p.x >> p.y >> p.z; return is; }
std::ostream& operator << (std::ostream& os, const Pos3D& p) { os << p.x << " " << p.y << " " << p.z << "\n"; return os; }
using Face = std::array<int, 3>;
std::vector<Face> Hull3D;
struct Edge {
	int face_num, edge_num;
	Edge(int t = 0, int v = 0) : face_num(t), edge_num(v) {}
};
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ll ret = dot(d1, d3, d2);
	return !cross(d1, d2, d3).mag() && ret >= 0;
}
bool on_seg_weak(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ll ret = dot(d1, d3, d2);
	return !cross(d1, d2, d3).mag() && ret > 0;
}
int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
	Pos3D CCW = cross(d1, d2, d3);
	ll ret = CCW * norm;
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
ld area(const std::vector<Pos3D>& H, const Pos3D& norm) {
	ll ret = 0;
	if (H.size() < 3) return ret;
	Pos3D O = H[0];
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos3D cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(O, cur, nxt) * norm / norm.Euc();
	}
	return std::abs(ret * .5);
}
bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3).mag()) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3).mag()) && ret > 0;
}
//std::vector<Pos3D> graham_scan(std::vector<Pos3D>& C, const Pos3D& norm) {
ld graham_scan(std::vector<Pos3D>& C, const Pos3D& norm) {
	//if (C.size() < 3) {
	//	std::sort(C.begin(), C.end());
	//	return C;
	// }
	if (C.size() < 3) return 0;
	std::vector<Pos3D> H;
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos3D& p, const Pos3D& q) -> bool {
		int ret = ccw(C[0], p, q, norm);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i], norm) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	//return H;
	return area(H, norm);
}
bool collinear(const Pos3D& a, const Pos3D& b, const Pos3D& c) {
	return ((b - a) / (c - b)).Euc() == 0;
}
int dot_cross(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {
	return cross(a, b, c) * (p - a);
}
bool coplanar(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {
	return cross(a, b, c) * (p - a) == 0;
}
bool above(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {// is p strictly above plane
	return cross(a, b, c) * (p - a) > 0;
}
void prep(std::vector<Pos3D>& p) {//refer to Koosaga'
	shuffle(p.begin(), p.end(), std::mt19937(0x14004));
	int dim = 1;
	for (int i = 1; i < p.size(); i++) {
		if (dim == 1) {
			if (p[0] != p[i]) std::swap(p[1], p[i]), ++dim;
		}
		else if (dim == 2) {
			if (!collinear(p[0], p[1], p[i]))
				std::swap(p[2], p[i]), ++dim;
		}
		else if (dim == 3) {
			if (!coplanar(p[0], p[1], p[2], p[i]))
				std::swap(p[3], p[i]), ++dim;
		}
	}
	assert(dim == 4);
	return;
}
ld dist(const std::vector<Pos3D>& C, const Face& F, const Pos3D& p) {
	Pos3D norm = cross(C[F[0]], C[F[1]], C[F[2]]);
	ll ret = norm * (p - C[F[0]]);
	return std::abs(ret / (norm.mag()));
}
ld get_min_dist(const std::vector<Pos3D>& C, const std::vector<Face>& F, const Pos3D& p) {
	ld MIN = INF;
	for (const Face& face : F) MIN = std::min(MIN, dist(C, face, p));
	return MIN;
}
std::vector<Face> convex_hull_3D(std::vector<Pos3D>& candi) {
	// 3D Convex Hull in O(n log n)
	// Very well tested. Good as long as not all points are coplanar
	// In case of collinear faces, returns arbitrary triangulation
	// Credit: Benq
	// refer to Koosaga'
	prep(candi);
	int sz = candi.size();
	std::vector<Face> faces;
	std::vector<int> active;//whether face is active - face faces outside 
	std::vector<std::vector<int>> vis(sz);//faces visible from each point
	std::vector<std::vector<int>> rvis;//points visible from each face
	std::vector<std::array<Edge, 3>> other;//other face adjacent to each edge of face
	auto ad = [&](const int& a, const int& b, const int& c) -> void {//add face
		faces.push_back({ a, b, c });
		active.push_back(1);
		rvis.emplace_back();
		other.emplace_back();
		return;
	};
	auto visible = [&](const int& a, const int& b) -> void {
		vis[b].push_back(a);
		rvis[a].push_back(b);
		return;
	};
	auto abv = [&](const int& a, const int& b) -> bool {//above
		Face tri = faces[a];
		return above(candi[tri[0]], candi[tri[1]], candi[tri[2]], candi[b]);
	};
	auto edge = [&](const Edge& e) -> pi {
		return { faces[e.face_num][e.edge_num], faces[e.face_num][(e.edge_num + 1) % 3] };
	};
	auto glue = [&](const Edge& a, const Edge& b) -> void {//link two faces by an edge
		pi x = edge(a); assert(edge(b) == pi(x.second, x.first));
		other[a.face_num][a.edge_num] = b;
		other[b.face_num][b.edge_num] = a;
		return;
	};//ensure face 0 is removed when i = 3

	ad(0, 1, 2), ad(0, 2, 1);
	if (abv(1, 3)) std::swap(candi[1], candi[2]);
	for (int i = 0; i < 3; i++) glue({ 0, i }, { 1, 2 - i });
	for (int i = 3; i < N; i++) visible(abv(1, i), i);//coplanar points go in rvis[0]

	std::vector<int> label(sz, -1);
	for (int i = 3; i < sz; i++) {//incremental construction
		std::vector<int> rem;
		for (auto& v : vis[i]) if (active[v]) { active[v] = 0, rem.push_back(v); }
		if (!rem.size()) continue;//hull unchanged

		int st = -1;//start idx
		for (const int& v : rem) {
			for (int j = 0; j < 3; j++) {
				int o = other[v][j].face_num;
				if (active[o]) {//create new face!
					int idx1, idx2;
					std::tie(idx1, idx2) = edge({ v, j });
					ad(idx1, idx2, i);
					st = idx1;
					int cur = rvis.size() - 1;
					label[idx1] = cur;

					std::vector<int> tmp;
					set_union(rvis[v].begin(), rvis[v].end(), rvis[o].begin(), rvis[o].end(), back_inserter(tmp));
					//merge sorted vectors ignoring duplicates

					for (auto& x : tmp) if (abv(cur, x)) visible(cur, x);
					//if no rounding errors then guaranteed that only x > i matters

					glue({ cur, 0 }, other[v][j]);//glue old, new face
				}
			}
		}
		for (int x = st, y; ; x = y) {//glue new faces together
			int X = label[x];
			glue({ X, 1 }, { label[y = faces[X][1]], 2 });
			if (y == st) break;
		}
	}
	std::vector<Face> hull3D;
	for (int i = 0; i < faces.size(); i++) if (active[i]) hull3D.push_back(faces[i]);
	return hull3D;
}