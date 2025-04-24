#include <iostream>
#include <vector>
#include <algorithm>
typedef long long ll;
const int LEN = 1e6 + 1;
const ll MOD = 1e9 + 7;
int N, M, K, Q;
ll a, b, c;
ll arr[LEN];
ll tree[LEN << 2];

ll mul(int s, int e, int l = 1, int r = N, int n = 1) {
	if (e < l || r < s) return 1;
	if (s <= l && r <= e) return tree[n];
	int m = l + r >> 1;
	ll L = mul(s, e, l, m, n << 1) % MOD;
	ll R = mul(s, e, m + 1, r, n << 1 | 1) % MOD;
	return (L * R) % MOD;
}
ll update(int idx, int l = 1, int r = N, int n = 1) {
	if (~idx && (r < idx || idx < l)) return tree[n];
	if (l == r) return tree[n] = arr[l];
	int m = l + r >> 1;
	ll L = update(idx, l, m, n << 1) % MOD;
	ll R = update(idx, m + 1, r, n << 1 | 1) % MOD;
	return tree[n] = (L * R) % MOD;
}
void query() {
	std::cin >> a >> b >> c;
	if (a == 1) { arr[b] = c; update(b); }
	else if (a == 2) { if (b > c) std::swap(b, c); std::cout << mul(b, c) << "\n"; }
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M >> K;
	for (int i = 1; i <= N; i++) std::cin >> arr[i];
	update(-1);
	Q = M + K;
	while (Q--) query();
	return;
}
int main() { solve(); return 0; }//boj11505


//#include <iostream>
//#include <vector>
//#include <algorithm>
//typedef long long ll;
//const int LEN = 1e6 + 1;
//const ll MOD = 1e9 + 7;
//int N, M, K, T;
//ll a, b, c;
//ll arr[LEN];
//ll tree[LEN << 2];
//
//ll mul(int s, int e, int l = 1, int r = N, int n = 1) {
//	if (e < l || r < s) return 1;
//	if (s <= l && r <= e) return tree[n];
//	int m = l + r >> 1;
//	ll L = mul(s, e, l, m, n << 1) % MOD;
//	ll R = mul(s, e, m + 1, r, n << 1 | 1) % MOD;
//	return (L * R) % MOD;
//}
//ll update(int idx, int l = 1, int r = N, int n = 1) {
//	if (~idx && (r < idx || idx < l)) return tree[n];
//	if (l == r) return tree[n] = arr[l];
//	int m = l + r >> 1;
//	ll L = update(idx, l, m, n << 1) % MOD;
//	ll R = update(idx, m + 1, r, n << 1 | 1) % MOD;
//	return tree[n] = (L * R) % MOD;
//}
////ll init(int l = 1, int r = N, int n = 1) {
////	if (l == r) return tree[n] = arr[l];
////	int m = l + r >> 1;
////	ll L = init(l, m, n << 1) % MOD;
////	ll R = init(m + 1, r, n << 1 | 1) % MOD;
////	return tree[n] = (L * R) % MOD;
////}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N >> M >> K;
//	for (int i = 1; i <= N; i++) std::cin >> arr[i];
//	//init();
//	update(-1);
//	T = M + K;
//	while (T--) {
//		std::cin >> a >> b >> c;
//		if (a == 1) { arr[b] = c; update(b); }
//		else if (a == 2) { if (b > c) std::swap(b, c); std::cout << mul(b, c) << "\n"; }
//		//std::cout << "DEBUG:: "; for (int i = 1; i <= N; i++) std::cout << arr[i] << " "; std::cout << "DEBUG::\n";
//	}
//	return;
//}
//int main() { solve(); return 0; }//boj11505