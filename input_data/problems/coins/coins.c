#include <stdio.h>
#include <math.h>
#include <assert.h>

#define EPS 1e-8

int main(void)
{
  FILE *in, *out;
  int N;
  int w, d;
  int W;
  int norm;
  int t;

  in = fopen("coins.in", "r");
  out = fopen("coins.out", "w");

  while (fscanf(in, "%d %d %d %d", &N, &w, &d, &W) == 4) {
    norm = w*N*(N-1)/2;
    t = (norm-W)/d;
    if (t == 0) {
      t = N;
    }
    fprintf(out, "%d\n", t);
  }
  return 0;
}
