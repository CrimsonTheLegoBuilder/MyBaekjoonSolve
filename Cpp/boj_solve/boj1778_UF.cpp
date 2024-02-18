#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
typedef long long ll;
const int LEN = 50'001;
int N, M, P[LEN];


int find(int v) {
	if (P[v] < 0) return v;
	int p = find(P[v]);
	return P[v] = p;
}
int join(int x, int y) {
	int i, j;
	i = find(x);
	j = find(y);
	if (i == j) return 0;
	if (P[i] < P[j]) {
		P[i] += P[j];
		P[j] = i;
	}
	else {
		P[j] += P[i];
		P[i] = j;
	}
	return 1;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int t = 1;
	while (1) {
		memset(P, -1, sizeof P);
		std::cin >> N >> M;
		if (!N && !M) return 0;
		int x, y, cnt = 0;
		for (int i = 0; i < M; i++) {
			std::cin >> x >> y;
			join(x, y);
		}
		for (int i = 1; i <= N; i++) {
			cnt += (P[i] < 0);
		}
		std::cout << "Case " << t++ << ": " << cnt << "\n";
	}
}