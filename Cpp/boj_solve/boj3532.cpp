#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const int LEN = 40'005;

ll N, w, h;
ll X[LEN], Y[LEN];
int x, y;
ll H[LEN], V[LEN];
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> w >> h >> N;
	x = y = 0;
	X[x++] = Y[y++] = 0;
	X[x++] = w + 1; Y[y++] = h + 1;
	for (int i = 0; i < N; i++) std::cin >> X[x++] >> Y[y++];
	std::sort(X, X + x);
	std::sort(Y, Y + y);
	for (int i = 0; i < x - 1; i++) {
		H[i] = X[i + 1] - X[i];
		V[i] = Y[i + 1] - Y[i];
	}
	std::sort(H, H + x - 1);
	std::sort(V, V + y - 1);
	std::cout << (H[x - 2] - 1) * (V[y - 2] - 1) << "\n";
	return 0;
}