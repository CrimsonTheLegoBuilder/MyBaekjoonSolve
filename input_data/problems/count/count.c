#include "string.h"
#include "stdio.h"

FILE *in=0;
FILE *out=0;
char linebuffer[101];

void solve() 
{
    int count=0;
    while (fgets(linebuffer,sizeof(linebuffer),in)!=0) {
	++count;
    }
    fprintf(out,"%d\n",count);
}

int main()
{
    in = fopen("count.in","r");
    out = fopen("count.out","w");
    solve();
    fclose(out);
    fclose(in);
}
    
