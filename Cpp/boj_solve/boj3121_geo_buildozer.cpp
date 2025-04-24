#include <iostream>
#include <algorithm>
typedef long long ll;
const int LEN = 1'001;

int N, M, idx[LEN], color[LEN], ret;
char c;

struct Pos {  //dots
	ll x, y;
	int i;
	bool operator < (const Pos& p) const { return (x == p.x ? y < p.y : x < p.x); }
}dots[LEN];
struct Slope {  //segment's two point and slope
	int u, v;
	ll dx, dy;
	bool operator < (const Slope& s) const { return dy * s.dx < dx * s.dy; }
	bool operator == (const Slope& s) const { return dy * s.dx == dx * s.dy; }
}slopes[LEN * LEN];
struct LenNode {  //segment tree for cal subsequence max len
	int max, llen, rlen, all;
	LenNode operator + (const LenNode& r) const {
		return {
			std::max({max, r.max, rlen + r.llen}),
			all ? max + r.llen : llen,
			r.all ? rlen + r.max : r.rlen,
			all & r.all
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
		std::cin >> dots[i].x >> dots[i].y >> c;
		dots[i].i = i;
		color[i] = c == 'R';
	}
	std::sort(dots, dots + N);

	for (int i = 0; i < N; i++) {
		int u = dots[i].i;
		for (int j = i + 1; j < N; j++) {
			int v = dots[j].i;
			ll dx = dots[j].x - dots[i].x;
			ll dy = dots[j].y - dots[i].y;
			if (!dx) continue;
			if (dx * dy >= 0) {
				dx = std::abs(dx); dy = std::abs(dy);
			}
			else {
				dx = std::abs(dx); dy = -std::abs(dy);
			}
			slopes[M++] = { u, v, dx, dy };
		}
	}
	std::sort(slopes, slopes + M);

	for (int i = 0; i < N; i++) {
		idx[dots[i].i] = i;  //cur. idx
		update(i, color[dots[i].i]);
	}
	ret = std::max(ret, seg_len[1].max);

	for (int i = 0, j; i < M; i = j) {
		j = i;
		while (j < M && slopes[i] == slopes[j]) {
			int u = slopes[j].u, v = slopes[j].v;
			int x = idx[u], y = idx[v];
			int vx = color[u], vy = color[v];
			update(x, vy); update(y, vx);
			idx[u] = y; idx[v] = x;
			j++;
		}
		ret = std::max(ret, seg_len[1].max);
	}
	std::cout << ret;
	return 0;
}

/*
#include <iostream>
#include <algorithm>
typedef long long ll;
const int LEN = 1'001;

int N, M, idx[LEN], color[LEN], ret;
struct Pos {  //dots
	ll x, y;  //coord
	int i;    //idx
	char c;   //color
	bool operator < (const Pos& p) const { return (x == p.x ? y < p.y : x < p.x); }
}dots[LEN];
struct Slope {  //segment's two point and slope
	int u, v;
	ll dx, dy;
	bool operator < (const Slope& s) const {  //stable sort
		if (dy * s.dx == dx * s.dy) {
			if (u == s.u) { return v < s.v; }
			return u < s.u;
		}
		return dy * s.dx < dx * s.dy;
	}
	bool operator == (const Slope& s) const { return dy * s.dx == dx * s.dy; }
}slopes[LEN * LEN];
struct LenNode {  //segment tree for cal subsequence' max len
	int max, llen, rlen, all;
	LenNode operator + (const LenNode& r) const {
		return {
			std::max({ max, r.max, rlen + r.llen }),
			all ? max + r.llen : llen,
			r.all ? rlen + r.max : r.rlen,
			all & r.all
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
		std::cin >> dots[i].x >> dots[i].y >> dots[i].c;
	}
	std::sort(dots, dots + N);
	for (int i = 0; i < N; i++) {
		dots[i].i = i;  //numbering
		color[i] = dots[i].c == 'R';
	}

	for (int i = 0; i < N; i++) {
		int u = dots[i].i;
		for (int j = i + 1; j < N; j++) {
			int v = dots[j].i;
			ll dx = dots[j].x - dots[i].x;
			ll dy = dots[j].y - dots[i].y;
			if (!dx) continue;
			if (dx * dy >= 0) {
				dx = std::abs(dx); dy = std::abs(dy);  //1st quadrant
			}
			else {
				dx = std::abs(dx); dy = -std::abs(dy);  //2nd quadrant
			}
			slopes[M++] = { u, v, dx, dy };
		}
	}
	std::sort(slopes, slopes + M);  //sort by slope - idx

	//for (int i = 0; i < N; i++) {
	//	idx[dots[i].i] = i;  //cur. idx
	//	update(i, color[dots[i].i]);
	//}
	for (int i = 0; i < N; i++) {
		idx[i] = i;  //cur. idx
		update(i, color[i]);
	}
	ret = std::max(ret, seg_len[1].max);

	for (int i = 0, j; i < M; i = j) {
		j = i;
		while (j < M && slopes[i] == slopes[j]) {
			int u = slopes[j].u, v = slopes[j].v;
			int x = idx[u], y = idx[v];  //Pos u & v's current pos
			int vx = color[u], vy = color[v];
			update(x, vy); update(y, vx);
			idx[u] = y; idx[v] = x;
			j++;
		}
		ret = std::max(ret, seg_len[1].max);
	}
	std::cout << ret;
	return 0;
}
*/