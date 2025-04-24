#include <iostream>
#include <algorithm>
typedef long long ll;
const int LEN = 2'001;

int N, M, pos[LEN];
ll ret;
char c;

struct Pos {  //spots coord' and value
	ll x, y, v;
	bool operator < (const Pos& p) const { return (x == p.x ? y < p.y : x < p.x); }
}spot[LEN];
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

struct LenNode {  //segment tree for cal subsequence max len
	ll max, llen, rlen, all;
	LenNode operator + (const LenNode& r) const {
		return {
			std::max({max, r.max, rlen + r.llen}),
			std::max({llen, all + r.llen}),
			std::max({rlen + r.all, r.rlen}),
			all + r.all
		};
	}
}seg_len[LEN * 4];

void update(int idx, int v, int s = 0, int e = N - 1, int n = 1) {
	if (e < idx || idx < s) return;
	if (s == e) {
		seg_len[n] = { v, v, v, v };
		return;
	}
	int m = s + e >> 1;
	update(idx, v, s, m, n << 1);
	update(idx, v, m + 1, e, n << 1 | 1);
	seg_len[n] = seg_len[n << 1] + seg_len[n << 1 | 1];
}
LenNode get_len(int l, int r, int s = 0, int e = N - 1, int n = 1) {
	if (e < l || r < s) return { 0, 0, 0, 0 };
	if (l <= s && e <= r) return seg_len[n];
	int m = s + e >> 1;
	return get_len(l, r, s, m, n << 1) + get_len(l, r, m + 1, e, n << 1 | 1);
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> spot[i].x >> spot[i].y >> spot[i].v;
	}
	std::sort(spot, spot + N);

	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			ll dx = spot[j].x - spot[i].x;
			ll dy = spot[j].y - spot[i].y;
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
	//std::stable_sort(slopes, slopes + M);
	std::sort(slopes, slopes + M);

	for (int i = 0; i < N; i++) {
		pos[i] = i;  //cur. idx
		update(i, spot[i].v);
	}
	ret = std::max(ret, seg_len[1].max);

	for (int i = 0, j; i < M; i = j) {
		j = i;
		while (j < M && slopes[i] == slopes[j]) {
			int u = slopes[j].u, v = slopes[j].v;
			int x = pos[u], y = pos[v];
			int vx = spot[u].v, vy = spot[v].v;
			update(x, vy); update(y, vx);
			pos[u] = y; pos[v] = x;
			j++;
		}
		ret = std::max(ret, seg_len[1].max);
	}
	std::cout << ret;
	return 0;
}


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