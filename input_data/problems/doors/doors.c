#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

typedef struct {
  int x, y;
} Point;

/* Global point for computing convex hull */
Point start_p;

/* counterclockwise, clockwise, or undefined */
enum {CCW, CW, CNEITHER};

int ccw(Point a, Point b, Point c)
{
  int dx1 = b.x - a.x;
  int dx2 = c.x - b.x;
  int dy1 = b.y - a.y;
  int dy2 = c.y - b.y;
  int t1 = dy2 * dx1;
  int t2 = dy1 * dx2;

  if (t1 == t2) {
    if (dx1 * dx2 < 0 || dy1 * dy2 < 0) {
      if (dx1*dx1 + dy1*dy1 >= dx2*dx2 + dy2*dy2) {
        return CNEITHER;
      } else {
        return CW;
      }
    } else {
      return CCW;
    }
  } else if (t1 > t2) {
    return CCW;
  } else {
    return CW;
  }
}

int ccw_cmp(const void *a, const void *b)
{
  Point *p1 = (Point *)a;
  Point *p2 = (Point *)b;
  if (ccw(start_p, *p1, *p2) == CCW) {
    return -1;
  } else if (ccw(start_p, *p2, *p1) == CCW) {
    return 1;
  } else {
    return 0;
  }
}

int convex_hull(Point *polygon, int n, Point *hull) {
  int count, best_i, i;

  if (n == 1) {
    hull[0] = polygon[0];
    return 1;
  }

  /* find the first point: min y, and then min x */
  start_p.x = polygon[0].x;
  start_p.y = polygon[0].y;
  best_i = 0;
  for (i = 1; i < n; i++) {
    if ((polygon[i].y < start_p.y) ||
        (polygon[i].y == start_p.y && polygon[i].x < start_p.x)) {
      start_p = polygon[i];
      best_i = i;
    }
  }
  polygon[best_i] = polygon[0];
  polygon[0] = start_p;

  /* get simple closed polygon */
  qsort((polygon+1), n-1, sizeof(Point), ccw_cmp);

  /* do convex hull */
  count = 0;
  hull[count] = polygon[count]; count++;
  hull[count] = polygon[count]; count++;
  for (i = 2; i < n; i++) {
    while (count > 1 &&
           ccw(hull[count-2], hull[count-1], polygon[i]) == CW) {
      /* pop point */
      count--;
    }
    hull[count++] = polygon[i];
  }
  return count;
}

#define MAX_DP 500

void add_booth(Point *P, int x1, int y1, int x2, int y2, int i)
{
  P[i].x = x1;  P[i++].y = y1;
  P[i].x = x1;  P[i++].y = y2;
  P[i].x = x2;  P[i++].y = y1;
  P[i].x = x2;  P[i++].y = y2;
}

int point_in_poly(Point *poly, int n, Point p)
{
  int i, j, c = 0;

  /* first check to see if point is one of the vertices */
  for (i = 0; i < n; i++) {
    if (p.x == poly[i].x && p.y == poly[i].y) {
      return 1;
    }
  }

  /* now check if it's on the boundary */
  for (i = 0; i < n-1; i++) {
    if (ccw(poly[i], poly[i+1], p) == CNEITHER) {
      return 1;
    }
  }
  if (ccw(poly[n-1], poly[0], p) == CNEITHER) {
    return 1;
  }

  /* finally check if it's inside */
  for (i = 0, j = n-1; i < n; j = i++) {
    if (((poly[i].y <= p.y && p.y < poly[j].y) ||
        (poly[j].y <= p.y && p.y < poly[i].y)) &&
        ( (poly[j].y >= poly[i].y &&
	   (p.x - poly[i].x) * (poly[j].y-poly[i].y) < 
	   (poly[j].x - poly[i].x) * (p.y - poly[i].y)) ||
	  (poly[j].y < poly[i].y &&
	   (p.x - poly[i].x) * (poly[j].y-poly[i].y) >
	   (poly[j].x - poly[i].x) * (p.y - poly[i].y))))
      c = !c;
  }
  return c;
}

int direction(Point p1, Point p2, Point p3)
{
  int x1 = p3.x - p1.x;
  int y1 = p3.y - p1.y;
  int x2 = p2.x - p1.x;
  int y2 = p2.y - p1.y;
  return x1*y2 - x2*y1;
}

int on_segment(Point p1, Point p2, Point p3)
{
  return ((p1.x <= p3.x && p3.x <= p2.x) || (p2.x <= p3.x && p3.x <= p1.x)) &&
    ((p1.y <= p3.y && p3.y <= p2.y) || (p2.y <= p3.y && p3.y <= p1.y));
}

int intersect(Point a1, Point a2, Point b1, Point b2)
{
  int d1 = direction(b1, b2, a1);
  int d2 = direction(b1, b2, a2);
  int d3 = direction(a1, a2, b1);
  int d4 = direction(a1, a2, b2);

  if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
      ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
    return 1;
  } else {
    return (d1 == 0 && on_segment(b1, b2, a1)) ||
      (d2 == 0 && on_segment(b1, b2, a2)) ||
      (d3 == 0 && on_segment(a1, a2, b1)) ||
      (d4 == 0 && on_segment(a1, a2, b2));
  }
}


int intersect_hull(Point *h1, int n1, Point *h2, int n2)
{
  int i, j;

  for (i = 0; i < n1; i++) {
    if (point_in_poly(h2, n2, h1[i])) {
      return 1;
    }
  }

  for (i = 0; i < n2; i++) {
    if (point_in_poly(h1, n1, h2[i])) {
      return 1;
    }
  }

  for (i = 0; i < n1; i++) {
    for (j = 0; j < n2; j++) {
      if (intersect(h1[i], h1[(i+1)%n1], h2[j], h2[(j+1)%n2])) {
	return 1;
      }
    }
  }
  
  return 0;
}

int main(void)
{
  int D, P;
  int x1, y1, x2, y2;
  int i;
  Point doors[MAX_DP*4], penguins[MAX_DP*4];
  Point dhull[MAX_DP*4], phull[MAX_DP*4];
  int dhsize, phsize;
  int case_num = 1;
  FILE *in, *out;

  in = fopen("doors.in", "r");
  out = fopen("doors.out", "w");

  while (fscanf(in, "%d %d", &D, &P) == 2 &&
	 (D || P)) {
    for (i = 0; i < D; i++) {
      fscanf(in, "%d %d %d %d", &x1, &y1, &x2, &y2);
      add_booth(doors, x1, y1, x2, y2, i*4);
    }
    for (i = 0; i < P; i++) {
      fscanf(in, "%d %d %d %d", &x1, &y1, &x2, &y2);
      add_booth(penguins, x1, y1, x2, y2, i*4);
    }
    dhsize = convex_hull(doors, 4*D, dhull);
    phsize = convex_hull(penguins, 4*P, phull);

    /*
    printf("door hull = ");
    for (i = 0; i < dhsize; i++) {
      printf(" (%d,%d)", dhull[i]);
    }
    printf("\n");
    printf("peng hull = ");
    for (i = 0; i < phsize; i++) {
      printf(" (%d,%d)", phull[i]);
    }
    printf("\n");
    */    

    if (case_num > 1) {
      fprintf(out, "\n");
    }
    fprintf(out, 
	    "Case %d: It is %spossible to separate the two groups of vendors.\n",
	   case_num++, 
	   (intersect_hull(dhull, dhsize, phull, phsize)) ? "not " : "");
  }
  return 0;
}
