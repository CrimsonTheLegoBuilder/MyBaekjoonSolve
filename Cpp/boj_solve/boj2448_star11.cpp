#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
#include <cassert>
typedef std::vector<bool> Vbool;
const int LEN = 1 << 12;

int N;
Vbool S[LEN];
Vbool F0 = { 1 };
Vbool F1 = { 1, 0, 1 };
Vbool F2 = { 1, 1, 1, 1, 1 };
void star(bool f, int w, int c, int d) {
	int h = (c / 3) * 3;
	if (f) {
		for (int i = h; i < h * 2; i++) {
			int t = (w * 2 - 1) - 2 * (i - c);
			for (int j = 0; j < t; j++) S[i].push_back(0);
		}
		return;
	}
	if (!d) {
		S[h + 0].insert(S[h + 0].end(), F0.begin(), F0.end());
		S[h + 1].insert(S[h + 1].end(), F1.begin(), F1.end());
		S[h + 2].insert(S[h + 2].end(), F2.begin(), F2.end());
		return;
	}
	star(0, w >> 1, c - d, d >> 1);
	star(0, w >> 1, c + d, d >> 1);
	star(1, w >> 1, c + 0, d >> 1);
	star(0, w >> 1, c + d, d >> 1);
	return;
}
void star(int X) {
	//for (int i = 0; i < X; i++) S[i].clear();
	for (int i = 0; i < X; i++) 
		for (int j = X - i - 1; j > 0; j--) 
			S[i].push_back(0);
	//assert(!(N % 3));
	//if (N > 3) assert(!(N % 2));
	star(0, X, X >> 1, X >> 2);
	for (int i = 0; i < X; i++) {
		int cnt = 0;
		for (const bool& f : S[i]) { std::cout << (f ? "*" : " "); cnt++; }
		//for (int j = 0; j < X - cnt; j++) std::cout << " ";
		std::cout << "\n";
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("../../../input_data/star11out.txt", "w", stdout);
	std::cin >> N;
	star(N);
	//for (int i = 3; i < LEN; i *= 2) star(i);
	return;
}
int main() { solve(); return 0; }//boj2448 star 11
