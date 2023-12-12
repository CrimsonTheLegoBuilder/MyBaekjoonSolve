#include <stdio.h>
#include <assert.h>

#define MAX_N 500

int main(void)
{
  int n, a[MAX_N], perm[MAX_N], i, j, k;
  FILE *in, *out;

  in = fopen("permutation.in", "r");
  out = fopen("permutation.out", "w");
  assert(in && out);


  fscanf(in, "%d", &n);
  while (n > 0) {
    for (i = 0; i < n; i++) {
      fscanf(in, "%d", a+i);
    }
    for (i = n-1, k = 0; i >= 0; i--, k++) {
      /* shift last a[i] elements one to the right */
      for (j = k-1; j >= a[i]; j--) {
	perm[j+1] = perm[j];
      }
      
      perm[a[i]] = i+1;
    }

    for (i = 0; i < n; i++) {
      fprintf(out, "%d%c", perm[i], (i < n-1) ? ',' : '\n');
    }
    
    fscanf(in, "%d", &n);
  }

  return 0;
}
