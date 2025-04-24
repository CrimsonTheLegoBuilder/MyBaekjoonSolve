#include <iostream>
#include <algorithm>
#include <vector>
typedef long long ll;
const int LEN = 10'001;
int N, D, DP[LEN];

struct Info {
	int s, e, c;
	bool operator < (const Info& I) const {
		return e < I.e;
	}
};
std::vector<Info> G;

int main() {
	int s, e, d;
	std::cin >> N >> D;
	for (int i = 0; i < N; i++) {
		std::cin >> s >> e >> d;
		G.push_back({ s, e, d });
		//std::cout << s << " " << e << " " << d << "\n";
	}
	for (int i = 1; i <= D; i++) {
		DP[i] = i;
	}
	std::sort(G.begin(), G.end());
	for (const Info& I : G) {
		DP[I.e] = std::min(DP[I.e], DP[I.s] + I.c);
		for (int i = I.e; i <= D; i++) {
			DP[i] = std::min({ DP[i], DP[i - 1] + 1 });
		}
	}
	//for (int i = 1; i < D; i++) {
	//	DP[i] = std::min(DP[i], DP[i-1] + 1);
	//}
	//for (int i = 0; i <= D; i++) {
	//	std::cout << i << "/" << DP[i] << " ";
	//}
	std::cout << DP[D];

	return 0;
}

/*
2 20
0 10 5
2 7 3
*/