#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>
typedef long long ll;
const int LEN = 110;
int N, M, CNT, match[LEN * 10];// , arrow[LEN];
bool done[LEN];// , connected[LEN];

//std::vector<int> vtx_U, vtx_V;
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
//void dfs_mask(int i) {
//	done[i] = 1;
//	for (const int& j : G[i]) {
//		if (done[j] || match[j] == i) continue;
//		done[j] = 1;
//		dfs_mask(match[j]);
//	}
//	return;
//}
int bi_matching() {
	memset(match, -1, sizeof match);
	CNT = 0;
	for (int i = 1; i <= N; i++) {
		memset(done, 0, sizeof done);
		CNT += dfs(i);
	}
	//if (f) {
	//	for (int i = 1; i <= N; ++i)
	//		if (~match[i]) arrow[match[i]] = i;
	//	memset(done, 0, sizeof done);
	//	connected[u] = connected[v] = 1;
	//	for (const int& i : vtx_U)
	//		if (!arrow[i])
	//			dfs_mask(i);
	//	for (const int& i : vtx_U)
	//		if (done[i])
	//			connected[i] = 1;
	//	for (const int& j : vtx_V)
	//		if (!done[j])
	//			connected[j] = 1;
	//	for (int i = 1; i <= N; i++)
	//		if (connected[i])
	//			std::cout << i << " ";
	//}
	return CNT;
}
void init() {
	std::cin.tie()->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	int u, v;
	for (int i = 0; i < M; i++) {
		std::cin >> u >> v;
		G[u].push_back(v);
	}
	CNT = 0;
	return;
}
void solve() {
	init();
	std::cout << bi_matching() << "\n";
	return;
}
int main() { solve(); return 0; }//boj1298
