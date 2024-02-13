#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;
const ll MAX = 1e16;
const int LEN = 100;
int N;
int board[205][205];

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
	ll area() const { return std::abs(x * y); }
	//ld mag() const { return hypot(x, y); }
}; const Pos O = { 0, 0 };
std::vector<Pos> H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll area(std::vector<Pos> H, const int& sz) {
	ll A = 0;
	for (int i = 0; i < sz; i++) A += cross(O, H[i], H[(i + 1) % sz]);
	return A;
}
ll GCD(ll x, ll y) { return !y ? x : GCD(y, x % y); }
ll Pick_s_theorem(const Pos& cur, const Pos& nxt) {\
	//Pick`s Theorem : a = i + b/2 - 1
	Pos d = nxt - cur;
	if (!d.x) return 0;
	if (!d.y) return -d.x * cur.y; 
	ll A = d.area();
	ll C = GCD(std::abs(d.x), std::abs(d.y));
	//ll C = std::gcd(std::abs(d.x), std::abs(d.y));
	ll B = d.Man() + C;
	ll inner = (A - B + 2) >> 1;//2i = A - b + 2
	ll box = std::abs(d.x) * std::min(cur.y, nxt.y);
	//printf("box: %d\n", box);
	Pos s = cur, e = nxt;
	if (e < s) std::swap(s, e);
	int i = s.x, j = s.y;
	if (std::abs(s.x - e.x) <= std::abs(s.y - e.y)) {
		if (s.y < e.y) {
			while (i < e.x) {
				while (j < e.y && cross(s, e, Pos(i, j + 1)) > 0 && cross(s, e, Pos(i + 1, j)) < 0) {
					board[i][j] += 1;
					j++;
				}
				if (cross(s, e, Pos(i + 1, j)) > 0 && cross(s, e, Pos(i + 2, j - 1)) < 0) j--;
				i++;
			}
		}
		if (s.y > e.y) {
			while (i < e.x) {
				while (j > e.y && cross(s, e, Pos(i + 1, j)) > 0 && cross(s, e, Pos(i, j - 1)) < 0) {
					board[i][j - 1] += 1;
					j--;
				}
				if (cross(s, e, Pos(i + 2, j + 1)) > 0 && cross(s, e, Pos(i + 1, j)) < 0) j++;
				i++;
			}
		}
	}
	else {
		if (s.y < e.y) {
			while (j < e.y) {
				while (i < e.x && cross(s, e, Pos(i, j + 1)) > 0 && cross(s, e, Pos(i + 1, j)) < 0) {
					board[i][j] += 1;
					i++;
				}
				if (cross(s, e, Pos(i - 1, j + 2)) > 0 && cross(s, e, Pos(i, j + 1)) < 0) i--;
				j++;
			}
		}
		if (s.y > e.y) {
			while (j > e.y) {
				while (i < e.x && cross(s, e, Pos(i + 1, j)) > 0 && cross(s, e, Pos(i, j - 1)) < 0) {
					board[i][j - 1] += 1;
					i++;
				}
				if (cross(s, e, Pos(i, j - 1)) > 0 && cross(s, e, Pos(i - 1, j - 2)) < 0) i--;
				j--;
			}
		}
	}
	//if (d.x < 0) std::cout << inner + C - 1 + box << "\n";
	//else if (d.x > 0) std::cout << inner + C - 1 - A - box << "\n";
	if (d.x < 0) return inner + C - 1 + box;
	else if (d.x > 0) return inner + C - 1 - A - box;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("boj6632.txt", "r", stdin);
	//freopen("boj6632.out", "w", stdout);
	int cnt = 0;
	while (1) {
		std::cin >> N;
		if (!N) return;
		H.resize(N);
		memset(board, 0, sizeof board);
		for (int i = 0; i < N; i++) std::cin >> H[i].x >> H[i].y, H[i].x += 101, H[i].y += 101;
		if (area(H, N) < 0) std::reverse(H.begin(), H.end());
		ll box = 0;
		std::cout << cnt++ << "\n";
		for (int i = 0; i < N; i++) std::cout << H[i].x << " " << H[i].y << "\n";
 		for (int i = 0; i < N; i++) 
			box += Pick_s_theorem(H[i], H[(i + 1) % N]);
		for (int i = 0; i < 205; i++)
			for (int j = 0; j < 205; j++)
				box += board[i][j] >> 1;
		std::cout << box << "\n";
		for (int i = 99; i < 120; i++) {
			for (int j = 99; j < 120; j++) {
				std::cout << board[i][j];
			}
			std::cout << "\n";
		}
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
1 1
2 7
0


*/


