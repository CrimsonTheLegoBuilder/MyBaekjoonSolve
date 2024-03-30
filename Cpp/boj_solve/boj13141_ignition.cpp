#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <cstring>
typedef long long ll;
const int INF = 1e9;
const int LEN = 200;
int N, M;

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
	}
	for (int k = 0; k < N; k++)//floyd_warshall
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				min_dist[i][j] = std::min(min_dist[i][j], min_dist[i][k] + min_dist[k][j]);
	return;
}
void solve() {
	init();
	int ret = INF, tmp = 0;
	for (int s = 0; s < N; s++) {
		tmp = 0;
		for (int i = 0; i < N; i++) {
			for (int j = i; j < N; j++) {
				if (max_dist[i][j] == -1) continue;
				int remain = max_dist[i][j], diff;
				diff = min_dist[s][j] - min_dist[s][i];
				if (diff < remain) tmp = std::max(tmp, ((remain - diff) >> 1) + min_dist[s][j]);
				diff = min_dist[s][i] - min_dist[s][j];
				if (diff < remain) tmp = std::max(tmp, ((remain - diff) >> 1) + min_dist[s][i]);
			}
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