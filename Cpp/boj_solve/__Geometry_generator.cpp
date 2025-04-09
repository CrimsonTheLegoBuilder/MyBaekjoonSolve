//#include "testlib.h"
//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <cmath>
//typedef long long ll;
//typedef long double ld;
//typedef std::vector<bool> Vbool;
//typedef std::vector<int> Vint;
//typedef std::vector<ll> Vll;
//typedef std::vector<ld> Vld;
//const ld PI = acos(-1);
//inline int sign(const ld& x) { return x < 0 ? -1 : !!x; }
//inline ld norm(ld th) {
//	while (th < 0) th += 2 * PI;
//	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
//	return th;
//}
//inline int fit(const int& x, const int& lo, const int& hi) { return std::max(lo, std::min(hi, x)); }
//
//#define ANG
////#define CAR
////#define RAD
//
//struct Pos {
//	int x, y;
//	Pos(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
//	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
//	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
//	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	Pos operator * (const int& n) const { return { x * n, y * n }; }
//	Pos operator / (const int& n) const { return { x / n, y / n }; }
//	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
//	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
//	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
//	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
//	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
//	Pos& operator *= (const int& n) { x *= n; y *= n; return *this; }
//	Pos& operator /= (const int& n) { x /= n; y /= n; return *this; }
//	Pos operator - () const { return { -x, -y }; }
//	Pos operator ~ () const { return { -y, x }; }
//	Pos operator ! () const { return { y, x }; }
//	ll xy() const { return (ll)x * y; }
//	ll Euc() const { return (ll)x * x + (ll)y * y; }
//	int Man() const { return std::abs(x) + std::abs(y); }
//	ld mag() const { return hypot(x, y); }
//	ld rad() const { return atan2(y, x); }
//	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
//	Pos rot(const ld& the) const { return { int(x * cos(the) - y * sin(the)), int(x * sin(the) + y * cos(the)) }; }
//	int quad() const { return y > 0 || y == 0 && x >= 0; }
//	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
//	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << "(" << p.x << ", " << p.y << ")"; return os; }
//	void println() const { std::cout << x << " " << y << "\n";  return; }
//	void print() const { std::cout << x << " " << y;  return; }
//}; const Pos O = Pos(0, 0);
//typedef std::vector<Pos> Vpos;
//Pos qry[105][3];
//Vpos P[40];
//bool cmp(const Pos& p, const Pos& q) {
//	bool f0 = O < p;
//	bool f1 = O < q;
//	if (f0 != f1) return f0;
//	ll tq = p / q;
//	return !tq ? p.Euc() <= q.Euc() : tq > 0;
//}
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
//bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
//bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
//std::vector<Pos> graham_scan(std::vector<Pos>& C) {
//	std::vector<Pos> H;
//	if (C.size() < 3) {
//		std::sort(C.begin(), C.end());
//		return C;
//	}
//	std::swap(C[0], *min_element(C.begin(), C.end()));
//	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
//		int ret = ccw(C[0], p, q);
//		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
//		return ret > 0;
//		}
//	);
//	C.erase(unique(C.begin(), C.end()), C.end());
//	int sz = C.size();
//	for (int i = 0; i < sz; i++) {
//		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
//			H.pop_back();
//		H.push_back(C[i]);
//	}
//	return H;
//}
//bool convex_valiator(const std::vector<Pos>& H) {
//	int sz = H.size();
//	for (int i = 0; i < sz; i++) {
//		const Pos& p0 = H[i], & p1 = H[(i + 1) % sz], & p2 = H[(i + 2) % sz];
//		std::cout << "p0:: " << p0 << " p1:: " << p1 << " p2:: " << p2 << "\n";
//		int tq = ccw(p0, p1, p2);
//		std::cout << "dir:: " << (tq > 0 ? "OK" : "BAD") << "\n";
//		if (tq <= 0) return 0;
//	}
//	return 1;
//}
//int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
//	int sz = H.size();
//	if (!sz) return -1;
//	if (sz == 1) return p == H[0] ? 0 : -1;
//	if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
//	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
//	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
//	int s = 0, e = sz - 1, m;
//	while (s + 1 < e) {
//		m = s + e >> 1;
//		if (cross(H[0], H[m], p) >= 0) s = m;
//		else e = m;
//	}
//	if (cross(H[s], H[e], p) > 0) return 1;
//	else if (on_seg_strong(H[s], H[e], p)) return 0;
//	else return -1;
//}
//bool two_polygon_check(const Vpos& U, const Vpos& V) {
//	int szu = U.size();
//	int szv = V.size();
//	for (int i = 0; i < szu; i++) {
//		const Pos& u1 = U[i], & u2 = U[(i + 1) % szu];
//		if (inner_check_bi_search(V, u1) >= 0) return 0;
//		for (int j = 0; j < szv; j++) {
//			const Pos& v1 = V[j], & v2 = V[(j + 1) % szv];
//			if (on_seg_strong(u1, u2, v1)) return 0;
//			if (on_seg_strong(u1, u2, v2)) return 0;
//			if (on_seg_strong(v1, v2, u1)) return 0;
//			if (on_seg_strong(v1, v2, u2)) return 0;
//		}
//	}
//	for (const Pos& v : V) if (inner_check_bi_search(U, v) >= 0) return 0;
//	return 1;
//}
//bool val(const Pos& p, const int& i = -1) {
//	bool f0 = -1000 <= p.x && p.x <= 1000 && -1000 <= p.y && p.y <= 1000;
//	if (i == -1 || !f0) return f0;
//	if (inner_check_bi_search(P[0], p) >= 0) return 0;
//	return 1;
//}
//Pos fit(const Pos& p, const int& x) {
//	return Pos(fit(p.x, -x, x), fit(p.y, -x, x));
//}
//Vpos Polygon_generator(const int& n, const int& i, const int& B, const Pos& cen, const int& tol) {
//	int coo = 900 / B;
//	Vpos C, H;
//	Pos s = Pos(rnd.next(1, coo), 0);
//	int cnt = 0;
//	while (C.size() < n * 10) {
//		cnt = cnt + 1;
//		ld t = rnd.next() * 2 * PI;
//		Pos p = cen + s.rot(t) + Pos(rnd.next(-tol, tol), rnd.next(-tol, tol));
//		if (val(p, i) && std::find(C.begin(), C.end(), p) == C.end())
//			C.push_back(p);
//		if (cnt % 10000 == 0) {
//			std::cout << "Now P[" << i << "] gene...\n";
//			//std::cout << "C.sz:: " << C.size() << "\n";
//		}
//		if (cnt > 100000) return {};
//	}
//	H = graham_scan(C);
//	//std::cout << "Random Hull gene\n";
//	//std::cout << "H.sz:: bfr " << H.size() << "\n";
//	if (H.size() > n) {
//		//std::cout << "Hull is too big\n";
//		int sz = H.size();
//		int diff = sz - n;
//		Vint I;
//		while (I.size() < diff) {
//			int i(rnd.next(0, sz - 1));
//			if (std::find(I.begin(), I.end(), i) == I.end())
//				I.push_back(i);
//		}
//		Vbool F(sz, 0);
//		for (const int& i : I) F[i] = 1;
//		Vpos V;
//		for (int i = 0; i < sz; i++) if (!F[i]) V.push_back(H[i]);
//		H = V;
//	}
//	return H;
//}
//int main(int argc, char* argv[]) {
//	registerGen(argc, argv, 1);
//	std::cout << "generating start\n";
//	Vpos C, PR;
//	int cnt = 0;
//	Pos s = Pos(rnd.next(1, 1000), 0);
//	int B = 33;//sum of inner polygon
//	int sum_b = 100;//sum of virtice of inner polygon
//	int v = sum_b / B;
//	while (C.size() < B * 10) {
//		cnt = (cnt + 1) % 100;
//		ld t = rnd.next() * 2 * PI;
//		Pos p(rnd.next(-850, 850), rnd.next(-850, 850));
//		//Pos p = s.rot(t) + Pos(rnd.next(-200, 200), rnd.next(-200, 200));
//		if (val(p) && std::find(C.begin(), C.end(), p) == C.end() && p.mag() <= 900)
//			C.push_back(p);
//		if (!cnt) {
//			std::cout << "Now C gene...\n";
//			std::cout << "C.sz:: " << C.size() << "\n";
//		}
//	}
//	Pos cen = Pos(Pos(rnd.next(-0, 0), rnd.next(-0, 0)));
//
//#ifdef ANG
//	for (Pos& p : C) p -= cen;
//	std::sort(C.begin(), C.end(), cmp);
//#elif defined CAR
//	std::sort(C.begin(), C.end());
//#endif
//#ifndef RAD
//	int sz = C.size();
//	int tum = sz / B;
//	for (int i = 0; i < B; i++) {
//		int s = i * tum;
//		int e = (i + 1) * tum - 1;
//		Vpos C_;
//		for (int j = s; j < e; j++) {
//			C_.push_back(C[j]);
//		}
//		Vpos H = graham_scan(C_);
//		if (H.size() > v) {
//			std::cout << "Hull is too big\n";
//			int sz = H.size();
//			int diff = sz - v;
//			Vint I;
//			while (I.size() < diff) {
//				int id(rnd.next(0, sz - 1));
//				if (std::find(I.begin(), I.end(), id) == I.end())
//					I.push_back(id);
//			}
//			Vbool F0(sz, 0);
//			for (const int& id : I) F0[id] = 1;
//			Vpos V;
//			for (int id = 0; id < sz; id++) if (!F0[id]) V.push_back(H[id]);
//			H = V;
//		}
//		P[i + 1] = H;
//		std::cout << "P[" << i + 1 << "].sz:: " << P[i + 1].size() << "\n";
//		std::cout << "i:: " << i + 1 << "\n";
//	}
//#endif
//
//#ifdef RAD
//	ld phi = rnd.next();
//	ld t = 2 * PI / B;
//	Vbool F(C.size(), 0);
//	for (int i = 1; i <= B; i++) {
//		ld tr = norm(phi + t * i);
//		ld tl = norm(phi + t * (i + 1));
//		Pos r = cen + Pos(1000, 0).rot(tr);
//		Pos l = cen + Pos(1000, 0).rot(tl);
//		if (ccw(cen, r, l) < 0) std::swap(r, l);
//		Vpos C_;
//		for (int j = 0; j < C.size(); j++) {
//			Pos p = C[j];
//			if (!F[j] && ccw(cen, r, p) > 0 && ccw(cen, l, p) <= 0) {
//				F[j] = 1;
//				C_.push_back(p);
//			}
//		}
//		std::cout << "C_.sz:: " << C_.size() << "\n";
//		Vpos H = graham_scan(C_);
//		if (H.size() > v) {
//			std::cout << "Hull is too big\n";
//			int sz = H.size();
//			int diff = sz - v;
//			Vint I;
//			while (I.size() < diff) {
//				int i(rnd.next(0, sz - 1));
//				if (std::find(I.begin(), I.end(), i) == I.end())
//					I.push_back(i);
//			}
//			Vbool F0(sz, 0);
//			for (const int& i : I) F0[i] = 1;
//			Vpos V;
//			for (int i = 0; i < sz; i++) if (!F0[i]) V.push_back(H[i]);
//			H = V;
//		}
//		P[i] = H;
//		std::cout << "P[" << i << "].sz:: " << P[i].size() << "\n";
//		std::cout << "i:: " << i << "\n";
//	}
//#endif
//
//	int N = 100;
//	C.clear();
//	for (int i = 1; i <= B; i++) {
//		for (Pos& p : P[i]) {
//			int err = rnd.next(10, 200);
//			C.push_back(fit(Pos(p.x + err, p.y), 1000));
//			err = rnd.next(10, 200);
//			C.push_back(fit(Pos(p.x - err, p.y), 1000));
//			err = rnd.next(10, 200);
//			C.push_back(fit(Pos(p.x, p.y + err), 1000));
//			err = rnd.next(10, 200);
//			C.push_back(fit(Pos(p.x, p.y - err), 1000));
//			err = rnd.next(10, 200);
//			C.push_back(fit(Pos(p.x + err, p.y + err), 1000));
//			err = rnd.next(10, 200);
//			C.push_back(fit(Pos(p.x + err, p.y - err), 1000));
//			err = rnd.next(10, 200);
//			C.push_back(fit(Pos(p.x - err, p.y + err), 1000));
//			err = rnd.next(10, 200);
//			C.push_back(fit(Pos(p.x - err, p.y - err), 1000));
//		}
//	}
//	Vpos H = graham_scan(C);
//	std::cout << "H.sz:: " << H.size() << "\n";
//	C.clear();
//	//while (C.size() < N) {
//	//	cnt = (cnt + 1) % 100;
//	//	Pos p(rnd.next(-1000, 1000), rnd.next(-1000, 1000));
//	//	if (std::find(C.begin(), C.end(), p) == C.end() &&
//	//		inner_check_bi_search(H, p) < 0)
//	//		C.push_back(p);
//	//	if (!cnt) {
//	//		std::cout << "Now C gene...\n";
//	//		std::cout << "C.sz:: " << C.size() << "\n";
//	//	}
//	//}
//	//H = graham_scan(C);
//	//if (H.size() > N) {
//	//	std::cout << "Hull is too big\n";
//	//	int sz = H.size();
//	//	int diff = sz - N;
//	//	Vint I;
//	//	while (I.size() < diff) {
//	//		int id(rnd.next(0, sz - 1));
//	//		if (std::find(I.begin(), I.end(), id) == I.end())
//	//			I.push_back(id);
//	//	}
//	//	Vbool F0(sz, 0);
//	//	for (const int& id : I) F0[id] = 1;
//	//	Vpos V;
//	//	for (int id = 0; id < sz; id++) if (!F0[id]) V.push_back(H[id]);
//	//	H = V;
//	//}
//	std::cout << "H.sz:: " << H.size() << "\n";
//	P[0] = H;
//
//	std::cout << "H" << " = [\n";
//	for (int i = 0; i <= B; i++) {
//		if (!P[i].size()) continue;
//		std::cout << "  [";
//		//for (Pos& p : P[i]) {
//		for (int j = 0; j < P[i].size(); j++) {
//			std::cout << "" << P[i][j];
//			if (j < P[i].size() - 1) std::cout << ", ";
//		}
//		std::cout << "],\n";
//	}
//	std::cout << "]\n";
//
//	std::cout << "Q::\n";
//	int Q = 100;
//	for (int q = 0; q < Q; q++) {
//		for (int i = 0; i < 3; i++) {
//			bool f;
//			while (1) {
//				f = 1;
//				Pos p = Pos(rnd.next(-1000, 1000), rnd.next(-1000, 1000));
//				for (int b = 0; b <= B; b++) {
//					if (inner_check_bi_search(P[b], p) == 0) {
//						f = 0; break;
//					}
//				}
//				for (int j = 0; j < i; j++) {
//					if (qry[q][j] == qry[q][i]) {
//						f = 0; break;
//					}
//				}
//				if (f) {
//					qry[q][i] = p;
//					break;
//				}
//			}
//		}
//		std::cout << "query[" << q + 1 << "] done\n";
//	}
//	for (int q = 0; q < Q; q++) {
//		for (int i = 0; i < 3; i++) {
//			std::cout << qry[q][i] << ", ";
//		}
//		std::cout << "\n";
//	}
//
//	std::cout << P[0].size() << "\n";
//	for (const Pos& p : P[0]) p.println();
//	std::cout << B << "\n";
//	for (int i = 1; i <= B; i++) {
//		std::cout << P[i].size() << "\n";
//		for (const Pos& p : P[i]) p.println();
//	}
//	std::cout << Q << "\n";
//	for (int q = 0; q < Q; q++) {
//		qry[q][1].print();
//		std::cout << " ";
//		qry[q][2].print();
//		std::cout << " ";
//		qry[q][3].println();
//	}
//
//	return 0;
//}


#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;
typedef long double ld;
typedef std::vector<bool> Vbool;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
const ld PI = acos(-1);
inline int sign(const ld& x) { return x < 0 ? -1 : !!x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}
inline int fit(const int& x, const int& lo, const int& hi) { return std::max(lo, std::min(hi, x)); }

int pt;
struct Pos {
	int x, y;
	Pos(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const int& n) { x /= n; y /= n; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ll xy() const { return (ll)x * y; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	Pos rot(const ld& the) const { return { int(x * cos(the) - y * sin(the)), int(x * sin(the) + y * cos(the)) }; }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << "(" << p.x << ", " << p.y << ")"; return os; }
	void println() const { std::cout << x << " " << y << "\n";  return; }
	void print() const { std::cout << x << " " << y;  return; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Vpos;
Pos qry[505][3];
Vpos P[500];
bool cmp(const Pos& p, const Pos& q) {
	bool f0 = O < p;
	bool f1 = O < q;
	if (f0 != f1) return f0;
	ll tq = p / q;
	return !tq ? p.Euc() <= q.Euc() : tq > 0;
}
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
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
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
int main() {
	int N = 50;
	Pos p = Pos(-1000, 0);
	std::cout << N << "\n";
	ld t = 2 * PI / N;
	Vpos H;
	//std::cout << N / 3 << "\n";
	for (int i = 0; i < N; i++) {
		Pos p_ = p.rot(t * i);
		p_.println();
		H.push_back(p_);
	}
	P[0] = H; pt++;
	Pos q = Pos(-900, 0);
	Pos r = Pos(-300, 0);
	ld t_ = t / 6;
	for (int i = 0; i < N; i++) {
		//std::cout << "3\n";
		if (i % 3 == 0) {
			Pos p0 = q.rot(t * i + t_);
			Pos p1 = q.rot(t * i + t_ * 5);
			Pos p2 = r.rot(t * i + t_ * 3);
			p0.println();
			p1.println();
			p2.println();
			//std::cout << "pt:: " << pt << "\n";
			P[pt] = { p0, p1, p2 }; pt++;
		}
	}
	//std::cout << "H = [";
	//for (int i = 0; i < pt; i++) {
	//	std::cout << "  [";
	//	int sz = P[i].size();
	//	for (int j = 0; j < sz; j++) {
	//		std::cout << P[i][j];
	//		if (j < sz - 1) std::cout << ", ";
	//	}
	//	std::cout << "],\n";
	//}
	//std::cout << "]";
	std::cout << "FUCK::\n";
	std::cout << N << "\n";
	for (Pos& p : P[0]) p.println();
	std::cout << N / 3 << "\n";
	//std::cout << "FUCK\n";
	for (int i = 1; i <= (N / 3); i++) {
		std::cout << P[i].size() << "\n";
		for (Pos& p : P[i]) p.println();
	}
	Pos R = Pos(-204, 200), G = Pos(-200, 200), B = Pos(-196, 200);
	int Q = 50;
	int w = 8;
	int h = -10;
	std::cout << Q << "\n";
	for (int i = 1; i <= Q; i++) {
		R.print();
		std::cout << " ";
		G.print();
		std::cout << " ";
		B.println();
		R.x += w;
		G.x += w;
		B.x += w;
		if (i % 50 == 0) {
			w *= -1;
			R.y += h;
			G.y += h;
			B.y += h;
		}
	}

	//std::cout << "H = [";
	//for (int i = 0; i < 100; i++) {
	//	Pos p_ = p.rot(t * i);
	//	std::cout << "(" << p_ << "), ";
	//}
	//std::cout << "]";
	return 0;
}