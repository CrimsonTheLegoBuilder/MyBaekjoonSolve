#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>
typedef int ll;
typedef double ld;
const ld TOL = 1e-7;
const ll INF = 1e9;
const int LEN = 100;
int N;
int board[205][205];
ll MAXX, MAXY, MINX, MINY;

bool z(const ld& x) { return std::abs(x) < TOL; }
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
	ll Man() const { return std::abs(x) + std::abs(y); }
	//ld mag() const { return hypot(x, y); }
}; const Pos O = { 0, 0 };
Pos DRC[4] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
std::vector<Pos> H;
std::queue<Pos> Q;

struct Pdd { ld x, y; Pdd(ld X = 0, ld Y = 0) : x(X), y(Y) {} };

int bfs(int x, int y, const int& f) {
	Q.push(Pos(x, y));
	board[y][x] = f;
	int cnt = 1;
	while (Q.size()) {
		Pos p = Q.front(); Q.pop();
		for (int i = 0; i < 4; i++) {
			Pos w = p + DRC[i];
			if ((MINX <= w.x) && (w.x <= MAXX) && (MINY <= w.y) && (w.y <= MAXY) && !board[w.y][w.x]) {
				Q.push(w);
				board[w.y][w.x] = f;
				cnt++;
			}
		}
	}
	return cnt;
}
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
int ccw(const Pos& d1, const Pos& d2, const Pdd& d3) {
	ld ret = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pdd& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
ll area(std::vector<Pos> H, const int& sz) {
	ll A = 0;
	for (int i = 0; i < sz; i++) A += cross(O, H[i], H[(i + 1) % sz]);
	return A;
}
bool on_seg_strong(const Pos& s1, const Pos& s2, const Pos p) {
	return !ccw(s1, s2, p) && dot(s1, p, s2) >= 0;
}
bool on_seg_strong(const Pos& s1, const Pos& s2, const Pdd p) {
	ld ret = dot(s1, p, s2);
	return !ccw(s1, s2, p) && (ret > 0 || z(ret));
}
bool inner_check(std::vector<Pos>& H, const int& sz, const Pos& p) {
	int cnt = 0;
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
bool inner_check(std::vector<Pos>& H, const int& sz, const Pdd& p) {
	int cnt = 0;
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (cur.y == nxt.y) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return cnt & 1;
}
void sweep(const Pos& cur, const Pos& nxt) {
	Pos d = nxt - cur;
	if (!d.x) {
		if (d.y > 0) for (int i = cur.y; i < nxt.y; i++) board[i][cur.x] = 1;
		else if (d.y < 0) for (int i = nxt.y; i < cur.y; i++) board[i][cur.x - 1] = 1;
		return;
	}
	if (!d.y) {
		if (d.x > 0) for (int j = cur.x; j < nxt.x; j++) board[cur.y - 1][j] = 1;
		else if (d.x < 0) for (int j = nxt.x; j < cur.x; j++) board[cur.y][j] = 1;
		return;
	}
	Pos s = cur, e = nxt;
	if (e < s) std::swap(s, e);
	int j = s.x, i = s.y;
	if (std::abs(s.x - e.x) >= std::abs(s.y - e.y)) {
		if (s.y < e.y) {
			while (i < e.y) {
				while (j < e.x && cross(s, e, Pos(j, i + 1)) > 0 && cross(s, e, Pos(j + 1, i)) < 0) {
					board[i][j] = 1;
					j++;
				}
				if (cross(s, e, Pos(j - 1, i + 2)) > 0 && cross(s, e, Pos(j, i + 1)) < 0) j--;
				i++;
			}
		}
		if (s.y > e.y) {
			while (i > e.y) {
				while (j < e.x && cross(s, e, Pos(j + 1, i)) > 0 && cross(s, e, Pos(j, i - 1)) < 0) {
					board[i - 1][j] = 1;
					j++;
				}
				if (cross(s, e, Pos(j, i - 1)) > 0 && cross(s, e, Pos(j - 1, i - 2)) < 0) j--;
				i--;
			}
		}
	}
	else {
		if (s.y < e.y) {
			while (j < e.x) {
				while (i < e.y && cross(s, e, Pos(j, i + 1)) > 0 && cross(s, e, Pos(j + 1, i)) < 0) {
					board[i][j] = 1;
					i++;
				}
				if (cross(s, e, Pos(j + 1, i)) > 0 && cross(s, e, Pos(j + 2, i - 1)) < 0) i--;
				j++;
			}
		}
		if (s.y > e.y) {
			while (j < e.x) {
				while (i > e.y && cross(s, e, Pos(j + 1, i)) > 0 && cross(s, e, Pos(j, i - 1)) < 0) {
					board[i - 1][j] = 1;
					i--;
				}
				if (cross(s, e, Pos(j + 2, i + 1)) > 0 && cross(s, e, Pos(j + 1, i)) < 0) i++;
				j++;
			}
		}
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("boj6632.txt", "r", stdin);
	//freopen("boj6632.out", "w", stdout);
	while (1) {
		std::cin >> N;
		if (!N) return;
		H.resize(N);
		memset(board, 0, sizeof board);
		MINX = MINY = INF;
		MAXX = MAXY = -INF;
		for (int i = 0; i < N; i++) {
			std::cin >> H[i].x >> H[i].y;
			H[i] += { 101, 101 };
			MINX = std::min(MINX, H[i].x);
			MINY = std::min(MINY, H[i].y);
			MAXX = std::max(MAXX, H[i].x);
			MAXY = std::max(MAXY, H[i].y);
		}
		MINY--, MINX--, MAXX++, MAXY++;
		if (area(H, N) < 0) std::reverse(H.begin(), H.end());
		for (int i = 0; i < N; i++) sweep(H[i], H[(i + 1) % N]);
		ll cnt = 0;
		for (int i = MINY; i < MAXY; i++)
			for (int j = MINX; j < MAXX; j++) {
				if (!board[i][j]) {
					if (inner_check(H, N, Pos(j, i)) && inner_check(H, N, Pdd(j + .5, i + .5))) cnt += bfs(j, i, 2);
					else bfs(j, i, 1);
				}
			}
		std::cout << cnt << "\n";
		//for (int i = MINY; i < MAXY; i++)
		//	for (int j = MINX; j < MAXX; j++)
		//		cnt += board[i][j] == 2;
		//std::cout << cnt << "\n";
	}
}
int main() { solve(); return 0; }//boj6632


/*

4
3 3
13 14
2 2
14 13

3
0 0
10 10
9 10

13
1 7
1 1
2 4
3 1
3 4
5 1
4 4
7 1
5 4
9 1
6 4
11 1
2 7
0

5
0 0
0 -50
-50 -51
-51 -50
-50 0

*/


