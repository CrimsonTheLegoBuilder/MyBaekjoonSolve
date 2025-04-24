#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
const int LEN = 500;
int N, DP[LEN][LEN], CNT[LEN], cnt{ 0 };

struct Pos {
	ll x, y;
	bool operator < (const Pos& p) { return x == p.x ? y < p.y : x < p.x; }
} T[LEN];
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> T[i].x >> T[i].y;
	std::sort(T, T + N);
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			for (int k = i + 1; k < j; k++) {
				if (cross(T[i], T[k], T[j]) > 0) DP[i][j]++;
			}
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			for (int k = i + 1; k < j; k++) {
				if (cross(T[i], T[k], T[j]) > 0) {
					CNT[DP[i][j] - DP[i][k] - DP[k][j] - 1]++;
				}
				else if (cross(T[i], T[k], T[j]) < 0) {
					CNT[DP[i][k] + DP[k][j] - DP[i][j]]++;
				}
			}
		}
	}
	for (int i = 1; i < N - 2; i++) cnt += CNT[i];
	std::cout << cnt << "\n";
	return 0;
}