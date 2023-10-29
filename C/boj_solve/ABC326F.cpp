#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <set>
#include <map>
typedef long long ll;
const int LEN = 80;
int N, a, X, Y;
std::map<int, ll> M[4];
std::vector<int> A[2];


void f(std::map<int, ll>& m, std::vector<int>& a, int s, int i, int e, int bit, int dist) {
	if (i == e) {
		m[dist] = bit;
		return;
	}
	f(m, a, s, i + 1, e, bit, dist - a[i]);
	f(m, a, s, i + 1, e, bit & 1 << i - s, dist + a[i]);
}

int b0, b1, b2, b3;
bool f1, f2;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> X >> Y;
	for (int i = 0; i < N; i++) {
		std::cin >> a;
		A[i & 1].push_back(a);
	}
	for (int i = 0; i < 2; i++) {
		f(M[i * 2], A[i], 0, 0, A[i].size() / 2, 0, 0);
		f(M[i * 2 + 1], A[i], A[i].size() / 2 + 1, A[i].size() / 2 + 1, A[i].size(), 0, 0);
	}
	for (auto& [k, v] : M[0]) {
		std::cout << k << " " << v << " DEBUG\n";
		std::cout << M[1][Y - k] << "\n";
		if (M[1].find(Y - k) != M[1].end()) {
			f1 = 1;
			b0 = v, b2 = M[1][Y - k];
			std::cout << b2 << "\n";
			break;
		}
	}
	for (auto& [k, v] : M[2]) {
		if (M[3].find(X - k) != M[3].end()) {
			f2 = 1;
			b1 = v, b3 = M[3][X - k];
			break;
		}
	}
	if (f1 && f2) {
		std::cout << "Yes\n";

	}
	else std::cout << "No\n";
	return 0;
}