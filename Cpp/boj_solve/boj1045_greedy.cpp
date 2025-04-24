#include <iostream>
#include <queue>
#include <algorithm>
#include <cstring>
#include <cmath>
typedef long long ll;
const int LEN = 50;
std::string MAP[LEN];
int N, M, cnt[LEN];
ll V;

struct Seq {
	int a, b;
	bool operator < (const Seq& s) const { return a == s.a ? b > s.b : a > s.a; }
}cur;
std::priority_queue<Seq> H;

void greedy(int N, int M) {
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			if (MAP[i][j] == 'Y') H.push({ i, j });
			//std::cout << MAP[i][j];
		}
	}
	while (M--) {
		if (H.empty()) { std::cout << "-1\n"; return; }
		cur = H.top(), H.pop();
		V |= (ll)1 << cur.a; V |= (ll)1 << cur.b;
		cnt[cur.a]++; cnt[cur.b]++;
	}
	ll X = ((ll)1 << N) - 1;
	if (X ^ V) { std::cout << "-1\n"; return; }
	for (int i = 0; i < N; i++) { std::cout << cnt[i] << " "; }
	std::cout << "\n";
	return;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	for (int i = 0; i < N; i++) { std::cin >> MAP[i]; }
	greedy(N, M);
	return 0;
}