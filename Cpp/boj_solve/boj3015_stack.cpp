#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
typedef long long ll;
const int LEN = 5e5 + 2;
int N;
ll cur, H[LEN];
std::vector<int> stack;

struct Num {
	ll h;
	ll n;
	Num(ll X = 0, ll Y = 0) : h(X), n(Y) {}
};
std::vector<Num> S;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	ll cnt = 0;
	ll tmp;
	while (N--) {
		std::cin >> cur;
		tmp = 1;
		while (S.size() && S.back().h < cur) {
			cnt += S.back().n;
			S.pop_back();
		}
		if (S.size()) {
			if (S.back().h == cur) {
				cnt += S.back().n;
				tmp = S.back().n + 1;
				if (S.size() > 1) cnt++;
				S.pop_back();
			}
			else cnt++;
		}
		S.push_back(Num(cur, tmp));
	}
	std::cout << cnt << "\n";
	return;
}
int main() { solve(); return 0; }//boj3015 PATRIK