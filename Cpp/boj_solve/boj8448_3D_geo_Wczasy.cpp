#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const int LEN = 1e6 + 5;
const ll INF = 1e18;

int N;
ll DIR[8];
ll P[LEN][3];
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	ll tmp;
	for (int i = 0; i < 8; i++) DIR[i] = -INF;
	for (int i = 0; i < N; i++) {
		std::cin >> P[i][0] >> P[i][1] >> P[i][2];
		for (int j = 0; j < 8; j++) {
			tmp = 0;
			for (int k = 0; k < 3; k++)
				tmp += P[i][k] * (ll)(j & 1 << k ? 1 : -1);
			DIR[j] = std::max(DIR[j], tmp);
		}
	}
	for (int i = 0; i < N; i++) {
		ll ret = 0;
		for (int j = 0; j < 8; j++) {
			tmp = DIR[j];
			for (int k = 0; k < 3; k++)
				tmp += P[i][k] * (ll)(j & 1 << k ? -1 : 1);
			ret = std::max(ret, tmp);
		}
		std::cout << ret << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj8448