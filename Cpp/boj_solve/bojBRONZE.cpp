#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
ll N, A[1005], B[1005], C, D, T, R, G, P, U, V, W, K, X, L, H;
ld F;
std::string S;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N >> S;
	if (S == "miss") N *= 0;
	if (S == "bad") N *= 200;
	if (S == "cool") N *= 400;
	if (S == "great") N *= 600;
	if (S == "perfect") N *= 1000;
	//std::cout << N << "\n";
	return 0;
}