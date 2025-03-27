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

#include<cstdio>
int X, Y, N, x, y;
bool L[1001][1001], d, r, l;
double A, P = .785398, b = .171213, c = .043388;
void a(int w, int h) {
	d = L[y + h][x + w];
	r = L[y + h][x];
	l = L[y][x + w];
	if (r & l)return;
	if (!d & !r & !l)A += P;
	if (d & !r & !l)A += 1 - P;
	if (!d & (r | l))A += b;
	if (d & (r | l))A += c;
}
main() {
	scanf("%d%d%d", &X, &Y, &N);
	while (N--) {
		scanf("%d%d", &x, &y);
		L[y][x] = 1;
		if (x < X & y < Y)a(1, 1);
		if (x < X & 0 < y)a(1, -1);
		if (0 < x & y < Y)a(-1, 1);
		if (0 < x & 0 < y)a(-1, -1);
	}
	printf("%.4f", A);
}