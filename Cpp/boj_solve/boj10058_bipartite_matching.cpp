#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>
typedef long long ll;
//typedef long double ld;
const int LEN = 110;
int N, M, CNT, match[LEN], arrow[LEN];
bool done[LEN], connected[LEN];

struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	ll Euc() const { return x * x + y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p);
	friend std::ostream& operator << (std::ostream& os, const Pos& p);
} pos[LEN]; const Pos O = { 0, 0 };
std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y << "\n"; return os; }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
std::vector<int> vtx_U, vtx_V;
std::vector<int> G[LEN];
void G_CLEAR() { for (int i = 1; i <= N; i++) G[i].clear(); return; }
bool dfs(int i) {
	done[i] = 1;
	for (const int& j : G[i]) {
		if (!~match[j] || (!done[match[j]] && dfs(match[j]))) {
			match[j] = i;
			return 1;
		}
	}
	return 0;
}
void dfs_mask(int i) {
	done[i] = 1;
	for (const int& j : G[i]) {
		if (done[j] || match[j] == i) continue;
		done[j] = 1;
		dfs_mask(match[j]);
	}
	return;
}
int bi_matching(int u, int v, bool f = 0) {
	int cnt = 2, r = (pos[u] - pos[v]).Euc();
	vtx_U.clear(); vtx_V.clear(); G_CLEAR();
	for (int i = 1; i <= N; i++) {
		if (i == u || i == v) continue;
		if ((pos[i] - pos[u]).Euc() <= r && (pos[i] - pos[v]).Euc() <= r) {
			cnt++; connected[i] = 1;
			int dir = ccw(pos[u], pos[v], pos[i]);
			if (dir < 0) vtx_U.push_back(i);
			if (dir > 0) vtx_V.push_back(i);
		}
	}
	for (const int& i : vtx_U) {
		for (const int& j : vtx_V) {
			if ((pos[i] - pos[j]).Euc() > r) {
				G[i].push_back(j);
				if (f) connected[i] = connected[j] = 0;
			}
		}
	}
	memset(match, -1, sizeof match);
	int mat = 0;
	for (const int& i : vtx_U) {
		memset(done, 0, sizeof done);
		mat += dfs(i);
	}
	if (f) {
		for (int i = 1; i <= N; ++i)
			if (~match[i]) arrow[match[i]] = i;
		memset(done, 0, sizeof done);
		connected[u] = connected[v] = 1;
		for (const int& i : vtx_U)
			if (!arrow[i])
				dfs_mask(i);
		for (const int& i : vtx_U)
			if (done[i])
				connected[i] = 1;
		for (const int& j : vtx_V)
			if (!done[j])
				connected[j] = 1;
		for (int i = 1; i <= N; i++)
			if (connected[i])
				std::cout << i << " ";
	}
	return cnt - mat;
}
void init() {
	std::cin.tie()->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	for (int i = 1; i <= N; i++) std::cin >> pos[i];
	CNT = 0;
	return;
}
void solve() {
	init();
	int R = M * M, U = 0, V = 0;
	for (int i = 1; i < N; i++) {
		for (int j = i + 1; j <= N; j++) {
			if (R < (pos[i] - pos[j]).Euc()) continue;
			int ret = bi_matching(i, j);
			if (CNT < ret) {
				CNT = ret;
				U = i;
				V = j;
			}
		}
	}
	if (!CNT) { std::cout << "1\n1\n"; return; }
	std::cout << CNT << "\n";
	memset(connected, 0, sizeof connected);
	memset(arrow, 0, sizeof arrow);
	bi_matching(U, V, 1);
	return;
}
int main() { solve(); return 0; }//boj10058 Sensor Network
//with help from Jay22
