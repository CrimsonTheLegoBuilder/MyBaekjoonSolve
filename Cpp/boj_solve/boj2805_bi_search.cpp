#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
const int LEN = 1'000'000;
int N, M;
ll T[LEN];

ll sum(ll m) {
	ll total = 0;
	for (int i = 0; i < N; i++) {
		total += (T[i] > m ? T[i] - m : 0);
	}
	return total;
}
ll bi_search(int N, int M) {
	//std::sort(T, T + N);
	ll e = 0;
	for (int i = 0; i < N; i++) {
		if (e < T[i]) e = T[i];
		//e = std::max(e, T[i]); 
	}
	ll s = 0, m;
	//std::cout << s << " " << e << "\n";
	while (s < e) {
		m = s + e >> 1;
		//std::cout << s << " " << e << " " << m << " " << sum(m) << "\n";
		ll t = sum(m);
		if (t == M) return m;
		else if (t > M) s = m + 1;
		else e = m - 1;
	}
	//std::cout << s << " " << e << "\n";
	if (sum(s) < M) s -= 1;
	//std::cout << s << "\n";
	return s;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	for (int i = 0; i < N; i++) { std::cin >> T[i]; }
	std::cout << bi_search(N, M) << "\n";
	return 0;
}


/*
3 30
0 0 31
*/