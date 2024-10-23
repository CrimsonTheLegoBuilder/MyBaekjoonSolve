#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef long double ld;
const int LEN = 1e6 + 1;

int H1, M1, S1, H2, M2, S2, H3, M3, S3;
ll T1, T2, T3;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	scanf("%d:%d:%d", &H1, &M1, &S1);
	scanf("%d:%d:%d", &H2, &M2, &S2);
	T1 = H1 * 60ll * 60 + M1 * 60ll + S1;
	T2 = H2 * 60ll * 60 + M2 * 60ll + S2;
	T3 = T2 - T1;
	if (T3 < 0) T3 += 24 * 60 * 60;
	H3 = T3 / (60 * 60);
	T3 %= (60 * 60);
	M3 = T3 / 60;
	T3 %= 60;
	S3 = T3;
	printf("%02d:%02d:%02d", H3, M3, S3);
	return 0;
}