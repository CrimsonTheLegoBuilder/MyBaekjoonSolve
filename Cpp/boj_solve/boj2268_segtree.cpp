#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
const int LEN = 1'000'001;
int N, M, A[LEN];
bool a;
ll b, c, diff, SEG[LEN << 2];

ll Sum(int l, int r, int s = 1, int e = N, int i = 1) {
	if (r < s || e < l) return 0;
	if (l <= s && e <= r) return SEG[i];
	int m = s + e >> 1;
	return Sum(l, r, s, m, i << 1) + Sum(l, r, m + 1, e, i << 1 | 1);
}
void Modify(int x, ll diff, int s = 1, int e = N, int i = 1) {
	if (e < x || x < s) return;
	SEG[i] += diff;
	if (s != e) {
		int m = s + e >> 1;
		Modify(x, diff, s, m, i << 1);
		Modify(x, diff, m + 1, e, i << 1 | 1);
	}
}
void Update(int b, ll c) {
	diff = c - A[b];
	Modify(b, diff);
	A[b] = c;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	while (M--) {
		std::cin >> a >> b >> c;
		if (!a) {
			if (b > c) std::swap(b, c);
			std::cout << Sum(b, c) << "\n";
		}
		else Update(b, c);
	}
	return 0;
}