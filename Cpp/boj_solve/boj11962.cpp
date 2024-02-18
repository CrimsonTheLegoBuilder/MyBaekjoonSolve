#include <iostream>
#include <algorithm>
typedef long long ll;
const int LEN = 200'001;
int N, Q, A, B, C;
const ll INF = 1e17;

ll sumt[LEN << 2], mint[LEN << 2], lazy[LEN << 2];

void propagate(int s, int e, int i) {
	if (lazy[i]) {
		mint[i] += lazy[i];
		sumt[i] += lazy[i] * (e - s + 1);
		if (s != e) {
			lazy[i << 1] += lazy[i];
			lazy[i << 1 | 1] += lazy[i];
		}
		lazy[i] = 0;
		return;
	}
}
ll get_min(int l, int r, int s = 1, int e = N, int i = 1) {
	propagate(s, e, i);
	if (r < s || e < l) return INF;
	if (l <= s && e <= r) return mint[i];
	int m = s + e >> 1;
	return std::min(get_min(l, r, s, m, i << 1), get_min(l, r, m + 1, e, i << 1 | 1));
}
ll get_sum(int l, int r, int s = 1, int e = N, int i = 1) {
	propagate(s, e, i);
	if (r < s || e < l) return 0;
	if (l <= s && e <= r) return sumt[i];
	int m = s + e >> 1;
	return (get_sum(l, r, s, m, i << 1) + get_sum(l, r, m + 1, e, i << 1 | 1));
}
void update(int l, int r, ll c, int s = 1, int e = N, int i = 1) {
	propagate(s, e, i);
	if (r < s || e < l) return;
	if (l <= s && e <= r) {
		mint[i] += c;
		sumt[i] += c * (e - s + 1);
		if (s ^ e) {
			lazy[i << 1] += c;
			lazy[i << 1 | 1] += c;
		}
		return;
	}
	int m = s + e >> 1;
	update(l, r, c, s, m, i << 1); update(l, r, c, m + 1, e, i << 1 | 1);
	mint[i] = std::min(mint[i << 1], mint[i << 1 | 1]);
	sumt[i] = sumt[i << 1] + sumt[i << 1 | 1];
}

char q;

int main() {
	std::cin >> N >> Q;
	for (int i = 1; i <= N; i++) {
		std::cin >> C;
		update(i, i, C);
	}
	for (int i = 0; i < Q; i++) {
		std::cin >> q;
		if (q == 'M') {
			std::cin >> A >> B;
			std::cout << get_min(A, B) << "\n";
		}
		else if (q == 'S') {
			std::cin >> A >> B;
			std::cout << get_sum(A, B) << "\n";
		}
		else if (q == 'P') {
			std::cin >> A >> B >> C;
			update(A, B, C);
		}
	}
	return 0;
}
