#include <stdio.h>
#include <assert.h>

FILE *in, *out;

#define MAX_TOOTHPICKS 5000

int best_prod[MAX_TOOTHPICKS+1];
int best_prod_part[MAX_TOOTHPICKS+1];

int best_sum[MAX_TOOTHPICKS+1];
int best_sum_part[MAX_TOOTHPICKS+1];

void print_toothpicks(int n)
{
  int i;
  for (i = 0; i < n; i++) {
    fprintf(out, "|");
  }
}

void print_prod_expr(int n)
{
  if (best_prod_part[n] == n) {
    print_toothpicks(n);
  } else {
    print_prod_expr(best_prod_part[n]);
    fprintf(out, "x");
    print_prod_expr(n/best_prod_part[n]);
  }
}

void print_sum_expr(int n)
{
  if (best_sum_part[n] == -1) {
    print_prod_expr(n);
  } else {
    print_sum_expr(best_sum_part[n]);
    fprintf(out, "+");
    print_sum_expr(n-best_sum_part[n]);
  }
}


int main(void)
{
  int i, j, n;

  for (i = 1; i <= MAX_TOOTHPICKS; i++) {
    best_prod[i] = i;
    best_prod_part[i] = i;
    for (j = 1; j < i; j++) {
      if (i % j == 0 && best_prod[j] + best_prod[i/j] + 2 < best_prod[i]) {
	best_prod[i] = best_prod[j] + best_prod[i/j] + 2;
	best_prod_part[i] = j;
      }
    }
  }

  for (i = 1; i <= MAX_TOOTHPICKS; i++) {
    best_sum[i] = best_prod[i];
    best_sum_part[i] = -1;
    for (j = 1; j < i; j++) {
      if (best_sum[j] + best_sum[i-j] + 2 < best_sum[i]) {
	best_sum[i] = best_sum[j] + best_sum[i-j] + 2;
	best_sum_part[i] = j;
      }
    }
  }

  in = fopen("toothpicks.in", "r");
  out = fopen("toothpicks.out", "w");
  assert(in && out);

  while (fscanf(in, "%d", &n) == 1) {
    fprintf(out, "%d toothpicks: ", best_sum[n]);
    print_sum_expr(n);
    fprintf(out, "=%d\n", n);
  }

  return 0;
}
