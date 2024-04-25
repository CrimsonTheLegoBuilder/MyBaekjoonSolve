//not my code. copied from chika
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
#include <queue>
using namespace std;
typedef long long ll;

int ary[1001] = { 0, };
int degree[1001] = { 0, };
int dp[1001] = { 0, };
vector<int>graph[1001];
int vertex = 0, edge = 0, over = 0;

void topo_sort(std::vector<int> graph[]) {//refer to chika
	queue<int> q;
	for (int i = 1; i <= vertex; ++i) {
		if (!degree[i]) q.push(i);
		}
	while (!q.empty()) {
		int current = q.front();
		q.pop();
		for (auto& i : graph[current]) {
			dp[i] = max(dp[i], dp[current] + ary[i]);
			if (--degree[i] == 0) q.push(i);
		}
	}
	cout << dp[over] << "\n";
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(0);
	int test = 0;
	cin >> test;
	while (test--) {
		cin >> vertex >> edge;
		for (int i = 1; i <= vertex; ++i) {
			cin >> ary[i];
			dp[i] = ary[i];
		}
		for (int i = 0; i < edge; ++i) {
			int start = 0, end = 0;
			cin >> start >> end;
			graph[start].push_back(end);
			degree[end]++;
		}
		cin >> over;
		topo_sort(graph);
		memset(degree, 0, sizeof(degree));
		memset(graph, 0, sizeof(graph));
	}
	return 0;
}