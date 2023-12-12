#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *in, *out, *tmp, *vfy;
int case_num = 1;

#define MAX_MARBLE 500
#define MAX_INDEX ((MAX_MARBLE+1)*(MAX_MARBLE+1))

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
  fprintf(tmp, "%4d%4d%4d\n", config[0], config[1], config[2]);
}

void search(int *config) 
{
  char visited[MAX_INDEX];
  int dist[MAX_INDEX];
  int pred[MAX_INDEX], queue[MAX_INDEX+1];
  int i, j, head, tail;
  int sum = config[0] + config[1] + config[2];
  int from, to, howmany;
  int orig[3], goal;
  int newconfig[3];
  int found;

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
  dist[i] = 0;
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
	  dist[j] = dist[i]+1;
	}
	config[from] += howmany;
	config[to] -= howmany;
      }
    }
  }

  config[0] = config[1] = config[2] = sum/3;
  if (!visited[myindex(config)])
    return;
  
  config[0] = orig[0];
  config[1] = orig[1];
  config[2] = orig[2];
  for (i = 0; i < dist[goal]; i++) {
    if (fscanf(out, "%d %d %d\n", newconfig, newconfig+1, newconfig+2) != 3) {
      fprintf(vfy, "Case %d: wrong number of moves\n", case_num);
      fprintf(vfy, "WRONG OUTPUT\n");
      exit(0);
    }
    print_config(newconfig);
    
    found = 0;
    for (from = 0; from < 3; from++) {
      for (to = 0; to < 3; to++) {
	if (from == to) continue;
	if (config[from] < config[to]) continue;
	howmany = config[to];
	config[from] -= howmany;
	config[to] += howmany;
	if (config[0] == newconfig[0] && config[1] == newconfig[1] &&
	    config[2] == newconfig[2]) {
	  found = 1;
	}
	config[from] += howmany;
	config[to] -= howmany;
      }
    }
    if (!found) {
      fprintf(vfy, "Case %d: move %d is invalid\n", case_num, i+1);
      fprintf(vfy, "WRONG OUTPUT\n");
      exit(0);
    }
    config[0] = newconfig[0];
    config[1] = newconfig[1];
    config[2] = newconfig[2];
  }
  if (!(config[0] == config[1] && config[1] == config[2])) {
    fprintf(vfy, "Case %d: final configuration is wrong\n", case_num);
    fprintf(vfy, "WRONG OUTPUT\n");
    exit(0);
  }
}

void solve(int *config)
{
  int newconfig[3];

  print_config(config);
  if (fscanf(out, "%d %d %d\n", newconfig, newconfig+1, newconfig+2) != 3) {
    fprintf(vfy, "Case %d: wrong number of moves\n", case_num);
    fprintf(vfy, "WRONG OUTPUT\n");
    exit(0);
  }
  if (!(config[0] == newconfig[0] && config[1] == newconfig[1] &&
	config[2] == newconfig[2])) {
    fprintf(vfy, "Case %d: wrong initial configuration\n", case_num);
    fprintf(vfy, "WRONG OUTPUT\n");
    exit(0);
  }
    
  if ((config[0] + config[1] + config[2]) % 3 == 0) {
    search(config);
  }
}

int main(void)
{
  int config[3];
  char line[1024];

  in = fopen("marbles.in", "r");
  out = fopen("marbles.out", "r");
  tmp = fopen("marbles.tmp", "w");
  vfy = fopen("marbles.vfy", "w");

  while (fscanf(in, "%d %d %d", config, config+1, config+2) == 3) {
    solve(config);
    fprintf(tmp, "============\n");
    if (!fgets(line, 1024, out) || strcmp(line, "============\n")) {
      fprintf(vfy, "Case %d: wrong number of moves\n", case_num);
      fprintf(vfy, "WRONG OUTPUT\n");
      exit(0);
    }
    case_num++;
  }
  fclose(in);
  fclose(out);
  fclose(tmp);

  if (system("diff marbles.out marbles.tmp > /dev/null")) {
    if (system("diff -biw marbles.out marbles.tmp > /dev/null")) {
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
