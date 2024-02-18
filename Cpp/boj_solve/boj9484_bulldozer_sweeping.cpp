#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
const ll INF = 1e17;
const int LEN = 2'001;
int N, M, order[LEN], idx[LEN];

struct Pos {
	ll x, y;
	Pos(ll X, ll Y) : x(X), y(Y) {}
	Pos() : x(0), y(0) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ll& n) const { return { x * n, y * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	ll operator ! () const { return x * y; }
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
} P[LEN];
const Pos O = { 0, 0 }, MAXL = { 0, INF }, MAXR = { INF, 0 }, pivot = { -1, -1 };
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
struct Slope {//segment's two point and slope
	int u, v;//idx
	ll dx, dy;
	Pos p() const { return { u, v }; }
	bool operator < (const Slope& s) const {
		if (dy * s.dx == dx * s.dy) return p() < s.p();
		return dy * s.dx < dx * s.dy;
	}
	bool operator == (const Slope& s) const { return dy * s.dx == dx * s.dy; }
} slopes[LEN * LEN];
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("boj9484_triangle_in.txt", "r", stdin);
	//freopen("boj9484_triangle_out.txt", "w", stdout);
	while (1) {
		memset(order, 0, LEN);
		memset(idx, 0, LEN);
		memset(P, 0, LEN);
		std::cin >> N;
		if (!N) return;

		for (int i = 0; i < N; i++) std::cin >> P[i].x >> P[i].y;
		std::sort(P, P + N);

		for (int i = 0; i < N; i++) order[i] = i, idx[i] = i;
		M = 0;
		ll MIN = INF, MAX = -INF;
		for (int i = 0; i < N; i++) {
			for (int j = i + 1; j < N; j++) {
				if (P[i].x == P[j].x) {
					if (i > 0) {
						MIN = std::min({ MIN, std::abs(cross(P[i], P[j], P[i - 1])) });
						MAX = std::max({ MAX, std::abs(cross(P[i], P[j], P[0])) });
					}
					else if (j < N - 1) {
						MIN = std::min({ MIN, std::abs(cross(P[i], P[j], P[j + 1])) });
						MAX = std::max({ MAX, std::abs(cross(P[i], P[j], P[N - 1])) });
					}
				}
				ll dx = P[j].x - P[i].x;
				ll dy = P[j].y - P[i].y;
				if (!dx) continue;
				if (dx * dy >= 0) dx = std::abs(dx), dy = std::abs(dy);
				else dx = std::abs(dx), dy = -std::abs(dy);
				slopes[M++] = { i, j, dx, dy };
			}
		}
		std::sort(slopes, slopes + M);

		for (int i = 0, j; i < M; i = j) {
			j = i;
			int s = order[slopes[i].u], e = order[slopes[i].u];
			while (j < M && slopes[i] == slopes[j]) {
				int u = slopes[j].u, v = slopes[j].v;
				if (e < order[v]) e = order[v];
				int ou = order[u], ov = order[v];
				if (ou > 0) MIN = std::min(MIN, std::abs(cross(P[u], P[v], P[idx[ou - 1]])));
				if (ov < N - 1) MIN = std::min(MIN, std::abs(cross(P[u], P[v], P[idx[ov + 1]])));
				order[u] = ov; order[v] = ou;
				idx[ou] = v; idx[ov] = u;
				j++;
			}
			if (s == 0 && e == N - 1) MAX = 0;
			if (s > 0) MAX = std::max(MAX, std::abs(cross(P[idx[s]], P[idx[e]], P[idx[0]])));
			if (e < N - 1) MAX = std::max(MAX, std::abs(cross(P[idx[s]], P[idx[e]], P[idx[N - 1]])));
		}
		//std::cout << N << "\n";
		std::cout << (MIN >> 1) << '.' << (MIN & 1 ? "5 " : "0 ");
		std::cout << (MAX >> 1) << '.' << (MAX & 1 ? "5\n" : "0\n");
	}
	return;
}
int main() { solve(); return 0; }//boj9484

/*
3
-1 -1
-1 0
-1 1
0
*/


//#define _CRT_SECURE_NO_WARNINGS
////#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//const ll INF = 1e17;
//const int LEN = 2'001;
//int N, M, order[LEN], idx[LEN];
//ll ret;
//char c;
//
//struct Pos {
//	ll x, y;
//	Pos(ll X, ll Y) : x(X), y(Y) {}
//	Pos() : x(0), y(0) {}
//	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
//	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	Pos operator * (const ll& n) const { return { x * n, y * n }; }
//	Pos operator / (const ll& n) const { return { x / n, y / n }; }
//	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
//	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
//	Pos operator ~ () const { return { -y, x }; }
//	ll operator ! () const { return x * y; }
//	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
//	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
//	//ld mag() const { return hypot(x, y); }
//} P[LEN];
//const Pos O = { 0, 0 }, MAXL = { 0, INF }, MAXR = { INF, 0 }, pivot = { -1, -1 };
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//struct Slope {//segment's two point and slope
//	int u, v;//idx
//	ll dx, dy;
//	Pos p() const { return { u, v }; }
//	bool operator < (const Slope& s) const {
//		//if (dy * s.dx == dx * s.dy) return u == s.u ? v < s.v : u < s.u;
//		if (dy * s.dx == dx * s.dy) return p() < s.p();
//		return dy * s.dx < dx * s.dy;
//	}
//	bool operator == (const Slope& s) const { return dy * s.dx == dx * s.dy; }
//} slopes[LEN * LEN];
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	freopen("boj9484_triangle_in.txt", "r", stdin);
//	freopen("boj9484_triangle_out.txt", "w", stdout);
//	while (1) {
//		memset(order, 0, LEN);
//		memset(idx, 0, LEN);
//		memset(P, 0, LEN);
//		std::cin >> N;
//		if (!N) return;
//		for (int i = 0; i < N; i++) std::cin >> P[i].x >> P[i].y;
//		std::sort(P, P + N);
//		for (int i = 0; i < N; i++) order[i] = i, idx[i] = i;
//
//		M = 0;
//		ll MIN = INF, MAX = -INF;
//		for (int i = 0; i < N; i++) {
//			for (int j = i + 1; j < N; j++) {
//				if (P[i].x == P[j].x) {
//					if (i > 0) {
//						MIN = std::min({ MIN, std::abs(cross(P[i], P[j], P[i - 1])) });
//						MAX = std::max({ MAX, std::abs(cross(P[i], P[j], P[0])) });
//						//std::cout << "i > 0 " << MIN << " " << MAX << "\n";
//					}
//					else if (j < N - 1) {
//						MIN = std::min({ MIN, std::abs(cross(P[i], P[j], P[j + 1])) });
//						MAX = std::max({ MAX, std::abs(cross(P[i], P[j], P[N - 1])) });
//						//std::cout << "j < N - 1 " << MIN << " " << MAX << "\n";
//					}
//				}
//				ll dx = P[j].x - P[i].x;
//				ll dy = P[j].y - P[i].y;
//				if (!dx) continue;
//				if (dx * dy >= 0) dx = std::abs(dx), dy = std::abs(dy);
//				else dx = std::abs(dx), dy = -std::abs(dy);
//				slopes[M++] = { i, j, dx, dy };
//			}
//		}
//		std::sort(slopes, slopes + M);
//		//std::cout << MIN << " " << MAX << "\n";
//
//		for (int i = 0, j; i < M; i = j) {
//			j = i;
//			int s = order[slopes[i].u], e = order[slopes[i].u];
//			//Pos S = P[slopes[i].u], E = P[slopes[i].u];
//			while (j < M && slopes[i] == slopes[j]) {
//				int u = slopes[j].u, v = slopes[j].v;
//				if (e < order[v]) e = order[v];
//				//if (E < P[v]) E = P[v];
//				int ou = order[u], ov = order[v];
//				if (ou > 0)
//					MIN = std::min({ MIN, std::abs(cross(P[u], P[v], P[idx[ou - 1]])) });
//				if (ov < N - 1)
//					MIN = std::min({ MIN, std::abs(cross(P[u], P[v], P[idx[ov + 1]])) });
//				order[u] = ov; order[v] = ou;
//				idx[ou] = v; idx[ov] = u;
//				j++;
//			}
//			//if (s > 0) MAX = std::max({ MAX, std::abs(cross(S, E, P[idx[0]]))});
//			if (s == 0 && e == N - 1) MAX = 0;
//			if (s > 0) MAX = std::max({ MAX, std::abs(cross(P[idx[s]], P[idx[e]], P[idx[0]]))});
//			//if (e < N - 1) MAX = std::max({ MAX, std::abs(cross(S, E, P[idx[N - 1]])) });
//			if (e < N - 1) MAX = std::max({ MAX, std::abs(cross(P[idx[s]], P[idx[e]], P[idx[N - 1]])) });
//			//std::cout << "s e " << s << " " << e << " MIN " << MIN << " MAX " << MAX << "\n";
//
//		}
//		//std::cout << MIN << "\n" << MAX << "\n";
//		std::cout << (MIN >> 1) << '.' << (MIN & 1 ? "5 " : "0 ");
//		std::cout << (MAX >> 1) << '.' << (MAX & 1 ? "5\n" : "0\n");
//	}
//	return;
//}
//int main() { solve(); return 0; }//boj9484
