#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
const int LEN = 300;
int N, DP[LEN][LEN], DP2[LEN][LEN], cnt = 0, a, b, c;

struct Pos {
	ll x, y;
	int i;
	bool operator < (const Pos& p) { return x == p.x ? y < p.y : x < p.x; }

} T[LEN];
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (!cross(d1, d3, d2) && (dot(d1, d3, d2) >= 0));
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> T[i].x >> T[i].y, T[i].i = i + 1;
	std::sort(T, T + N);
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			for (int k = i + 1; k < j; k++) {
				if (cross(T[i], T[k], T[j]) > 0) DP[i][j]++;
				if (cross(T[i], T[k], T[j]) > 0 || on_seg(T[i], T[j], T[k])) DP2[i][j]++;
			}
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			for (int k = i + 1; k < j; k++) {
				if (cross(T[i], T[k], T[j]) >= 0) {
					int total = DP2[i][j] - DP[i][k] - DP[k][j] + 2;
					if (cnt < total) {
						cnt = total, a = T[i].i, b = T[j].i, c = T[k].i;
					}
				}
				else if (cross(T[i], T[k], T[j]) < 0) {
					int total = DP2[i][k] + DP2[k][j] - DP[i][j] + 3;
					if (cnt < total) {
						cnt = total, a = T[i].i, b = T[j].i, c = T[k].i;
					}
				}
			}
		}
	}
	std::cout << cnt << "\n" << a << " " << b << " " << c << "\n";
	return;
}
int main() { solve(); return 0; }//boj3179


//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//const int LEN = 300;
//int N, DP[LEN][LEN], DP2[LEN][LEN], cnt = 0, a, b, c;
//
//struct Pos {
//	ll x, y;
//	int i;
//	bool operator < (const Pos& p) { return x == p.x ? y < p.y : x < p.x; }
//
//} T[LEN];
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
//}
//bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (!cross(d1, d3, d2) && (dot(d1, d3, d2) >= 0));
//}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	for (int i = 0; i < N; i++) std::cin >> T[i].x >> T[i].y, T[i].i = i + 1;
//	std::sort(T, T + N);
//	for (int i = 0; i < N; i++) {
//		for (int j = i + 1; j < N; j++) {
//			for (int k = i + 1; k < j; k++) {
//				if (cross(T[i], T[k], T[j]) > 0) DP[i][j]++;
//				if (cross(T[i], T[k], T[j]) > 0 || on_seg(T[i], T[j], T[k])) DP2[i][j]++;
//			}
//		}
//	}
//	for (int i = 0; i < N; i++) {
//		for (int j = i + 1; j < N; j++) {
//			for (int k = i + 1; k < j; k++) {
//				if (cross(T[i], T[k], T[j]) >= 0) {
//					int total = DP2[i][j] - DP[i][k] - DP[k][j] + 2;
//					if (cnt < total) {
//						cnt = total, a = T[i].i, b = T[j].i, c = T[k].i;
//						//cnt = total, a = i, b = j, c = k;
//					}
//				}
//				else if (cross(T[i], T[k], T[j]) < 0) {
//					int total = DP2[i][k] + DP2[k][j] - DP[i][j] + 3;
//					if (cnt < total) {
//						cnt = total, a = T[i].i, b = T[j].i, c = T[k].i;
//						//cnt = total, a = i, b = j, c = k;
//					}
//				}
//			}
//		}
//	}
//	std::cout << cnt << "\n" << a << " " << b << " " << c << "\n";
//	//Pos x = T[a], y = T[b], z = T[c];
//	//if (cross(x, y, z) < 0) std::swap(y, z);
//	//for (int i = 0; i < N; i++) {
//	//	if ((cross(x, y, T[i]) > 0 && cross(y, z, T[i]) > 0 && cross(z, x, T[i]) > 0) || on_seg(x, y, T[i]) || on_seg(y, z, T[i]) || on_seg(z, x, T[i])) std::cout << i << "\n";
//	//}
//	return;
//}
//
//int main() { solve(); return 0; }//boj3179