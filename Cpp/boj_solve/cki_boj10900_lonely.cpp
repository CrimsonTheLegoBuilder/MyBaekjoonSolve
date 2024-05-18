#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <assert.h>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>

using namespace std;
typedef long long ll;
typedef pair<int, int> Pi;
#define Fi first
#define Se second
#define pb(x) push_back(x)
#define sz(x) (int)x.size()
#define rep(i,n) for(int i=0;i<n;i++)
#define all(x) x.begin(),x.end()

typedef pair<double, double> Pd;
double x[100010];
int xz;
int N;
const double EPS = 1e-10;

struct circle {
	int x, y, r;
	bool operator==(const circle& l)const {
		return x == l.x && y == l.y && r == l.r;
	}
	bool operator<(const circle& l)const {
		return r < l.r;
	}
}C[305];

struct point {
	point() {}
	point(double x, double y) :x(x), y(y) {}
	double x, y;
};

struct lx {
	lx() {}
	lx(double x, int p) :p(p), x(x) {}
	int p;
	double x;
	bool operator<(const lx& l)const {
		return fabs(x - l.x) < EPS ? p < l.p : x < l.x;
	}
}v[660];

int ans;
int chk[330];
int Left[330], Right[330];
int tv;

int main() {

	scanf("%d", &N);
	rep(i, N) {
		scanf("%d%d%d", &C[i].x, &C[i].y, &C[i].r);
		for (int j = 0; j < i; j++) {
			if (C[i] == C[j]) {
				i--; N--; ans++;
				break;
			}
		}
	}

	/*
	 N = 300;
	 rep(i, N){
	 C[i].r = 1000 - i / 30;
	 C[i].y = i % 30;
	 C[i].x = i / 40;

	 }
	*/
	sort(C, C + N);
	reverse(C, C + N);

	rep(P, 1) {
		//rep(i, N)swap(C[i].x, C[i].y); memset(chk, 0, sizeof chk); xz = 0; ans = 0;
		// if(P == 0)rep(i, N)printf("%d %d %d\n", C[i].x, C[i].y, C[i].r);

		rep(i, N)Left[i] = C[i].x - C[i].r, Right[i] = C[i].x + C[i].r;
		rep(i, N)rep(j, i) {
			// http://math.stackexchange.com/questions/256100/how-can-i-find-the-points-at-which-two-circles-intersect
			int x1 = C[i].x, x2 = C[j].x, y1 = C[i].y, y2 = C[j].y;
			int r1 = C[i].r, r2 = C[j].r;
			int lensq = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
			if (lensq < (r1 - r2) * (r1 - r2))continue;
			if (lensq > (r1 + r2) * (r1 + r2))continue;
			double d = sqrt(lensq);
			double l = (r1 * r1 - r2 * r2 + lensq) / (2 * d);
			double h = sqrt(r1 * r1 - l * l);
			double g1 = l / d * (x2 - x1) + h / d * (y2 - y1) + x1;
			double g2 = l / d * (x2 - x1) - h / d * (y2 - y1) + x1;
			if (lensq == (r1 - r2) * (r1 - r2) || lensq == (r1 + r2) * (r1 + r2) || y1 == y2)x[xz++] = g1;
			else x[xz++] = g1, x[xz++] = g2;
		}
		rep(i, N) {
			x[xz++] = Left[i];
			x[xz++] = Right[i];
		}
		sort(x, x + xz);
		double prev = x[0];
		rep(i, xz) {
			if (i != 0 && x[i] - x[i - 1] > EPS) {
				double p = (x[i] + prev) / 2;
				prev = x[i];
				//printf("X%f\n", x[i]);
				tv = 0;
				rep(j, N) {
					if (Left[j] + EPS < p && p < Right[j] - EPS) {
						double y1 = sqrt(C[j].r * C[j].r - (C[j].x - p) * (C[j].x - p));
						double y2 = -y1;
						y1 += C[j].y;
						y2 += C[j].y;
						v[tv++] = lx(y2, j);
						v[tv++] = lx(y1, -j - 1);
					}
				}
				sort(v, v + tv);
				//rep(i, tv)printf("%d %f\n", v[i].p, v[i].x); puts("");
				int now[2] = {};
				rep(j, tv) {
					if (v[j].p < 0)now[0]--, now[1] += v[j].p;
					else now[0]++, now[1] += v[j].p + 1;
					if (now[0] == 1)chk[now[1] - 1] = 1;
				}
				//rep(j, N)printf("%d ", chk[j]); puts("");
			}
		}
		rep(i, N)if (!chk[i])ans++;
		printf("%d\n", ans);
	}
	return 0;
}
