#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <queue>
#include <map>
typedef long long ll;
const int LEN = 305;
int N, H, W;
char MAP[LEN][LEN];
ll V[LEN][LEN];

struct Pos {
	ll x, y, c;
	Pos(ll X = 0, ll Y = 0, ll C = 0) : x(X), y(Y), c(C) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
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
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
std::vector<Pos> I = { Pos(1, 0), Pos(-1, 0), Pos(0, 1), Pos(0, -1) };
std::map<Pos, ll> kusuri;
bool valid(const Pos& nxt, int able) {
	bool f1 = 0 <= nxt.x && nxt.x < H && 0 <= nxt.y && nxt.y < W && nxt.c > 0;
	bool f2 = 0;
	if (f1) f2 = MAP[nxt.x][nxt.y] != '#' && V[nxt.x][nxt.y] == able;
	return f2;
}
void bfs(Pos s, int st, int able) {
	std::queue<Pos> Q;
	V[s.x][s.y] = st;
	//std::cout << s.c << "\n";
	Q.push(s);
	while (Q.size()) {
		Pos v = Q.front(); Q.pop();
		for (const Pos& i : I) {
			Pos nxt = v + i;
			nxt.c = v.c - 1;
			if (valid(nxt, able)) V[nxt.x][nxt.y] = st, Q.push(nxt);
		}
	}
	return;
}
void solve() {
	Pos S, T;
	std::cin >> H >> W;
	for (int i = 0; i < H; i++) std::cin >> MAP[i];
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (MAP[i][j] == 'S') S = Pos(i, j, 0);
			if (MAP[i][j] == 'T') T = Pos(i, j, 0);
		}
	}
	//std::cout << "S : " << S << "\n";
	//std::cout << "T : " << T << "\n";
	std::cin >> N;
	ll R, C, w;
	for (int i = 0; i < N; i++) {
		std::cin >> R >> C >> w;
		R--; C--;
		Pos key = Pos(R, C, 0);
		kusuri[key] = w;
	}
	memset(V, 0, sizeof V);
	if (kusuri.find(S) == kusuri.end()) { std::cout << "No\n"; return; }
	//std::cout << "S : " << kusuri[S] << "\n";
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (kusuri.find(Pos(i, j, 0)) != kusuri.end()) {
				bfs(Pos(i, j, kusuri[Pos(i, j, 0)]), 1, 0);
			}
		}
	}
	//for (int i = 0; i < H; i++) {
	//	for (int j = 0; j < W; j++) {
	//		std::cout << V[i][j];
	//	}
	//	std::cout << "\n";
	//}
	S.c = 1e18;
	bfs(S, 2, 1);
	std::cout << (V[T.x][T.y] == 2 ? "Yes\n" : "No\n");
	return;
}
int main() { solve(); return 0; }