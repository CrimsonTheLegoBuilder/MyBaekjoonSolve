#include <stdio.h>
#include <math.h>

int main(void)
{
  double balance, rate, target;
  FILE *in, *out;

  in = fopen("account.in", "r");
  out = fopen("account.out", "w");

  while (fscanf(in, "%lf %lf %lf", &balance, &rate, &target) == 3) {
    fprintf(out, "%d\n", (int)ceil(log(target / balance)/log(1+rate/100)));
  }

  return 0;
}
