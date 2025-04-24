#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-9;
const ld PI = acos(-1);
const int LEN = 1e3;
inline bool zero(const ld& x) { return std::abs(x) < TOL; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }

int N, M, T, Q, C;
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	inline ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	inline ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld xy() const { return x * y; }
	Pos rot(ld the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
inline ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
//inline ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && (ret > 0 || zero(ret));
}
//inline bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ld ret = dot(d1, d3, d2);
//	return !ccw(d1, d2, d3) && ret > 0;
//}
//inline Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
//	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
//	return (p1 * a2 + p2 * a1) / (a1 + a2);
//}
bool inner_check(const std::vector<Pos>& H, const Pos& p) {//concave
	int cnt = 0, sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (zero(cur.y - nxt.y)) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return (cnt & 1) * 2;
}
//ld area(std::vector<Pos>& H) {
//	Pos pivot = Pos(0, 0);
//	ld ret = 0;
//	int h = H.size();
//	for (int i = 0; i < h; i++) {
//		Pos cur = H[i], nxt = H[(i + 1) % h];
//		ret += cross(pivot, cur, nxt);
//	}
//	return ret / 2;
//}
//inline void norm(Polygon& H) { if (area(H) < 0) std::reverse(H.begin(), H.end()); }
std::vector<Pos> polygon_line_intersections(const Pos& s, const Pos& e, Polygon& P) {
	auto valid = [&](const ld& t, const ld& x) -> bool {
		return (-TOL < t) && (t < 1 + TOL) && (-TOL < x) && (x < 1 + TOL);
		};

	std::vector<Pos> ret;
	std::vector<ld> tmp = { .0, 1. };

	Pos vt = e - s;
	int sz = P.size();
	for (int i = 0; i < sz; i++) {
		Pos a = P[i], b = P[(i + 1) % sz];
		Pos vx = b - a;
		ld det = vt / vx;
		if (zero(det)) continue;
		ld t = (vt / (s - a)) / det;//ratio of a -> inx -> b
		ld x = (vx / (a - s)) / -det;//ratio of s -> inx -> e
		if (valid(t, x)) tmp.push_back(x);
	}

	std::sort(tmp.begin(), tmp.end());
	tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
	for (int i = 0; i < tmp.size() - 1; i++) {
		Pos p = s + vt * (tmp[i] + tmp[i + 1]) * .5;
		//if (inner_check(P, p)) {
		//	ret.push_back(s + vt * tmp[i]);
		//	ret.push_back(s + vt * tmp[i + 1]);
		//}//Pos, Pos
		if (inner_check(P, p)) ret.push_back(Pos(tmp[i], tmp[i + 1]));//ratio, ratio
	}
	return ret;
}
std::vector<Pos> circle_line_intersections(const Pos& s, const Pos& e, const Pos& p, const ld& r) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	Pos vec = e - s;
	Pos OM = s - p;

	ld a = vec * vec;
	ld b = 2 * (vec * OM);
	ld c = OM * OM - r * r;

	ld J = b * b - 4 * a * c;

	if (J < TOL) return {};

	ld lo = (-b - sqrt(J)) / (2 * a);
	ld hi = (-b + sqrt(J)) / (2 * a);

	//if (hi < lo) std::swap(lo, hi);
	if (hi < 0 || 1 < lo) return {};

	//return { s + vec * lo, s + vec * hi };//Pos, Pos
	return { Pos(std::max((ld)0, lo), std::min(hi, (ld)1)) };//ratio, ratio
}
Pos path[20];
Polygon continents[20];
bool F(ld m) {
	auto box = [&](const Pos& u, const Pos& v) -> Polygon {
		Pos n = ~(v - u).unit();
		return { u + n * m, u - n * m, v - n * m, v + n * m };
		};

	int sz;
	for (int i = 0; i < N - 1; i++) {
		std::vector<Pos> tmp;
		std::vector<Pos> inxs_ratio;
		Pos s = path[i], e = path[i + 1];

		for (int j = 0; j < C; j++) {
			inxs_ratio = polygon_line_intersections(s, e, continents[j]);
			tmp.insert(tmp.end(), inxs_ratio.begin(), inxs_ratio.end());
		}

		for (int j = 0; j < C; j++) {
			sz = continents[j].size();
			for (int k = 0; k < sz; k++) {
				Pos u = continents[j][k], v = continents[j][(k + 1) % sz];
				
				Polygon B = box(u, v);
				inxs_ratio = polygon_line_intersections(s, e, B);
				tmp.insert(tmp.end(), inxs_ratio.begin(), inxs_ratio.end());

				inxs_ratio = circle_line_intersections(s, e, continents[j][k], m);
				tmp.insert(tmp.end(), inxs_ratio.begin(), inxs_ratio.end());
			}
		}

		std::sort(tmp.begin(), tmp.end());
		tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
		sz = tmp.size();
		ld nxt = 0;
		for (Pos& cur : tmp) {
			if (cur.x > nxt) return 0;
			nxt = std::max(nxt, cur.y);
		}
		if (nxt < 1) return 0;
	}
	return 1;
}
ld query() {//concave hull - line cross check binary search
	//init
	std::cin >> C >> N;
	for (int i = 0; i < N; i++) std::cin >> path[i];
	for (int i = 0; i < C; i++) {
		std::cin >> M;
		continents[i].resize(M);
		for (Pos& p : continents[i]) std::cin >> p;
	}
	//bi_search
	ld s = 0, e = 30'000;
	while (!zero(e - s)) {
		ld m = (s + e) / 2;
		if (F(m)) e = m;
		else s = m;
	}
	return s;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	//freopen("../../../input_data/flight/testdata.in", "r", stdin);
	//freopen("../../../input_data/flight/f_out.txt", "w", stdout);
	//T = 1;
	std::cin >> T;
	while (T--) std::cout << query() << "\n";
	return;
}
int main() { solve(); return 0; }//boj3696
//solved it by studying with solution code.
