#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef std::vector<int> vint;
const int LEN = 1 << 9;

//#define DEBUG

int T, N, Q;
int tree[LEN];
void query() {
	memset(tree, 0, sizeof tree);
	std::cin >> N;
	int c = 1 << (N - 1);
	int n = 1;
	for (int i = c << 1; i < c << 2; i++) {
		tree[i] = n;
		n++;
	}
	for (int i = N - 1; i >= 0; i--) {
		int w;
		for (int j = 0; j < c; j++) {
			std::cin >> w;
			tree[(1 << i) + j] = w;
		}
		c >>= 1;
	}

#ifdef DEBUG
	std::cout << "\nDEBUG:: fuck::\n";
	for (int i = 1; i < (1 << (N + 1)); i++) std::cout << tree[i] << " ";
	std::cout << "\nDEBUG:: suck::\n";
#endif

	c = 1 << N;
	std::cin >> Q;
	while (Q--) {
		int q, a1 = 1, a2 = 0;
		std::cin >> q;
		int p = c + q - 1;
		while (tree[p] == q) {
			a2++;
			p >>= 1;
		}
		a2 = c - (1 << (a2 - 1)) + 1;
		int pre = q;
		while (tree[p]) {
			if (pre != tree[p]) {
				a1++;
				pre = tree[p];
			}
			p >>= 1;
		}
		std::cout << "Player " << q;
		std::cout << " can be ranked as high as " << a1;
		std::cout << " or as low as " << a2 << ".\n";
	}
	std::cout << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) query();
	return;
}
int main() { solve(); return 0; }//boj3065
