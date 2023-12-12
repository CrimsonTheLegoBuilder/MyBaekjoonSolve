#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *in, *out, *tmp, *vfy;
int case_num = 1;

#define MAX_TOOTHPICKS 5000
int best_prod[MAX_TOOTHPICKS+1];
int best_sum[MAX_TOOTHPICKS+1];

int eval(char *expr)
{
  int ans;
  char *p;

  if (p = strchr(expr, '+')) {
    *p = 0;
    if ((ans = eval(expr) + eval(p+1)) > MAX_TOOTHPICKS) {
      ans = MAX_TOOTHPICKS+1;
    }
    return ans;
  }

  if (p = strchr(expr, 'x')) {
    *p = 0;
    if ((ans = eval(expr) * eval(p+1)) > MAX_TOOTHPICKS) {
      ans = MAX_TOOTHPICKS+1;
    }
    return ans;
  }

  if (!*expr) {
    fprintf(vfy, "Case %d: bad expression\n", case_num);
    fprintf(vfy, "WRONG OUTPUT\n");
    exit(0);
  }
  for (p = expr; *p; *p++) {
    if (*p != '|') {
      fprintf(vfy, "Case %d: bad expression\n", case_num);
      fprintf(vfy, "WRONG OUTPUT\n");
      exit(0);
    }
  }
  return p-expr;
}

int main(void)
{
  char line[2000], expr[2010];
  int i, j, n, ans, n2, total;

  for (i = 1; i <= MAX_TOOTHPICKS; i++) {
    best_prod[i] = i;
    for (j = 1; j < i; j++) {
      if (i % j == 0 && best_prod[j] + best_prod[i/j] + 2 < best_prod[i]) {
	best_prod[i] = best_prod[j] + best_prod[i/j] + 2;
      }
    }
  }

  for (i = 1; i <= MAX_TOOTHPICKS; i++) {
    best_sum[i] = best_prod[i];
    for (j = 1; j < i; j++) {
      if (best_sum[j] + best_sum[i-j] + 2 < best_sum[i]) {
	best_sum[i] = best_sum[j] + best_sum[i-j] + 2;
      }
    }
  }


  in = fopen("toothpicks.in", "r");
  out = fopen("toothpicks.out", "r");
  tmp = fopen("toothpicks.tmp", "w");
  vfy = fopen("toothpicks.vfy", "w");


  while (fscanf(in, "%d", &n) == 1) {
    fprintf(tmp, "%d toothpicks: ", best_sum[n]);
    if (!fgets(line, 2000, out) || line[strlen(line)-1] != '\n') {
      fprintf(vfy, "Case %d: wrong output line\n", case_num);
      fprintf(vfy, "WRONG OUTPUT\n");
      exit(0);
    }
    if (sscanf(line, "%d toothpicks: %[|+x]=%d\n", &ans, expr, &n2) != 3) {
      fprintf(vfy, "Case %d: wrong output line\n", case_num);
      fprintf(vfy, "WRONG OUTPUT\n");
      exit(0);
    }
    if (n != n2 || ans != best_sum[n]) {
      fprintf(vfy, "Case %d: wrong numeric output\n", case_num);
      fprintf(vfy, "WRONG OUTPUT\n");
      exit(0);
    }
    fprintf(tmp, "%s=%d\n", expr, n);

    total = 0;
    for (i = 0; expr[i]; i++) {
      total += (expr[i] == '|') ? 1 : 2;
    }
    if (total != ans) {
      fprintf(vfy, "Case %d: wrong number of toothpicks in expression.\n",
	      case_num);
      fprintf(vfy, "WRONG OUTPUT\n");
      exit(0);
    }
    if (eval(expr) != n) {
      fprintf(vfy, "Case %d: expression evaluate to wrong answer.\n",
	      case_num);
      fprintf(vfy, "WRONG OUTPUT\n");
      exit(0);
    }

    case_num++;
  }
  fclose(in);
  fclose(out);
  fclose(tmp);

  if (system("diff toothpicks.out toothpicks.tmp > /dev/null")) {
    if (system("diff -biw toothpicks.out toothpicks.tmp > /dev/null")) {
      fprintf(vfy, "Final diff failed.\n");
      fprintf(vfy, "WRONG OUTPUT\n");
    } else {
      fprintf(vfy, "PRESENTATION ERROR\n");
    }
  } else {
    fprintf(vfy, "CORRECT!\n");
  }

  return 0;
}
