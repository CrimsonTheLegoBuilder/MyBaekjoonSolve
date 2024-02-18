#include <iostream>
typedef long long ll;
const int MAX = 1'000'001;
ll F[MAX];

ll sum(ll* F, int idx) {
	ll s = 0;
	while (idx > 0) {
		s += F[idx];
		idx -= idx & -idx;
	}
	return s;
}
void update(ll* F, int idx, int N, ll diff) {
	while (idx <= N) {
		F[idx] += diff;
		idx += idx & -idx;
	}
}
//void init(ll* A, ll* F, int N) {
//	for (int i = 1; i <= N; i++) {
//		update(F, i, N, A[i]);
//	}
//}
ll bs(ll* F, ll x, int N) {
	int l = 1, r = N, m;
	while (l < r) {
		m = l + r >> 1;
		if (sum(F, m) >= x) r = m;
		else l = m + 1;
	}
	return r;
}


int main() {
	int n, a;
	ll b, c;
	std::cin >> n;
	while (n--) {
		std::cin >> a;
		if (a == 1) {
			std::cin >> b;
			c = bs(F, b, MAX - 1);
			update(F, c, MAX - 1, -1);
			std::cout << c << "\n";
		}
		else if (a == 2) {
			std::cin >> b >> c;
			update(F, b, MAX - 1, c);
		}
	}
	return 0;
}