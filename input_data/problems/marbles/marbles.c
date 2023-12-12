#include <stdio.h>

#define MAX_MARBLE 500
#define MAX_INDEX ((MAX_MARBLE+1)*(MAX_MARBLE+1))

FILE *in, *out;

int myindex(int *config)
{
  return config[0]*(MAX_MARBLE+1)+config[1];
}

void index2config(int index, int *config, int sum)
{
  config[0] = index / (MAX_MARBLE+1);
  config[1] = index % (MAX_MARBLE+1);
  config[2] = sum - config[0] - config[1];
}

void print_config(int *config)
{
  fprintf(out, "%4d%4d%4d\n", config[0], config[1], config[2]);
}

void print_path(int *config, int sum, int *orig, int *pred)
{
  int newconfig[3];
  if (config[0] == orig[0] && config[1] == orig[1] && config[2] == orig[2])
    return;

  index2config(pred[myindex(config)], newconfig, sum);
  print_path(newconfig, sum, orig, pred);
  print_config(config);
}

void print_add(int i, int j, int sum)
{
  int config[3];

  index2config(i, config, sum);
  printf("%4d %4d %4d -> ", config[0], config[1], config[2]);
  index2config(j, config, sum);
  printf("%4d %4d %4d\n", config[0], config[1], config[2]);
}

void search(int *config) 
{
  char visited[MAX_INDEX];
  int pred[MAX_INDEX], queue[MAX_INDEX+1];
  int i, j, head, tail;
  int sum = config[0] + config[1] + config[2];
  int from, to, howmany;
  int orig[3], goal;

  orig[0] = config[0];
  orig[1] = config[1];
  orig[2] = config[2];

  for (i = 0; i < MAX_INDEX; i++) {
    visited[i] = 0;
    pred[i] = -1;
  }
  
  head = tail = 0;
  i = myindex(config);
  visited[i] = 1;
  pred[i] = -1;
  queue[tail++] = i;

  config[0] = config[1] = config[2] = sum/3;
  goal = myindex(config);

  while (head != tail) {
    i = queue[head++];
    if (i == goal) break;
    index2config(i, config, sum);
        
    for (from = 0; from < 3; from++) {
      for (to = 0; to < 3; to++) {
	if (from == to) continue;
	if (config[from] < config[to]) continue;
	howmany = config[to];
	config[from] -= howmany;
	config[to] += howmany;
	j = myindex(config);
	if (!visited[j]) {
	  pred[j] = i;
	  visited[j] = 1;
	  queue[tail++] = j;
	  /*	  print_add(i, j, sum); */
	}
	config[from] += howmany;
	config[to] -= howmany;
      }
    }
  }

  print_config(orig);

  config[0] = config[1] = config[2] = sum/3;
  if (!visited[myindex(config)])
    return;
    
  print_path(config, sum, orig, pred);
}

void solve(int *config)
{
  if ((config[0] + config[1] + config[2]) % 3 == 0) {
    search(config);
  } else {
    print_config(config);
  }
}

int main(void)
{
  int config[3];

  in = fopen("marbles.in", "r");
  out = fopen("marbles.out", "w");

  while (fscanf(in, "%d %d %d", config, config+1, config+2) == 3) {
    solve(config);
    fprintf(out, "============\n");
  }
  return 0;
}
