#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
//#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
const int LEN = 3'001;
int N, P[LEN];

//struct Pos { ll x, y; };
struct Pos { int x, y; };
struct Seg { Pos l, r; } S[LEN];

int C(const Pos& d1, const Pos& d2, const Pos& d3) {
	//ll ccw = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	int ccw = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return ccw > 0 ? 1 : ccw < 0 ? -1 : 0;
}
int D(const Pos& d1, const Pos& d2, const Pos& d3) {
	//ll dot = (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
	int dot = (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
	return dot > 0 ? 1 : dot < 0 ? -1 : 0;
}
bool X(int a, int b) {
	Pos d1 = S[a].l, d2 = S[a].r, d3 = S[b].l, d4 = S[b].r;
	bool F1 = C(d1, d2, d3) * C(d2, d1, d4) > 0 && C(d3, d4, d1) * C(d4, d3, d2) > 0;
	bool F2 = !C(d1, d2, d3) && D(d1, d3, d2) >= 0 ||
		!C(d1, d2, d4) && D(d1, d4, d2) >= 0 ||
		!C(d3, d4, d1) && D(d3, d1, d4) >= 0 ||
		!C(d3, d4, d2) && D(d3, d2, d4) >= 0;
	return F1 || F2;
}
void init() {
	memset(P, -1, sizeof P);
	//std::cin.tie(0)->sync_with_stdio(0);
	//std::cout.tie(0);
	scanf("%d", &N);
	//std::cin >> N;
	for (int i = 1; i <= N; i++) {
		//std::cin >> S[i].l.x >> S[i].l.y >> S[i].r.x >> S[i].r.y;
		scanf("%d%d%d%d", &S[i].l.x, &S[i].l.y, &S[i].r.x, &S[i].r.y);
	}
}

int find(int v) {
	if (P[v] < 0) return v;
	int p = find(P[v]);
	return P[v] = p;
}
bool join(int u, int v) {
	int i = find(u);
	int j = find(v);
	if (i == j) return 0;
	if (P[i] > P[j]) {
		P[j] += P[i];
		P[i] = j;
	}
	else {
		P[i] += P[j];
		P[j] = i;
	}
	return 1;
}

int main() {
	init();
	for (int i = 1; i <= N; i++) {
		for (int j = i + 1; j <= N; j++) {
			if (X(i, j)) join(i, j);
		}
	}
	//for (int i = 1; i <= N; i++) find(i);
	int cnt{ 0 }, MAX{ 0 };
	for (int i = 1; i <= N; i++) {
		if (P[i] < 0) {
			cnt++, MAX = std::min(MAX, P[i]);
		}
	}
	//std::cout << cnt << "\n" << -MAX << "\n";
	printf("%d\n%d\n", cnt, -MAX);
	return 0;
}