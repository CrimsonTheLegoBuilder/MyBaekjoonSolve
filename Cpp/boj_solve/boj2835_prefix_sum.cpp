#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
typedef long long ll;
const int LEN = 87000;

ll S[LEN];
int N, M;

inline int get_sec(int h, int m, int s) { return h * 3600 + m * 60 + s + 1; }
void solve() {
	int _ = scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		int hs, ms, ss, he, me, se;
		_ = scanf("%d:%d:%d - %d:%d:%d", &hs, &ms, &ss, &he, &me, &se);
		int s = get_sec(hs, ms, ss);
		int e = get_sec(he, me, se);
		if (s <= e) {
			S[s]++;
			S[e + 1]--;
		}
		else {
			S[1]++;
			S[e + 1]--;
			S[s]++;
			S[86401]--;
		}
	}
	for (int i = 1; i < LEN; i++) S[i] += S[i - 1];
	for (int i = 1; i < LEN; i++) S[i] += S[i - 1];
	_ = scanf("%d", &M);
	while (M--) {
		int hs, ms, ss, he, me, se;
		_ = scanf("%d:%d:%d - %d:%d:%d", &hs, &ms, &ss, &he, &me, &se);
		int s = get_sec(hs, ms, ss);
		int e = get_sec(he, me, se);
		ll num = 0, den = 1;
		if (s <= e) {
			den = e - s + 1;
			num = S[e] - S[s - 1];
		}
		else {
			den = 86400 - abs(e - s) + 1;
			num = S[e] - S[0] + S[86400] - S[s - 1];
		}
		_ = printf("%.9f\n", 1. * num / den);
	}
}
int main() { solve(); return 0; }