#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <random>
#include <array>
#include <tuple>
typedef long long ll;
typedef double ld;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
int N, M, Q, T;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }

struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	ll operator ^ (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ll xy() const { return (ll)x * y; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
std::vector<Pos> C, H;
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) ^ (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (ll)(d2.x - d1.x) * (ll)(d3.y - d2.y) - (ll)(d2.y - d1.y) * (ll)(d3.x - d2.x);
}
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
}
int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
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
	C.erase(unique(C.begin(), C.end()), C.end());
	if (C.size() <= 2) { for (const Pos& pos : C) H.push_back(pos); }
	else {
		for (int i = 0; i < C.size(); i++) {
			while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
		int s = H.size() + 1;
		for (int i = C.size() - 1; i >= 0; i--) {
			while (H.size() > s && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
	}
	return H;
}

//Pos dooms_pos;

//bool fuck(const Pos& p, const Pos& q) {
//	//std::cout << p << " " << q << " pq\n";
//	if (p == q) return 0;
//	int ret = ccw(dooms_pos, p, q);
//	//std::cout << p << " " << q << " " << ret << " ret\n";
//	if (!ret) return (dooms_pos - p).Euc() < (dooms_pos - q).Euc();
//	//std::cout << "ccw\n";
//	return ret > 0;
//}

std::vector<Pos> graham_scan(std::vector<Pos>& C) {
	std::vector<Pos> H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	//dooms_pos = *min_element(C.begin(), C.end());
	//std::cout << dooms_pos << "\n";
	//std::cout << "what the fuck\n";
	//for (int i = 0; i < C.size(); ++i) {
	//	for (int j = i + 1; j < C.size(); ++j) {
	//		std::cout << C[i] << " " << C[j] << "\n";
	//		std::cout << cross(dooms_pos, C[i], C[j]) << " ccw\n";
	//		std::cout << cross(dooms_pos, C[j], C[i]) << " ccw\n";
	//		std::cout << fuck(C[i], C[j]) << ' ' << fuck(C[j], C[i]) << '\n';
	//std::cout << (dooms_pos - C[i]).Euc() << " " << (dooms_pos - C[j]).Euc() << "\n";
	//	}
	//}

	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
		//std::cout << p << " " << q << " pq\n";
		if (p == q) return 0;
		int ret = ccw(C[0], p, q);
		//std::cout << p << " " << q << " " << ret << " ret\n";
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		//std::cout << (C[0] - p).Euc() << " " << (C[0] - q).Euc() << "\n";
		//std::cout << "ccw\n";
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
std::vector<Pos> minkowski_sum(std::vector<Pos>& A, std::vector<Pos>& B, const bool& f = 0) {
	int sza = A.size();
	int szb = B.size();
	//std::cout << sza << " " << szb << "\n";
	std::vector<Pos> MS;

	if (f) for (Pos& p : B) p *= -1;

	int a = 0, b = 0;
	for (int i = 0; i < sza; i++) if (A[i] < A[a]) a = i;
	for (int j = 0; j < szb; j++) if (B[j] < B[b]) b = j;
	//std::cout << "DEBUG::find_min\n";
	MS.push_back(A[a] + B[b]);
	for (int k = 1; k < sza + szb; k++) {
		Pos a1 = A[(a + 1) % sza] + B[b];
		Pos b1 = B[(b + 1) % szb] + A[a];
		//std::cout << "DEBUG::loop\n";
		//std::cout << ccw(MS.back(), a1, b1) << "\n";
		if (ccw(MS.back(), a1, b1) > 0) MS.push_back(a1), a = (a + 1) % sza;
		else MS.push_back(b1), b = (b + 1) % szb;
	}
	//std::cout << "MS\n";
	//for (Pos& p : MS) std::cout << p << "\n";
	if (f) for (Pos& p : B) p *= -1;
	//std::cout << "loop_end\n";
	return graham_scan(MS);
	//return monotone_chain(MS);
}
std::vector<Pos> UR, UL, DR, DL;
Polygon HUR, HUL, HDR, HDL;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	M = N;
	int x1, y1, x2, y2;
	ll u = INF, r = INF, l = -INF, d = -INF;
	while (M--) {
		std::cin >> x1 >> y1 >> x2 >> y2;
		assert(x1 != x2);
		assert(y1 != y2);
		u = std::min((ll)y2, u);
		r = std::min((ll)x2, r);
		l = std::max((ll)x1, l);
		d = std::max((ll)y1, d);
		UR.push_back(Pos(x2, y2));
		UL.push_back(Pos(x1, y2));
		DR.push_back(Pos(x2, y1));
		DL.push_back(Pos(x1, y1));
	}
	//std::cout << "DEBUG::init\n";
	if (N < 3 || u >= d || r >= l) { std::cout << "possible\n"; return; }
	//std::cout << "DEBUG::imp\n";

	Pos ex1, ex2;

	//std::sort(UR.begin(), UR.end());
	//ex1 = Pos(UR[0].x, MOD), ex2 = Pos(UR.back().x, MOD);
	ex1 = *std::min_element(UR.begin(), UR.end()), ex1.y++;
	ex2 = *std::max_element(UR.begin(), UR.end()), ex2.y++;
	UR.push_back(ex1); UR.push_back(ex2);
	HUR = graham_scan(UR);
	//std::cout << "DEBUG::get_hull\n";

	//std::sort(UL.begin(), UL.end());
	//ex1 = Pos(UL[0].x, MOD), ex2 = Pos(UL.back().x, MOD);
	ex1 = *std::min_element(UL.begin(), UL.end()), ex1.y++;
	ex2 = *std::max_element(UL.begin(), UL.end()), ex2.y++;
	UL.push_back(ex1); UL.push_back(ex2);
	HUL = graham_scan(UL);
	//std::cout << "DEBUG::\n";

	//std::sort(DR.begin(), DR.end());
	//ex1 = Pos(DR[0].x, -MOD), ex2 = Pos(DR.back().x, -MOD);
	ex1 = *std::min_element(DR.begin(), DR.end()), ex1.y--;
	ex2 = *std::max_element(DR.begin(), DR.end()), ex2.y--;
	DR.push_back(ex1); DR.push_back(ex2);
	HDR = graham_scan(DR);
	//std::cout << "DEBUG::\n";

	//std::sort(DL.begin(), DL.end());
	//ex1 = Pos(DL[0].x, -MOD), ex2 = Pos(DL.back().x, -MOD);
	ex1 = *std::min_element(DL.begin(), DL.end()), ex1.y--;
	ex2 = *std::max_element(DL.begin(), DL.end()), ex2.y--;
	DL.push_back(ex1); DL.push_back(ex2);
	HDL = graham_scan(DL);
	//std::cout << "DEBUG::get_hull\n";

	auto SUM1 = minkowski_sum(HUR, HDL, 1);
	//std::cout << "DEBUG::min_sum1\n";
	auto SUM2 = minkowski_sum(HUL, HDR, 1);
	//std::cout << "DEBUG::min_sum2\n";

	if (inner_check_bi_search(SUM1, O) < 1 || inner_check_bi_search(SUM2, O) < 1)
		std::cout << "possible\n";
	else std::cout << "impossible\n";
	//std::cout << "DEBUG::inner\n";
	return;
}
int main() { solve(); return 0; }//Faster Than Light boj26179



//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <cassert>
//#include <vector>
//#include <random>
//#include <array>
//#include <tuple>
//typedef long long ll;
//typedef double ld;
//const ll INF = 1e17;
//const int LEN = 1e5 + 1;
//const ld TOL = 1e-7;
//const ll MOD = 1'000'000'007;
//int N, M, Q, T;
//bool zero(const ld& x) { return std::abs(x) < TOL; }
//int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
//
//struct Pos {
//	int x, y;
//	//int i;
//	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
//	//Pos(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
//	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
//	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
//	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	Pos operator * (const int& n) const { return { x * n, y * n }; }
//	Pos operator / (const int& n) const { return { x / n, y / n }; }
//	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
//	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
//	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
//	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
//	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
//	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
//	Pos operator - () const { return { -x, -y }; }
//	Pos operator ~ () const { return { -y, x }; }
//	Pos operator ! () const { return { y, x }; }
//	ll xy() const { return (ll)x * y; }
//	ll Euc() const { return (ll)x * x + (ll)y * y; }
//	int Man() const { return std::abs(x) + std::abs(y); }
//	ld mag() const { return hypot(x, y); }
//	ld rad() const { return atan2(y, x); }
//	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
//	int quad() const { return y > 0 || y == 0 && x >= 0; }
//	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
//	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
//}; const Pos O = Pos(0, 0);
////bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
////bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
////bool cmpi(const Pos& p, const Pos& q) { return p.i < q.i; }
//typedef std::vector<Pos> Polygon;
//std::vector<Pos> C, H;
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
////ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
////ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ll ret = cross(d1, d2, d3);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
////ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d1) / (d2 - d1).mag(); }
////ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
////	return (d2 - d1) * (d4 - d3) / (d2 - d1).mag();
////}
//bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
//}
////bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
////	return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0;
////}
////int collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
////	return !ccw(d1, d2, d3) && !ccw(d1, d2, d4);
////}
////bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
////	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
////	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
////	return f1 && f2;
////	//bool f3 = on_seg_strong(s1, s2, d1) ||
////	//	on_seg_strong(s1, s2, d2) ||
////	//	on_seg_strong(d1, d2, s1) ||
////	//	on_seg_strong(d1, d2, s2);
////	//return (f1 && f2) || f3;
////}
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
//		if (cross(H[0], H[m], p) > 0) s = m;
//		else e = m;
//	}
//	if (cross(H[s], H[e], p) > 0) return 1;
//	else if (on_seg_strong(H[s], H[e], p)) return 0;
//	else return -1;
//}
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
//std::vector<Pos> minkowski_sum(std::vector<Pos>& A, std::vector<Pos>& B, const bool& f = 0) {
//	int sza = A.size();
//	int szb = B.size();
//	std::vector<Pos> MS;
//
//	if (f) for (Pos& p : B) p *= -1;
//
//	int a = 0, b = 0;
//	for (int i = 0; i < sza; i++) if (A[i] < A[a]) a = i;
//	for (int j = 0; j < szb; j++) if (B[j] < B[b]) b = j;
//	MS.push_back(A[a] + B[b]);
//	for (int k = 1; k < sza + szb; k++) {
//		Pos a1 = A[(a + 1) % sza] + B[b];
//		Pos b1 = B[(b + 1) % szb] + A[a];
//		if (ccw(MS.back(), a1, b1) > 0) MS.push_back(a1), a = (a + 1) % sza;
//		else MS.push_back(b1), b = (b + 1) % szb;
//	}
//
//	if (f) for (Pos& p : B) p *= -1;
//
//	return graham_scan(MS);
//}
//std::vector<Pos> UR, UL, DR, DL;
//Polygon HUR, HUL, HDR, HDL;
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	M = N;
//	int x1, y1, x2, y2;
//	ll u = INF, r = INF, l = -INF, d = -INF;
//	while (M--) {
//		std::cin >> x1 >> y1 >> x2 >> y2;
//		assert(x1 != x2);
//		assert(y1 != y2);
//		u = std::min((ll)y2, u);
//		r = std::min((ll)x2, r);
//		l = std::max((ll)x1, l);
//		d = std::max((ll)y1, d);
//		UR.push_back(Pos(x2, y2));
//		UL.push_back(Pos(x1, y2));
//		DR.push_back(Pos(x2, y1));
//		DL.push_back(Pos(x1, y1));
//	}
//
//	if (N < 3 || u >= d || r >= l) { std::cout << "possible\n"; return; }
//
//	Pos ex1, ex2;
//
//	std::sort(UR.begin(), UR.end());
//	//ex1 = UR[0] + Pos(0, 1), ex2 = UR[UR.size() - 1] + Pos(0, 1);
//	ex1 = Pos(UR[0].x, MOD), ex2 = Pos(UR.back().x, MOD);
//	UR.push_back(ex1); UR.push_back(ex2);
//	HUR = graham_scan(UR);
//
//	std::sort(UL.begin(), UL.end());
//	//ex1 = UL[0] + Pos(0, 1), ex2 = UL[UL.size() - 1] + Pos(0, 1);
//	ex1 = Pos(UL[0].x, MOD), ex2 = Pos(UL.back().x, MOD);
//	UL.push_back(ex1); UL.push_back(ex2);
//	HUL = graham_scan(UL);
//
//	std::sort(DR.begin(), DR.end());
//	//ex1 = DR[0] - Pos(0, 1), ex2 = DR[DR.size() - 1] - Pos(0, 1);
//	ex1 = Pos(DR[0].x, -MOD), ex2 = Pos(DR.back().x, -MOD);
//	DR.push_back(ex1); DR.push_back(ex2);
//	HDR = graham_scan(DR);
//
//	std::sort(DL.begin(), DL.end());
//	//ex1 = DL[0] - Pos(0, 1), ex2 = DL[DL.size() - 1] - Pos(0, 1);
//	ex1 = Pos(DL[0].x, -MOD), ex2 = Pos(DL.back().x, -MOD);
//	DL.push_back(ex1); DL.push_back(ex2);
//	HDL = graham_scan(DL);
//
//	auto SUM1 = minkowski_sum(HUR, HDL, 1);
//	auto SUM2 = minkowski_sum(HUL, HDR, 1);
//
//	if (inner_check_bi_search(SUM1, O) < 1 || inner_check_bi_search(SUM2, O) < 1)
//		std::cout << "possible\n";
//	else std::cout << "impossible\n";
//	return;
//}
//int main() { solve(); return 0; }//Faster Than Light boj26179