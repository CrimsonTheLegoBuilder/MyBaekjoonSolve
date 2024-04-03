#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
typedef long long ll;
const int LEN = 1e5 + 1;
int N;
ll H[LEN];
std::vector<int> stack;

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) std::cin >> H[i];
	ll ans = 0;
	stack = { 0 };
	for (int i = 1; i <= N; i++) {
		while (stack.size() && H[i] < H[stack.back()]) {
			ll h = stack.back();
			stack.pop_back();
			ans = std::max(ans, (i - 1 - stack.back()) * H[h]);
		}
		stack.push_back(i);
	}
	for (int i = 1; i < stack.size(); i++) {
		ans = std::max(ans, H[stack[i]] * (N - stack[i - 1]));
	}
	std::cout << ans << "\n";
}
int main() { solve(); return 0; }//boj1725 histogram
