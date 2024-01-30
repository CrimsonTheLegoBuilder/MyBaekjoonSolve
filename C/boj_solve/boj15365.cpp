#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>
typedef long long ll;
const ll INF = 1e17;
const int LEN = 2e3 + 1;
int N, M;

struct Pos {
	ll x, y;
	bool operator < (const Pos& p) const {
		return y * p.x == x * p.y ? (x * x + y * y) < (p.x * p.x + p.y * p.y) : y * p.x > x * p.y;
	}
};
const Pos O = { 0, 0 }, MAX = { INF, INF };
std::vector<Pos> H[LEN];
struct Info {
	int b;
	ll t, c;
	ll tc;
	bool operator < (const Info& i) const { return tc > i.tc; }
};
std::priority_queue<Info> Q;
void dijkstra() {
	for (int i = 0; i < LEN; i++) H[i].push_back(MAX);
	return;
}