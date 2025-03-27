#include <stdio.h>
bool L[1001][1001];
double A;
main() {
	int X, Y, N, x, y, s, i, j;
	scanf("%d%d%d", &X, &Y, &N);
	for (i = 0; i < N; i++) {
		scanf("%d%d", &x, &y);
		L[y][x] = 1;
	}
	for (j = 0; j < Y; j++) {
		for (i = 0; i < X; i++) {
			s = L[j][i] + L[j + 1][i] + L[j][i + 1] + L[j + 1][i + 1];
			if (s >= 3) A++;
			else if (s == 2) {
				if (L[j][i] & L[j + 1][i + 1] | L[j + 1][i] & L[j][i + 1]) A++;
				else A += 0.956611;//pi/6 + 3**.5/4
			}
			else if (s) A += 0.785398;//pi/4
		}
	}
	printf("%.4lf", A);
}

#include <stdio.h>
bool L[1001][1001];
double A, PI = 3.141592653, a1 = 0.04338852250, a2 = 0.17121331409;
void add(const int& x, const int& y, const int& dx, const int& dy) {
	bool d, r, l;
	d = L[y + dy][x + dx];
	r = L[y + dy][x];
	l = L[y][x + dx];
	if (r && l) return;
	else if (!d && !r && !l) A += PI * .25;
	else if (d && (r || l)) A += a1;
	else if (!d && (r || l)) A += a2;
	else if (d && (!r && !l)) A += 1 - (PI * .25);
	return;
}
main() {
	int X, Y, N, x, y, i;
	scanf("%d%d%d", &X, &Y, &N);
	for (i = 0; i < N; i++) {
		scanf("%d%d", &x, &y);
		if (L[y][x]) continue;
		L[y][x] = 1;
		if (x < X && y < Y) add(x, y, 1, 1);
		if (x < X && 0 < y) add(x, y, 1, -1);
		if (0 < x && y < Y) add(x, y, -1, 1);
		if (0 < x && 0 < y) add(x, y, -1, -1);
	}
	printf("%.4lf", A);
}