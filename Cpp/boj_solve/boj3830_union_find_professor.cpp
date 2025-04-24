#include <iostream>
#include <cstring>
const int LEN = 100001;
typedef long long ll;

int W[LEN], P[LEN];
int find(int x) {
	int y, g;
	if (P[x] < 0) return x;
	y = P[x];
	g = find(y);
	W[x] += W[y];
	return P[x] = g;
}
int join(int x, int y, int w) {
	int i, j;
	i = find(x);
	j = find(y);
	if (i == j) return 0;
	if (P[i] < P[j]) {
		P[i] += P[j];
		P[j] = i;
		W[j] = - W[y] + w + W[x];
	}
	else {
		P[j] += P[i];
		P[i] = j;
		W[i] = W[y] - w - W[x];
	}
	return 1;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	int N, M, flag = 1;
	while (1) {
		memset(P, -1, sizeof(P));
		memset(W, 0, sizeof(W));
		std::cin >> N >> M;
		if (!N && !M) return 0;
		for (int i = 0; i < M; i++) {
			char c;
			std::cin >> c;
			if (c == '!') {
				int a, b, w;
				std::cin >> a >> b >> w;
				join(a, b, w);
			}
			else {
				int a, b;
				std::cin >> a >> b;
				if (find(a) == find(b)) std::cout << (W[b] - W[a]) << "\n";
				else std::cout << "UNKNOWN\n";
			}
		}
	}
	return 0;
}