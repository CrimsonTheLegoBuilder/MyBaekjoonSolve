#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <cstring>
#include <vector>
#include <unordered_set>
typedef long long ll;
const int INF = 1e9;
const int LEN = 200;
int N, M;

struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { -x, -y }; }
	ll xy() const { return (ll)x * y; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
std::vector<Pos> P;
//std::unordered_set<Pos> S;

int min_dist[LEN][LEN], max_dist[LEN][LEN];
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	memset(max_dist, -1, sizeof max_dist);
	for (int i = 0; i < N; i++)
		for (int j = i + 1; j < N; j++)
			min_dist[i][j] = min_dist[j][i] = INF;
	int s, e, l;
	while (M--) {
		std::cin >> s >> e >> l;
		s--, e--;
		l <<= 1;
		if (min_dist[s][e] > l) min_dist[s][e] = min_dist[e][s] = l;
		if (max_dist[s][e] < l) max_dist[s][e] = max_dist[e][s] = l;
		if (e < s) std::swap(s, e);
		P.push_back({ s, e });
		//S.insert({ s, e });
	}
	for (int k = 0; k < N; k++)//floyd_warshall
		for (int i = 0; i < N; i++)
			for (int j = i + 1; j < N; j++)
				min_dist[i][j] = min_dist[j][i] = std::min(min_dist[i][j], min_dist[i][k] + min_dist[k][j]);
	return;
}
void solve() {
	init();
	//int ret = INF, tmp = 0;
	//for (int s = 0; s < N; s++) {
	//	tmp = 0;
	//	for (int i = 0; i < N; i++) {
	//		for (int j = i; j < N; j++) {
	//			if (max_dist[i][j] == -1) continue;
	//			int remain = max_dist[i][j], diff;
	//			diff = min_dist[s][j] - min_dist[s][i];
	//			if (diff < remain) tmp = std::max(tmp, ((remain - diff) >> 1) + min_dist[s][j]);
	//			diff = min_dist[s][i] - min_dist[s][j];
	//			if (diff < remain) tmp = std::max(tmp, ((remain - diff) >> 1) + min_dist[s][i]);
	//		}
	//	}
	//	ret = std::min(ret, tmp);
	//}
	P.erase(unique(P.begin(), P.end()), P.end());
	int ret = INF, tmp = 0;
	for (int s = 0; s < N; s++) {
		tmp = 0;
		for (const Pos& p : P) {
			if (max_dist[p.x][p.y] == -1) continue;
			int remain = max_dist[p.x][p.y], diff;
			diff = min_dist[s][p.y] - min_dist[s][p.x];
			if (diff < remain) tmp = std::max(tmp, ((remain - diff) >> 1) + min_dist[s][p.y]);
			diff = min_dist[s][p.x] - min_dist[s][p.y];
			if (diff < remain) tmp = std::max(tmp, ((remain - diff) >> 1) + min_dist[s][p.x]);
		}
		ret = std::min(ret, tmp);
	}
	std::cout << (ret >> 1) << (ret & 1 ? ".5\n" : ".0\n");
	return;
}
int main() { solve(); return 0; }//boj13141 ignition


				//if (diff >= remain) continue;
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cassert>
//#include <cstring>
//typedef long long ll;
//const ll INF = 1e17;
//const int LEN = 205;
//int N, M;
//
//ll min_dist[LEN][LEN];
//ll max_dist[LEN][LEN];
//void init() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N >> M;
//	int s, e; ll l;
//	memset(max_dist, -1, sizeof max_dist);
//	for (int i = 0; i <= N; i++)
//		for (int j = i + 1; j <= N; j++)
//			min_dist[i][j] = min_dist[j][i] = INF;
//	while (M--) {
//		std::cin >> s >> e >> l;
//		s--, e--;
//		l <<= 1;
//		if (min_dist[s][e] > l) min_dist[s][e] = min_dist[e][s] = l;
//		if (max_dist[s][e] < l) max_dist[s][e] = max_dist[e][s] = l;
//	}
//	//for (int i = 0; i < N; i++) {
//	//	for (int j = 0; j < N; j++) {
//	//		std::cout << min_dist[i][j] << " ";
//	//	}
//	//	std::cout << "\n";
//	//}
//	//for (int i = 0; i < N; i++) {
//	//	for (int j = 0; j < N; j++) {
//	//		std::cout << max_dist[i][j] << " ";
//	//	}
//	//	std::cout << "\n";
//	//}
//	for (int k = 0; k < N; k++)
//		for (int i = 0; i < N; i++)
//			for (int j = 0; j < N; j++)
//				min_dist[i][j] = std::min(min_dist[i][j], min_dist[i][k] + min_dist[k][j]);
//	return;
//}
//void solve() {
//	init();
//	ll ret = INF, tmp = 0;
//	for (int s = 0; s < N; s++) {
//		tmp = 0;
//		for (int i = 0; i < N; i++) {
//			for (int j = 0; j < N; j++) {
//				if (max_dist[i][j] == -1) continue;
//				ll rem = max_dist[i][j];
//				ll diff = min_dist[s][j] - min_dist[s][i];
//				if (diff >= rem) continue;
//				tmp = std::max(tmp, ((rem - diff) >> 1) + min_dist[s][j]);
//			}
//		}
//		ret = std::min(ret, tmp);
//	}
//	std::cout << (ret >> 1) << (ret & 1 ? ".5\n" : ".0\n");
//}
//int main() { solve(); return 0; }//boj13141 ignition