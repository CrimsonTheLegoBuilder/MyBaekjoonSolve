#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_LEN 20
#define MAX_N 100

typedef long long ELEM;

ELEM gcd(ELEM a, ELEM b)
{
  ELEM r;

  if (a < 0) {
    a = -a;
  }
  if (b < 0) {
    b = -b;
  }

  while (b) {
    r = a % b;
    a = b;
    b = r;
  }

  return a;
}

int fflinsolve(ELEM A[MAX_N][26], ELEM b[MAX_N][MAX_N], int m, int n)
{
  ELEM d, g, t;
  int i, j, k, k_c, k_r, pivot, rank;

  for (i = 0; i < m; i++) {
    for (j = 0; j < m; j++) {
      b[i][j] = (i == j);
    }
  }

  d = 1;
  rank = 0;

  for (k_c = k_r = 0; k_c < n; k_c++) {
    /* eliminate column k_c */
    
    /* find nonzero pivot */
    for (pivot = k_r; pivot < m && !A[pivot][k_c]; pivot++)
      ;
    
    if (pivot < m) {
      rank++;

      /* swap rows pivot and k_r */
      if (pivot != k_r) {
	for (j = k_c; j < n; j++) {
	  t = A[pivot][j];
	  A[pivot][j] = A[k_r][j];
	  A[k_r][j] = t;
	}
	for (j = 0; j < m; j++) {
	  t = b[pivot][j];
	  b[pivot][j] = b[k_r][j];
	  b[k_r][j] = t;
	}
      }

      /* do elimination */
      for (i = k_r+1; i < m; i++) {
	g = 0;
	for (j = k_c+1; j < n; j++) {
	  A[i][j] = (A[k_r][k_c]*A[i][j]-A[i][k_c]*A[k_r][j])/d;
	}
	for (j = 0; j < m; j++) {
	  b[i][j] = (A[k_r][k_c]*b[i][j]-A[i][k_c]*b[k_r][j])/d;
	}
	A[i][k_c] = 0;
      }
      if (d) {
	d = A[k_r][k_c];
      }
      k_r++;
    }
  }

  if (rank == m) {
    return -1;
  }

  /* now normalize */
  g = 0;
  for (k = 0; k < m; k++) {
    g = gcd(g, b[rank][k]);
  }
  for (k = 0; k < m; k++) {
    b[rank][k] /= g;
  }

  return rank;
}


int main(void)
{
  FILE *in, *out;
  int n;
  char words[MAX_N][MAX_LEN+1];
  int i, j, rank;
  ELEM A[MAX_N][26], b[MAX_N][MAX_N];

  in = fopen("stringeq.in", "r");
  out = fopen("stringeq.out", "w");

  while (fscanf(in, "%d", &n) == 1 && n > 0) {
    for (i = 0; i < n; i++) {
      fscanf(in, "%s", words[i]);
    }

    for (i = 0; i < n; i++) {
      for (j = 0; j < 26; j++) {
	A[i][j] = 0;
      }
    }
    for (i = 0; i < n; i++) {
      for (j = 0; words[i][j]; j++) {
	A[i][words[i][j]-'a']++;
      }
    }
    /*
    for (i = 0; i < n; i++) {
      for (j = 0; j < 26; j++) {
	printf("%d ", A[i][j]);
      }
      printf("\n");
    }
    printf("===\n");
    */
    if ((rank = fflinsolve(A, b, n, 26)) >= 0) {
      fprintf(out, "yes\n");
      for (i = 0; i < n; i++) {
	fprintf(out, "%s ", words[i]);
	if (b[rank][i] > 0) {
	  fprintf(out, "L ");
	} else if (b[rank][i] < 0) {
	  fprintf(out, "R ");
	} else {
	  fprintf(out, "N ");
	}
	fprintf(out, "%d\n", abs((int)b[rank][i]));
      }
    } else {
      fprintf(out, "no\n");
    }
  }
  
  return 0;
}
