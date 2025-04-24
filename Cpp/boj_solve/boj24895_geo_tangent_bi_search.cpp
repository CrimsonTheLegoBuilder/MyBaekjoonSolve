#include <iostream>
#include <algorithm>
typedef long long ll;
const int LEN = 5e4;
const ll MOD = 1'000'000'007;
int N, M;
ll memo[LEN + 1]{ 0 }, ym{ 0 }, hb{ 0 };

struct Pos {
	ll x, y;
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ll& n) const { return { x * n, y * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ll operator / (const Pos& p) const { return x * p.y - y * p.x; }
} Target[LEN], YM, HB, O = { 0, 0 };

ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	Pos cur = d2 - d1, nxt = d4 - d3;
	ll ret = cur / nxt;
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
}
void get_area() {
	memo[0] = 0;
	for (int i = 0; i < N; i++) {
		Pos cur = Target[i], nxt = Target[(i + 1) % N];
		memo[i + 1] = cross(O, cur, nxt) + memo[i];
	}
	//for (int i = 0; i <= N; i++) std::cout << "memo[" << i << "] " << memo[i] << "\n";
	return;
}
int inner_check_bi_search(const Pos& p, Pos H[], int h) {
	if (h < 3 || cross(H[0], H[1], p) < 0 || cross(H[0], H[h - 1], p) > 0) return -1;
	if (on_seg(H[0], H[1], p) || on_seg(H[0], H[h - 1], p)) return 0;
	int s = 0, e = h - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return 1;
	else if (on_seg(H[s], H[e], p)) return 0;
	return -1;
}
Pos find_tangent_bi_search(Pos H[], const Pos& p) {
	int i1{ 0 }, i2{ 0 };
	int ccw1 = ccw(p, H[0], H[1]), ccwN = ccw(p, H[0], H[N - 1]);
	//ll ccw_1 = cross(p, H[0], H[1]), ccw_N = cross(p, H[0], H[N - 1]);
	//std::cout << "DEBUG p: " << p.x << " " << p.y << " H[0]: " << H[0].x << " " << H[0].y << " H[1]:" << H[1].x << " " << H[1].y << " H[N-1]:" << H[N - 1].x << " " << H[N - 1].y << "\n";
	if (ccw1 * ccwN >= 0) {
		//std::cout << "tangent!! " << ccw_1 << " " << ccw_N << "\n";
		i1 = 0;
		if (!ccw1 && dot(p, H[1], H[0]) > 0) i1 = 1;
		if (!ccwN && dot(p, H[N - 1], H[0]) > 0) i1 = N - 1;
		int s = 0, e = N - 1, m;
		if (!ccw1) s += 1;
		if (!ccwN) e -= 1;
		bool f = ccw(p, H[s], H[s + 1]) >= 0;
		while (s < e) {
			m = s + e >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % N];
			if (!f) std::swap(p1, cur);
			if (ccw(p1, cur, nxt) > 0) s = m + 1;
			else e = m;
		}
		i2 = s;
		if (!ccw(p, H[i2], H[(i2 + 1)]) && dot(p, H[(i2 + 1)], H[i2]) > 0) i2 = (i2 + 1) % N;
	}
	else {
		//std::cout << "tangent?? " << ccw_1 << " " << ccw_N << "\n";
		int s = 0, e = N - 1, k, m;
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
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % N];
			if (!f) std::swap(p1, cur);
			if (ccw(p1, cur, nxt) > 0) s1 = m + 1;
			else e1 = m;
		}
		i1 = s1;
		if (!ccw(p, H[i1], H[(i1 + 1) % N]) && dot(p, H[(i1 + 1) % N], H[i1]) > 0) i1 = (i1 + 1) % N;
		int s2 = e, e2 = N - 1;
		while (s2 < e2) {
			m = s2 + e2 >> 1;
			Pos p1 = p, cur = H[m], nxt = H[(m + 1) % N];
			if (!f) std::swap(p1, cur);
			if (ccw(p1, cur, nxt) < 0) s2 = m + 1;
			else e2 = m;
		}
		i2 = s2;
		if (!ccw(p, H[i2], H[(i2 + 1) % N]) && dot(p, H[(i2 + 1) % N], H[i2]) > 0) i2 = (i2 + 1) % N;
	}
	return { i1, i2 };
}
ll get_score(const Pos& shoot) {
	int f = inner_check_bi_search(shoot, Target, N);
	//std::cout << f << "\n";
	if (!f) return 0;
	if (f > 0) return memo[N];
	ll score{ 0 };
	Pos tangent = find_tangent_bi_search(Target, shoot);
	//std::cout << "OUT tangent.. " << tangent.x << " " << tangent.y << "\n";
	ll i1 = tangent.x, i2 = tangent.y;
	if (i2 < i1) std::swap(i1, i2);
	score = memo[i2] - memo[i1] - cross(O, Target[i1], Target[i2]);
	score = std::min(score, memo[N] - score);
	//std::cout << memo[N] << " " << memo[i2] << " " << memo[i1] << " " << cross(O, Target[i1], Target[i2 - 1]) << " " << score << "\n";
	return score;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	for (int i = 0; i < N; i++) { std::cin >> Target[i].x >> Target[i].y; }
	get_area();
	for (int i = 0; i < M; i++) {
		std::cin >> YM.x >> YM.y;
		ym += get_score(YM); ym %= MOD;
	}
	//std::cout << "turn change\n";
	for (int i = 0; i < M; i++) { 
		std::cin >> HB.x >> HB.y;
		hb += get_score(HB); hb %= MOD;
	}
	std::cout << (ym == hb ? "same\n" : ym > hb ? "ym\n" : "hb\n");
	std::cout << ym << " " << hb << "\n";
}
int main() { solve(); return 0; }//boj24895

/*

4 1
1 1
-1 1
-1 -1
1 -1
0 0
1 2

3 1
2 2
-2 2
2 -2
1 1
-2 -2

6 6
0 -3
2 -1
2 1
0 3
-2 1
-2 -1
2 3
-2 3
-2 -3
2 -3
3 0
-3 0
4 0
-4 0
3 3
-3 3
-3 -3
3 -3
ans hb 0 48

*/