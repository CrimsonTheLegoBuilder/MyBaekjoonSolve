#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
typedef long long ll;
const int LEN = 2e5 + 5;
int N;
ll B[LEN];
std::vector<ll> S;

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> B[i];
	for (int i = 0; i < N; i++) {
		S.push_back(B[i]);
		while (S.size() > 1 && S.back() == S[S.size() - 2]) {
			S[S.size() - 2]++;
			S.pop_back();
		}
		//for (ll& I : S) std::cout << I << " ";
		//std::cout << "\n";
	}
	std::cout << S.size() << "\n";
	return;
}
int main() { solve(); return 0; }