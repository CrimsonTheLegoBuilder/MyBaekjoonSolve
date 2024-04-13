#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
typedef long long ll;
int N, x;
int v[140];
int cnt[101];
char a[105];

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> a;
	int i = 0;
	while (a[i] != '\0') {
		//std::cout << (int)a[i] << "\n";
		v[(int)a[i]]++;
		i++;
	}
	for (i = 97; i < 140; i++) {
		if (v[i]) cnt[v[i]]++;
	}
	for (i = 0; i < 101; i++) {
		//std::cout << cnt[i] << "\n";
		if (!(cnt[i] == 0 || cnt[i] == 2)) { std::cout << "No\n"; return; }
	}
	std::cout << "Yes\n";
	return;
}
int main() { solve(); return 0; }