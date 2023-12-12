#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

double prob(int N)
{
  /* idea: The probability that the first knot is good is
   *
   *  1 - N/2 / C(N, 2) = (N-2)/(N-1)
   *
   * if the first knot is successful, then it is reduced to the case
   * with N-2 strands.
   *
   * So the probability is:
   *
   * (N-2)(N-4) ... 2 
   * ----------------
   * (N-1)(N-3) ... 3
   *
   */
  
  double p = 1;
  int i;

  for (i = 2; i <= N-2; i+=2) {
    p *= i;
  }
  for (i = 3; i <= N-1; i+=2) {
    p /= i;
  }

  assert((int)floor(p*10000000+0.5) != 50);
  return p;
}

int main(void)
{
  FILE *in, *out;
  int N;
  
  if (!(in = fopen("knots.in", "r"))) {
    fprintf(stderr, "Cannot open input\n");
    exit(1);
  }
  if (!(out = fopen("knots.out", "w"))) {
    fprintf(stderr, "Cannot open output\n");
    exit(1);
  }

  while (fscanf(in, "%d", &N) == 1) {
    fprintf(out, "%.5f\n", prob(N));
  }
  return 0;
}
