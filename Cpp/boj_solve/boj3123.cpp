#include <stdio.h>
bool L[1001][1001];
double A;
int main() {
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