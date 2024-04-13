#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
typedef long long ll;
int N, x;
int v[140];
int cnt[101];
char S[100005], T[4];

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> S >> T;
	int i = 0, t = 0;
	while (S[i] != '\0') {
		if ((int)S[i] == (int)T[t] + 32) t++;
		if (t == 3) { std::cout << "Yes\n"; return; }
		i++;
	}
	if (t < 2 || T[2] != 'X') std::cout << "No\n";
	else std::cout << "Yes\n";
	return;
}
int main() { solve(); return 0; }