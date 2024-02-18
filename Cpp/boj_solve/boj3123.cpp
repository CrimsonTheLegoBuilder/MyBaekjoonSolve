#include <stdio.h>
//#include <algorithm>
typedef double ld;
const int LEN = 1'001;
bool loof[LEN][LEN];
ld area;


int main() {
	int X, Y, N, x, y, u, r, l, d;
	scanf("%d%d%d", &X, &Y, &N);
	for (int i = 0; i < N; i++) {
		scanf("%d%d", &x, &y);
		loof[y][x] = 1;
	}
	for (int j = 0; j < Y; j++) {
		for (int i = 0; i < X; i++) {
			u = loof[j][i], l = loof[j + 1][i];
			r = loof[j][i + 1], d = loof[j + 1][i + 1];
			if (u + l + r + d >= 3) area++;
			else if (u + l + r + d == 2) {
				if (u & d | l & r) area++;
				else area += 0.9566114774905181;  //pi/6 + 3**.5/4
			}
			else if (u + l + r + d) area += 0.7853981633974483;  //pi/4
		}
	}
	printf("%.4lf", area);
	return 0;
}