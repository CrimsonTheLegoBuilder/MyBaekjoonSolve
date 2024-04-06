#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <random>
#include <array>
#include <tuple>
typedef long long ll;
typedef double ld;
int N;

void solve() {
	std::cin >> N;
	for (int i = 1; i <= N; i++) std::cout << (i % 3 ? 'o' : 'x');
	return;
}
int main() { solve(); return 0; }