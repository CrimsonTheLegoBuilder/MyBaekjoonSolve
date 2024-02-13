/*
 * CTU Open 2009
 * Sample Solution: Arable Area
 * Josef Cibulka
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MAXN 100
#define MAXCOOR 100
#define D 0

typedef struct Pt
{
	int xp, xq; /* x coordinate = xp/xq */
	int edge;
} Pt;

int n;
int x[MAXN + 2], y[MAXN + 2];
int inc[MAXN + 2]; /* inc[i]=1 if the interior is to the right of edge i->i+1, otherwise -1 */
struct Pt cr[MAXN + 2];
int crcnt;

int por_pt(const void* e1, const void* e2)
{
	struct Pt* p1 = (struct Pt*)e1;
	struct Pt* p2 = (struct Pt*)e2;
	int ret = p1->xp * p2->xq - p2->xp * p1->xq;
	if (ret == 0) return inc[p1->edge] - inc[p2->edge];
	else return ret;
}

/* round the number p/q */
int my_round_down(int p, int q)
{
	assert(q > 0);
	if (p < 0) return (p - q + 1) / q;
	else return p / q;
}

int my_round_up(int p, int q)
{
	assert(q > 0);
	if (p < 0) return p / q;
	else return (p + q - 1) / q;
}

int main(void)
{
	int i, j, ii, tmp;
	int tmpp1, tmpq1, tmpp2, tmpq2;
	long long area;
	int miny, maxy;
	int cw;
	for (ii = 1;; ii++)
	{
		scanf("%d ", &n);
		if (n == 0) break;
		maxy = -10 * MAXCOOR; miny = 10 * MAXCOOR;
		for (i = 0; i < n; i++)
		{
			scanf(" %d %d ", x + i, y + i);
			assert(x[i] >= -MAXCOOR && x[i] <= MAXCOOR && y[i] >= -MAXCOOR && y[i] <= MAXCOOR);
			if (y[i] > maxy) maxy = y[i];
			if (y[i] < miny) miny = y[i];
		}
		x[n] = x[0]; y[n] = y[0];

		/* first determine whether we are going clockwise */
		area = 0;
		for (i = 0; i < n; i++)
		{
			/* count 2 * (the area to the right of edge from i to i+1) */
			area += (2 * MAXCOOR - (x[i] + x[i + 1])) * (y[i + 1] - y[i]);
		}
		if (area > 0) cw = 1; else cw = 0;

		for (i = 0; i < n; i++) if (cw == (y[i + 1] > y[i])) inc[i] = 1; else inc[i] = -1;


		area = 0;
		for (i = miny; i < maxy; i++)
		{
			/* find all crossings of edges with line y=i and line y=i+1 */
			crcnt = 0;
			for (j = 0; j < n; j++) if ((y[j] <= i && y[j + 1] > i) || (y[j + 1] <= i && y[j] > i))
			{
				if (y[j + 1] == y[j]) continue; /* vertical edges have no effect */
				tmpp1 = x[j] * (y[j + 1] - y[j]) + (i - y[j]) * (x[j + 1] - x[j]);
				tmpq1 = y[j + 1] - y[j];
				tmpp2 = x[j] * (y[j + 1] - y[j]) + (i + 1 - y[j]) * (x[j + 1] - x[j]);
				tmpq2 = y[j + 1] - y[j];

				if ((inc[j] == 1 && tmpp1 * tmpq2 >= tmpp2 * tmpq1) || (inc[j] == -1 && (tmpp1 * tmpq2 <= tmpp2 * tmpq1)))
				{
					cr[crcnt].xp = tmpp1;	cr[crcnt].xq = tmpq1;
				}
				else
				{
					cr[crcnt].xp = tmpp2;	cr[crcnt].xq = tmpq2;
				}
				if (cr[crcnt].xq < 0) { cr[crcnt].xq *= -1; cr[crcnt].xp *= -1; }
				cr[crcnt].edge = j;
				crcnt++;
			}
			qsort(cr, crcnt, sizeof(cr[0]), por_pt);
#if(D) 
			for (j = 0; j < crcnt; j++) printf("cr in %d/%d with %d\n", cr[j].xp, cr[j].xq, cr[j].edge);
#endif

			tmp = 0;
			for (j = 0; j < crcnt; j++)
			{
				assert(tmp <= 1);
#if(D>1)
				if (tmp > 0) printf("j==%d %d %d\n", j, my_round_up(cr[j - 1].xp, cr[j - 1].xq), my_round_down(cr[j].xp, cr[j].xq));
#endif
				if (tmp > 0) area += MAX(0, (my_round_down(cr[j].xp, cr[j].xq) - my_round_up(cr[j - 1].xp, cr[j - 1].xq)));
				tmp += inc[cr[j].edge];
			}

#if(D) 
			printf("%d area %lld\n", i, area);
#endif
		}
		printf("%lld\n", area);
	}
	return 0;
}