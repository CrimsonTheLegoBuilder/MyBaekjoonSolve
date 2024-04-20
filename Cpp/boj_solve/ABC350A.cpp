#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
typedef long long ll;
int N, x;
int v[140];
int cnt[101];
char S[10];

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> S;
	int a = (S[3] - 48) * 100 + (S[4] - 48) * 10 + (S[5] - 48);
	if (!a || a == 316 || a > 349) std::cout << "No\n";
	else std::cout << "Yes\n";

	return;
}
int main() { solve(); return 0; }