#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
typedef long long ll;
const int LEN = 5e5 + 2;
int N;
ll H[LEN];
std::vector<int> stack;

struct Num {
	ll h;
	int n;
};
std::vector<Num> S;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) std::cin >> H[i];
	ll cnt = 0;
	for (int i = 1; i <= N; i++) {
		while (S.size() && H[i] <= S.back().h) {
			if (H[i] < S.back().h) S.pop_back();
			if (H[i] == S.back().h) S.back().n++;
		}
		stack.push_back(i);
	}
	return;
}
int main() { solve(); return 0; }//boj1725 histogram