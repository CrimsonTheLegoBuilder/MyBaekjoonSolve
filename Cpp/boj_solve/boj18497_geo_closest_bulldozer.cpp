#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <cassert>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ll INF = 1e18;
const int LEN = 250;
const ld TOL = 1e-9;
const ld PI = acos(-1);
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL ? 1 : 0; }
inline bool zero(const ld& x) { return !sign(x); }

int N, M, E, OD[LEN], Q[LEN];
ld ANS[LEN][LEN];
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} P[LEN];
const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
struct Seg {
	short int u, v;//idx
	Seg(short int u = 0, short int v = 0) : u(u), v(v) {}
	Pos s() const { return P[v] - P[u]; }
	Pos p() const { return Pos(u, v); }
	int ccw(const Pos& p0) const { return sign(cross(P[u], P[v], p0)); }
	bool operator < (const Seg& S) const {
		assert(O != s()); assert(O != S.s());
		bool f1 = O < s();
		bool f2 = O < S.s();
		if (f1 != f2) return f1;
		ll CCW = s() / S.s();
		return !CCW ? S.ccw(P[u]) > 0 : CCW < 0;
	}
	bool operator == (const Seg& S) const { return s() / S.s() == 0 && s() * S.s() > 0; }
} events[LEN * LEN];
typedef std::vector<Seg> vseg;
typedef std::vector<ld> vld;
vseg segs[LEN][LEN];
vld angs[LEN][LEN];
ld dist(const Pos& p, const Pos& q) { return (p - q).mag(); }
ld sweep(const int& i, const int& j) {
	Pos I, J;
	I = P[i], J = P[j];
	auto theta = [&](const Pos& vec) -> ld {
		Pos K = -~(J - I);
		return rad(K, vec);
		};

	ld total = 0;
	const vseg& SS = segs[i][j];
	const vld& AA = angs[i][j];
	const int sz = SS.size();
	for (int k = 0; k < sz; k++) {
		const Seg& S0 = SS[k], & S1 = SS[(k + 1) % sz];
		const ld& A0 = AA[k], & A1 = AA[(k + 1) % sz];
		if (sign(A0) <= 0) continue;
		ld len = (J - I).mag();
		ld hi = std::min(theta(S0.s()), (ld)(PI * .5));
		ld lo = std::max(theta(S1.s()), -(ld)(PI * .5));
		if (sign(A0 - len) >= 0) {
			total += hi - lo;
			continue;
		}
		ld ratio = std::max(-(ld)1., std::min((ld)1., (ld)(A0 / len)));
		ld phi = acos(ratio);
		if (sign(hi - phi) > 0) {
			if (sign(lo - phi) > 0) total += hi - lo;
			else total += hi - phi;
		}
		if (sign(lo + phi) < 0) {
			if (sign(hi + phi) < 0) total += -(lo - hi);
			else total += -(lo + phi);
		}
	}
	return total;
}
inline void slope_push_back(const int& i1, const int& i2, const int& u, const int& v, const ld& ans) {
	segs[i1][i2].push_back(Seg(u, v));
	angs[i1][i2].push_back(ans);
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	memset(P, 0, sizeof P);
	memset(Q, 0, sizeof Q);
	memset(OD, 0, sizeof OD);
	std::cin >> N;
	if (N == 2) { std::cout << "1.0000000000\n"; return; }

	for (int i = 0; i < N; i++) std::cin >> P[i];
	std::sort(P, P + N);
	for (int i = 0; i < N; i++) OD[i] = i, Q[i] = i;

	E = 0;
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			events[E++] = Seg(i, j);
			events[E++] = Seg(j, i);
		}
	}
	std::sort(events, events + E);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			ANS[i][j] = -INF;
	
	ld ans = INF;
	for (int i = 0; i < N; i++) {
		for (int j = i - 1; j >= 0; j--) {
			ANS[i][j] = ans;
			ans = std::min(ans, dist(P[i], P[j]));
		}
	}
	
	for (int i = 0; i < E; i++) {
		int u = events[i].u, v = events[i].v;
		int ou = OD[u], ov = OD[v];
		OD[u] = ov; OD[v] = ou;
		Q[ou] = v; Q[ov] = u;

		assert(ov - ou == 1);

		ans = ou <= 1 ? INF : ANS[Q[ou - 1]][Q[0]];
		if (ou >= 2) ans = std::min(ans, dist(P[Q[0]], P[Q[ou - 1]]));
		for (int j = ou - 1; j >= 0; j--) {
			ANS[v][Q[j]] = ans;
			slope_push_back(v, Q[j], u, v, ans);
			ans = std::min(ans, dist(P[v], P[Q[j]]));
		}
		for (int j = ov - 1; j >= 0; j--) {
			ANS[u][Q[j]] = ans;
			slope_push_back(u, Q[j], u, v, ans);
			ans = std::min(ans, dist(P[u], P[Q[j]]));
		}

		if (ov < N - 1) {
			ANS[Q[ov + 1]][u] = ans;
			slope_push_back(Q[ov + 1], u, u, v, ans);
			ans = std::min(ans, dist(P[u], P[Q[ov + 1]]));
			ANS[Q[ov + 1]][v] = ans;
			slope_push_back(Q[ov + 1], v, u, v, ans);
			ans = std::min(ans, dist(P[v], P[Q[ov + 1]]));
		}
		for (int j = ov + 2; j < N; j++) {
			ans = std::min(ans, ANS[Q[j]][Q[0]]);
			ANS[Q[j]][u] = ans;
			slope_push_back(Q[j], u, u, v, ans);
			ans = std::min(ans, dist(P[u], P[Q[j]]));
			ANS[Q[j]][v] = ans;
			slope_push_back(Q[j], v, u, v, ans);
			ans = std::min(ans, dist(P[v], P[Q[j]]));
		}

		slope_push_back(v, u, u, v, -INF);
	}

	ld total = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			total += sweep(i, j);
		}
	}

	std::cout << total / (2 * PI) << "\n";
	return;
}
int main() { solve(); return 0; }//boj18497
//Petrozavodsk Programming Camp > Winter 2020 > Day 8: Almost Algoritmic Contest J


//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <vector>
//#include <cassert>
//#include <cstdint>
//typedef long long ll;
////typedef long double ld;
//typedef double ld;
//const ll INF = 1e18;
//const int LEN = 250;
//const ld TOL = 1e-9;
//const ld PI = acos(-1);
//inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
//inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL ? 1 : 0; }
//inline bool zero(const ld& x) { return !sign(x); }
//inline void norm(ld x) {
//	while (sign(x) < 0) x += 2 * PI;
//	while (sign(x - 2 * PI) >= 0) x -= 2 * PI;
//	return;
//}
//
//int N, M, E, OD[LEN], Q[LEN];
//ld ANS[LEN][LEN];
//struct Pos {
//	int x, y;
//	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
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
//	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
//	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
//	Pos operator - () const { return { -x, -y }; }
//	Pos operator ~ () const { return { -y, x }; }
//	Pos operator ! () const { return { y, x }; }
//	ll xy() const { return (ll)x * y; }
//	ll Euc() const { return (ll)x * x + y * y; }
//	int Man() const { return std::abs(x) + std::abs(y); }
//	ld mag() const { return hypot(x, y); }
//	ld rad() const { return atan2(y, x); }
//	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
//	int quad() const { return y > 0 || y == 0 && x >= 0; }
//	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
//	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
//} P[LEN];
//const Pos O = { 0, 0 }, INVAL = { -1, -1 };
//typedef std::vector<Pos> Polygon;
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return ret < 0 ? -1 : !!ret; }
//ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d1) / (d2 - d1).mag(); }
//ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3) / (d2 - d1).mag(); }
//bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
//bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
//bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
//bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2, const bool f = 1) {
//	//f : include end of seg, !f : ignore end of seg
//	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
//	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
//	if (!f) return f1 && f2;
//	bool f3 = on_seg_strong(s1, s2, d1) ||
//		on_seg_strong(s1, s2, d2) ||
//		on_seg_strong(d1, d2, s1) ||
//		on_seg_strong(d1, d2, s2);
//	return (f1 && f2) || f3;
//}
//struct Seg {
//	short int u, v;//idx
//	Seg(short int u = 0, short int v = 0) : u(u), v(v) {}
//	Pos s() const { return P[v] - P[u]; }
//	Pos p() const { return Pos(u, v); }
//	int ccw(const Pos& p0) const { return sign(cross(P[u], P[v], p0)); }
//	bool operator < (const Seg& S) const {
//		assert(O != s()); assert(O != S.s());
//		bool f1 = O < s();
//		bool f2 = O < S.s();
//		if (f1 != f2) return f1;
//		ll CCW = s() / S.s();
//		return !CCW ? S.ccw(P[u]) > 0 : CCW < 0;
//		//return !CCW ? !S.ccw(P[u]) ? p() < S.p() : S.ccw(P[u]) > 0 : CCW < 0;
//	}
//	bool operator == (const Seg& S) const { return s() / S.s() == 0 && s() * S.s() > 0; }
//} events[LEN * LEN];
//struct Slope {
//	Seg seg;
//	ld ans;
//	Slope(Seg se = Seg(0, 0), ld a = 0) : seg(se), ans(a) {}
//};
//typedef std::vector<Slope> vslope;
//typedef std::vector<Seg> vseg;
//typedef std::vector<ld> vld;
//vslope slopes[LEN][LEN];
//ld dist(const Pos& p, const Pos& q) { return (p - q).mag(); }
//ld sweep(const int& i, const int& j) {
//	Pos I, J;
//	I = P[i], J = P[j];
//	auto theta = [&](const Pos& vec) -> ld {
//		Pos K = -~(J - I);
//		return rad(K, vec);
//		};
//	
//	ld total = 0;
//	const vslope& SS = slopes[i][j];
//	const int sz = SS.size();
//	for (int k = 0; k < sz; k++) {
//		const Slope& S0 = SS[k], S1 = SS[(k + 1) % sz];
//		if (sign(S0.ans) <= 0) continue;
//		ld len = (J - I).mag();
//		ld hi = std::min(theta(S0.seg.s()), (ld)(PI * .5));
//		ld lo = std::max(theta(S1.seg.s()), -(ld)(PI * .5));
//		if (sign(S0.ans - len) >= 0) {
//			total += hi - lo;
//			continue;
//		}
//		ld ratio = std::max(-(ld)1., std::min((ld)1., (ld)(S0.ans / len)));
//		ld phi = acos(ratio);
//		if (sign(hi - phi) > 0) {
//			if (sign(lo - phi) > 0) total += hi - lo;
//			else total += hi - phi;
//		}
//		if (sign(lo + phi) < 0) {
//			if (sign(hi + phi) < 0) total += -(lo - hi);
//			else total += -(lo + phi);
//		}
//	}
//	return total;
//}
////ld sweep(const int& i, const int& j) {
////	Pos I, J;
////	I = P[i], J = P[j];
////	Pos K = -~(J - I);
////	auto theta = [&](const Pos& vec) -> ld {
////		return rad(K, vec);
////		};
////	std::cout << "\nDEBUG::\n";
////	std::cout << "DEBUG:: I:: " << I << " J:: " << J << "\n";
////
////	ld total = 0;
////	ld pre = 0;
////	const vslope& SS = slopes[i][j];
////	const int sz = SS.size();
////	ld len = (J - I).mag();
////	for (int k = 0; k < sz; k++) {
////		const Slope& S0 = SS[k], S1 = SS[(k + 1) % sz];
////		std::cout << "FUCK:: len:: " << len << "\n";
////		std::cout << "FUCK:: ans:: " << S0.ans << "\n";
////		std::cout << "SUCK:: S0.u:: " << P[S0.seg.u] << " S0.v:: " << P[S0.seg.v] << "\n";
////		std::cout << "SUCK:: S1.u:: " << P[S1.seg.u] << " S1.v:: " << P[S1.seg.v] << "\n";
////		std::cout << "::SLOPE:: S0:: " << S0.seg.s() << " S1:: " << S1.seg.s() << "\n";
////		if (sign(S0.ans) <= 0) continue;
////		std::cout << "FUCK:: angle cal::\n";
////		ld hi = std::min(theta(S0.seg.s()), (ld)(PI * .5));
////		ld lo = std::max(theta(S1.seg.s()), -(ld)(PI * .5));
////		std::cout << "DEBUG:: vec:: " << J - I << "\n";
////		std::cout << "FUCK:: hi:: " << hi << " lo:: " << lo << "\n";
////		if (sign(S0.ans - len) >= 0) {
////			std::cout << "FUCK:: SEX:: FUCK:: continue:: add:: " << hi - lo << "\n";
////			total += hi - lo;
////			pre = total;
////			continue;
////		}
////		std::cout << "DEBUG:: cos(phi):: " << S0.ans / len << "\n";
////		ld ratio = std::max(-(ld)1., std::min((ld)1., (ld)(S0.ans / len)));
////		std::cout << "DEBUG:: ratio:: " << ratio << "\n";
////		ld phi = acos(ratio);
////		std::cout << "FUCK:: phi:: " << phi << "\n";
////		if (sign(hi - phi) > 0) {
////			if (sign(lo - phi) > 0) total += hi - lo;
////			else total += hi - phi;
////			std::cout << "high:: " << total << "\n";
////		}
////		if (sign(lo + phi) < 0) {
////			if (sign(hi + phi) < 0) total += -(lo - hi);
////			else total += -(lo + phi);
////			std::cout << "low :: " << total << "\n";
////		}
////		std::cout << "FUCK:: SEX:: FUCK:: calculate:: add:: " << total - pre << "\n";
////		pre = total;
////	}
////	return total;
////}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(10);
//	memset(P, 0, sizeof P);
//	memset(Q, 0, sizeof Q);
//	memset(OD, 0, sizeof OD);
//	std::cin >> N;
//	if (N == 2) { std::cout << "1.0000000000\n"; return; }
//
//	//std::cout << "FUCK:: \n";
//	for (int i = 0; i < N; i++) std::cin >> P[i];
//	std::sort(P, P + N);
//	for (int i = 0; i < N; i++) OD[i] = i, Q[i] = i;
//	//std::cout << "FUCK:: init\n";
//
//	//std::cout << "DEBUG::\npos = [\n";
//	//for (int i = 0; i < N; i++) {
//	//	std::cout << "    (" << P[i].x << ", " << P[i].y << "),\n";
//	//}
//	//std::cout << "]\n";
//
//	E = 0;
//	for (int i = 0; i < N; i++) {
//		for (int j = i + 1; j < N; j++) {
//			events[E++] = Seg(i, j);
//			events[E++] = Seg(j, i);
//		}
//	}
//	std::sort(events, events + E);
//	//std::cout << "FUCK:: event sort\n";
//
//	for (int i = 0; i < N; i++)
//		for (int j = 0; j < N; j++)
//			ANS[i][j] = -INF;// , slp[hs(i, j)].reserve(N * N * 2);
//	//std::cout << "FUCK:: ANS init\n";
//
//	ld ans = INF;
//	for (int i = 0; i < N; i++) {
//		for (int j = i - 1; j >= 0; j--) {
//			ANS[i][j] = ans;
//			ans = std::min(ans, dist(P[i], P[j]));
//		}
//	}
//	//std::cout << "FUCK:: ANS cal 1\n";
//
//	//std::cout << "FUCK:: E:: " << E << "\n";
//
//	for (int i = 0; i < E; i++) {
//		//std::cout << "DEBUG:: loop(" << i << ")\n";
//		int u = events[i].u, v = events[i].v;
//		int ou = OD[u], ov = OD[v];
//		OD[u] = ov; OD[v] = ou;
//		Q[ou] = v; Q[ov] = u;
//
//		assert(ov - ou == 1);
//
//		//std::cout << "FUCK:: SEX:: u:: " << u << " v:: " << v << "\n";
//		//std::cout << "FUCK:: P[u]:: " << P[u] << " P[v]:: " << P[v] << "\n";
//		//std::cout << "FUCK:: ou:: " << ou << " ov:: " << ov << "\n";
//
//		ans = ou <= 1 ? INF : ANS[Q[ou - 1]][Q[0]];
//		//std::cout << "SUCK:: Q[0]:: " << Q[0] << "\n";
//		if (ou >= 2) ans = std::min(ans, dist(P[Q[0]], P[Q[ou - 1]]));
//		//if (ou >= 2) std::cout << "FUCK:: Q[OD[v] - 1]:: " << Q[OD[v] - 1] << "\n";
//		//if (ou >= 2) std::cout << "FUCK:: ANS[Q[0]][Q[OD[v] - 1]]:: " << ANS[Q[0]][Q[OD[v] - 1]] << "\n";
//		for (int j = ou - 1; j >= 0; j--) {
//			ANS[v][Q[j]] = ans;
//			slopes[v][Q[j]].push_back(Slope(Seg(u, v), ans));
//			ans = std::min(ans, dist(P[v], P[Q[j]]));
//			//std::cout << "BEBUG:: ANS[v]:: " << ANS[v][Q[j]] << "\n";
//			//std::cout << "BEBUG:: slope :: " << v << " " << Q[j] << "\n";
//		}
//		for (int j = ov - 1; j >= 0; j--) {
//			ANS[u][Q[j]] = ans;
//			slopes[u][Q[j]].push_back(Slope(Seg(u, v), ans));
//			ans = std::min(ans, dist(P[u], P[Q[j]]));
//			//std::cout << "BEBUG:: ANS[u]:: " << ANS[u][Q[j]] << "\n";
//			//std::cout << "BEBUG:: slope :: " << u << " " << Q[j] << "\n";
//		}
//
//		if (ov < N - 1) {
//			ANS[Q[ov + 1]][u] = ans;
//			slopes[Q[ov + 1]][u].push_back(Slope(Seg(u, v), ans));
//			ans = std::min(ans, dist(P[u], P[Q[ov + 1]]));
//			ANS[Q[ov + 1]][v] = ans;
//			slopes[Q[ov + 1]][v].push_back(Slope(Seg(u, v), ans));
//			ans = std::min(ans, dist(P[v], P[Q[ov + 1]]));
//			ans = std::min(ans, ANS[Q[ov + 1]][Q[0]]);
//		}
//		for (int j = ov + 2; j < N; j++) {
//			ans = std::min(ans, ANS[Q[j]][Q[0]]);
//			ANS[Q[j]][u] = ans;
//			slopes[Q[j]][u].push_back(Slope(Seg(u, v), ans));
//			ans = std::min(ans, dist(P[u], P[Q[j]]));
//			ANS[Q[j]][v] = ans;
//			slopes[Q[j]][v].push_back(Slope(Seg(u, v), ans));
//			ans = std::min(ans, dist(P[v], P[Q[j]]));
//		}
//
//		slopes[v][u].push_back(Slope(Seg(u, v), -INF));
//	
//		//for (int j = ou + 1; j < N; j++) {
//		//	ANS[v][Q[j]] = -INF;
//		//	slopes[v][Q[j]].push_back(Slope(Seg(u, v), -INF));
//		//	//std::cout << "BEBUG:: ANS[v]:: -INF\n";
//		//	//std::cout << "BEBUG:: slope rvs:: " << v << " " << Q[j] << "\n";
//		//}
//		//for (int j = ov + 1; j < N; j++) {
//		//	ANS[u][Q[j]] = -INF;
//		//	slopes[u][Q[j]].push_back(Slope(Seg(u, v), -INF));
//		//	//std::cout << "BEBUG:: ANS[u]:: -INF\n";
//		//	//std::cout << "BEBUG:: slope rvs:: " << u << " " << Q[j] << "\n";
//		//}
//		//std::cout << "SHIT\n";
//	}
//	//std::cout << "FUCK:: ANS rotate\n";
//
//	ld total = 0;
//	for (int i = 0; i < N; i++) {
//		for (int j = 0; j < N; j++) {
//			if (i == j) continue;
//			total += sweep(i, j);
//		}
//	}
//	//std::cout << "FUCK:: sweep\n";
//
//	std::cout << total / (2 * PI) << "\n";
//	std::cout << "Size of Pos: " << sizeof(Pos) << " bytes\n";
//	std::cout << "Size of Seg: " << sizeof(Seg) << " bytes\n";
//	std::cout << "Size of Slope: " << sizeof(Slope) << " bytes\n";
//	return;
//}
//int main() { solve(); return 0; }//boj18497
////Petrozavodsk Programming Camp > Winter 2020 > Day 8: Almost Algoritmic Contest J
//
///*
//
//3
//-1000000 0
//1000000 0
//1000000 1
//1.5000004
//
//5
//4 -10
//-3 0
//-8 6
//-9 4
//-5 0
//3.4242720071
//
//5
//-395 310
//597 -448
//-290 300
//-352 -283
//-499 -368
//2.8473923420
//
//30
//-938 702
//950 -131
//-649 190
//431 793
//29 606
//-887 -555
//742 265
//-972 419
//-28 -399
//-316 727
//-654 160
//-74 694
//168 438
//-997 604
//-430 -836
//809 -790
//-581 516
//677 -721
//947 976
//-559 -140
//-692 -902
//-828 927
//-562 711
//258 -715
//761 243
//143 -106
//803 -109
//-760 430
//-264 -146
//270 905
//
//21.4411311891
//
//*/
