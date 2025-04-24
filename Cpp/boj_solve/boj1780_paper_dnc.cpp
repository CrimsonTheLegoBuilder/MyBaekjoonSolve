#include <iostream>
typedef long long ll;
const int LEN = 2187;  // 3*3*3*3*3*3*3;

struct Paper {
	int p, z, m;
};

int p[LEN][LEN];

Paper P(int ru, int rd, int cl, int cr) {
	if (ru == rd) {
		if (p[ru][cl] == 1) return { 1, 0, 0 };
		else if (p[ru][cr] == 0) return { 0, 1 ,0 };
		else if (p[ru][cr] == -1) return { 0, 0, 1 };
	}
	int s = p[ru][cl];
	bool f = 1;
	for (int i = ru; i <= rd; i++) {
		for (int j = cl; j <= cr; j++) {
			if (s != p[i][j]) f = 0;
		}
	}
	if (f) {
		if (s == 1) return { 1, 0, 0 };
		else if (s == 0) return { 0, 1, 0 };
		else if (s == -1) return { 0, 0, 1 };
	}
	int r1 = (ru * 2 + rd) / 3;
	int r2 = (ru + 2 * rd) / 3;
	int c1 = (cl * 2 + cr) / 3;
	int c2 = (cl + 2 * cr) / 3;
	Paper p1 = P(ru, r1, cl, c1);
	Paper p2 = P(ru, r1, c1 + 1, c2);
	Paper p3 = P(ru, r1, c2 + 1, cr);
	Paper p4 = P(r1 + 1, r2, cl, c1);
	Paper p5 = P(r1 + 1, r2, c1 + 1, c2);
	Paper p6 = P(r1 + 1, r2, c2 + 1, cr);
	Paper p7 = P(r2 + 1, rd, cl, c1);
	Paper p8 = P(r2 + 1, rd, c1 + 1, c2);
	Paper p9 = P(r2 + 1, rd, c2 + 1, cr);
	Paper pp = {
		p1.p + p2.p + p3.p + p4.p + p5.p + p6.p + p7.p + p8.p + p9.p,
		p1.z + p2.z + p3.z + p4.z + p5.z + p6.z + p7.z + p8.z + p9.z,
		p1.m + p2.m + p3.m + p4.m + p5.m + p6.m + p7.m + p8.m + p9.m,
	};
	if (!pp.m && !pp.z) return { 1, 0, 0 };
	if (!pp.p && !pp.m) return { 0, 1, 0 };
	if (!pp.p && !pp.z) return { 0, 0, 1 };
	else return pp;
	
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int n; std::cin >> n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cin >> p[i][j];
		}
	}
	Paper total = P(0, n - 1, 0, n - 1);
	std::cout << total.m << "\n" << total.z << "\n" << total.p << "\n";
	return 0;
}