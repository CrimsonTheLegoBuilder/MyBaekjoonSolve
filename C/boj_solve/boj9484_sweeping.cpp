#include <iostream>
#include <algorithm>
typedef long long ll;
const ll INF = 1e18;
const int LEN = 2'001;

int N, M, pos[LEN];
ll ret;
char c;

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
	//ld mag() const { return hypot(x, y); }
} P[LEN];
const Pos O = { 0, 0 }, MAXL = { 0, INF }, MAXR = { INF, 0 }, pivot = { -1, -1 };
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
struct Slope {  //segment's two point and slope
	int u, v;
	ll dx, dy;
	bool operator < (const Slope& s) const {
		if (dy * s.dx == dx * s.dy) {
			if (u == s.u) return v < s.v;
			return u < s.u;
		}
		return dy * s.dx < dx * s.dy;
	}
	bool operator == (const Slope& s) const { return dy * s.dx == dx * s.dy; }
}slopes[LEN * LEN];
//struct LenNode {  //segment tree for cal subsequence max len
//	ll max, llen, rlen, all;
//	LenNode operator + (const LenNode& r) const {
//		return {
//			std::max({max, r.max, rlen + r.llen}),
//			std::max({llen, all + r.llen}),
//			std::max({rlen + r.all, r.rlen}),
//			all + r.all
//		};
//	}
//}seg_len[LEN * 4];
//void update(int idx, int v, int s = 0, int e = N - 1, int n = 1) {
//	if (e < idx || idx < s) return;
//	if (s == e) {
//		seg_len[n] = { v, v, v, v };
//		return;
//	}
//	int m = s + e >> 1;
//	update(idx, v, s, m, n << 1);
//	update(idx, v, m + 1, e, n << 1 | 1);
//	seg_len[n] = seg_len[n << 1] + seg_len[n << 1 | 1];
//}
//LenNode get_len(int l, int r, int s = 0, int e = N - 1, int n = 1) {
//	if (e < l || r < s) return { 0, 0, 0, 0 };
//	if (l <= s && e <= r) return seg_len[n];
//	int m = s + e >> 1;
//	return get_len(l, r, s, m, n << 1) + get_len(l, r, m + 1, e, n << 1 | 1);
//}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> P[i].x >> P[i].y;
	std::sort(P, P + N);
	ll MIN = INF, MAX = -INF;

	for (int i = 0; i < N; i++) {
		Pos p1 = P[i];
		for (int j = i + 1; j < N; j++) {
			ll dx = P[j].x - P[i].x;
			ll dy = P[j].y - P[i].y;
			if (P[j].x == P[i].x) {
				if (i > 0) {
					MIN = std::min({ MIN, std::abs(cross(p1, P[j], P[i - 1])) });
					MAX = std::min({ MAX, std::abs(cross(p1, P[j], P[0])) });
				}
				else if (j < N - 1) {
					MIN = std::min({ MIN, std::abs(cross(p1, P[j], P[j + 1])) });
					MAX = std::min({ MAX, std::abs(cross(p1, P[j], P[N - 1])) });
				}
			}
			if (!dx) continue;
			if (dx * dy >= 0) {
				dx = std::abs(dx); dy = std::abs(dy);
			}
			else {
				dx = std::abs(dx); dy = -std::abs(dy);
			}
			slopes[M++] = { i, j, dx, dy };
		}
	}
	std::sort(slopes, slopes + M);

	for (int i = 0; i < N; i++) pos[i] = i;  //cur. idx
	//ret = std::max(ret, seg_len[1].max);
	for (int i = 0, j; i < M; i = j) {
		j = i;
		Pos S = P[slopes[i].u], E = P[slopes[i].u];
		int s = slopes[i].u, e = slopes[i].u;
		while (j < M && slopes[i] == slopes[j]) {
			int u = slopes[j].u, v = slopes[j].v;
			if (e < v) e = v;
			if (E < P[pos[v]]) { E = P[pos[v]];}
			if (u > 0) {
				MIN = std::min({ MIN, std::abs(cross(P[pos[u]], P[pos[v]], P[pos[u - 1]]))});
			}
			if (v < N - 1) {
				MIN = std::min({ MIN, std::abs(cross(P[pos[u]], P[pos[v]], P[pos[v + 1]]))});
			}
			//int vx = spot[u].v, vy = spot[v].v;
			//update(x, vy); update(y, vx);
			int x = pos[u], y = pos[v];
			pos[u] = y; pos[v] = x;
			j++;
		}
		if (s > 0) MAX = std::max({ MAX, std::abs(cross(S, E, P[pos[0]])) });
		if (e < N - 1) MAX = std::max({ MAX, std::abs(cross(S, E, P[pos[N - 1]])) });
		
		//ret = std::max(ret, seg_len[1].max);
	}
	std::cout << MIN << "\n" << MAX << "\n";
	std::cout << (MIN >> 1) << '.' << (MIN & 1 ? "5\n" : "0\n");
	std::cout << (MAX >> 1) << '.' << (MAX & 1 ? "5\n" : "0\n");
	return;
}
int main() { solve(); return 0; }//boj9484


//ll V[LEN];
	//for (int i = 0; i < N; i++) {
	//	V[i] = dots[i].v;
	//}


//#include <iostream>
//#include <algorithm>
//typedef long long ll;
//const int LEN = 2'001;
//
//int N, M, pos[LEN], ret;
//ll V[LEN];
//char c;
//
//struct Pos {  //dots
//	ll x, y, v;
//	bool operator < (const Pos& p) const { return (x == p.x ? y < p.y : x < p.x); }
//}dots[LEN];
//struct Slope {  //segment's two point and slope
//	int u, v;
//	ll dx, dy;
//	bool operator < (const Slope& s) const { return dy * s.dx < dx * s.dy; }
//	bool operator == (const Slope& s) const { return dy * s.dx == dx * s.dy; }
//}slopes[LEN * LEN];
//
//struct LenNode {  //segment tree for cal subsequence max len
//	int max, llen, rlen, all;
//	LenNode operator + (const LenNode& r) const {
//		return {
//			std::max({max, r.max, rlen + r.llen}),
//			std::max({llen, all + r.llen}),
//			std::max({rlen + r.all, r.rlen}),
//			all + r.all
//		};
//	}
//}seg_len[LEN * 4];
//
//void update(int idx, int v, int s = 0, int e = N - 1, int n = 1) {
//	if (e < idx || idx < s) return;
//	if (s == e) {
//		seg_len[n] = { v, v, v, v };
//		return;
//	}
//	int m = s + e >> 1;
//	update(idx, v, s, m, n << 1);
//	update(idx, v, m + 1, e, n << 1 | 1);
//	seg_len[n] = seg_len[n << 1] + seg_len[n << 1 | 1];
//}
//LenNode get_len(int l, int r, int s = 0, int e = N - 1, int n = 1) {
//	if (e < l || r < s) return { 0, 0, 0, 0 };
//	if (l <= s && e <= r) return seg_len[n];
//	int m = s + e >> 1;
//	return get_len(l, r, s, m, n << 1) + get_len(l, r, m + 1, e, n << 1 | 1);
//}
//
//
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	for (int i = 0; i < N; i++) {
//		std::cin >> dots[i].x >> dots[i].y >> dots[i].v;
//	}
//	std::sort(dots, dots + N);
//	for (int i = 0; i < N; i++) {
//		V[i] = dots[i].v;
//	}
//
//	for (int i = 0; i < N; i++) {
//		for (int j = i + 1; j < N; j++) {
//			ll dx = dots[j].x - dots[i].x;
//			ll dy = dots[j].y - dots[i].y;
//			if (!dx) continue;
//			if (dx * dy >= 0) {
//				dx = std::abs(dx); dy = std::abs(dy);
//			}
//			else {
//				dx = std::abs(dx); dy = -std::abs(dy);
//			}
//			slopes[M++] = { i, j, dx, dy };
//		}
//	}
//	std::sort(slopes, slopes + M);
//
//	for (int i = 0; i < N; i++) {
//		pos[i] = i;  //cur. idx
//		update(i, V[i]);
//	}
//	ret = std::max(ret, seg_len[1].max);
//
//	for (int i = 0, j; i < M; i = j) {
//		j = i;
//		while (j < M && slopes[i] == slopes[j]) {
//			int u = slopes[j].u, v = slopes[j].v;
//			int x = pos[u], y = pos[v];
//			int vx = V[u], vy = V[v];
//			update(x, vy); update(y, vx);
//			pos[u] = y; pos[v] = x;
//			j++;
//		}
//		ret = std::max(ret, seg_len[1].max);
//	}
//	std::cout << ret;
//	return 0;
//}