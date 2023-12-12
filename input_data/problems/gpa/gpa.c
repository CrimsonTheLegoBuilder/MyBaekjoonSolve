#include <stdio.h>

int main(void)
{
  FILE *in, *out;
  int c;
  int sum, count;
  char error;

  in = fopen("gpa.in", "r");
  out = fopen("gpa.out", "w");

  while ((c = fgetc(in)) != EOF) {
    sum = 0;
    count = 0;
    error = 0;
    do {
      switch (c) {
      case 'A':
	sum += 4;
	count++;
	break;
      case 'B':
	sum += 3;
	count++;
	break;
      case 'C':
	sum += 2;
	count++;
	break;
      case 'D':
	sum += 1;
	count++;
	break;
      case 'F':
	count++;
	break;
      case ' ':
      case '\n':
	break;
      default:
	error = 1;
      }
    } while ((c = fgetc(in)) != '\n');
    
    if (error) {
      fprintf(out, "Unknown letter grade in input\n");
    } else {
      fprintf(out, "%.2f\n", (double)sum/count);
    }
  }

  return 0;
}
