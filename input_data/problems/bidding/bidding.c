#include <stdio.h>
#include <stdlib.h>

FILE *in, *out;
int hcp;
int dist[4];           /* 0 = C, 1 = D, 2 = H, 3 = S */
char suit_index[256];
char *suit[4] = { "Clubs", "Diamonds", "Hearts", "Spades" };
int hcp_val[256];
int dist_count[14];
char honor[4][4];      /* i,j -> suit,rank */
                       /* rank = 0,1,2,3 -> A,K,Q,J */

int read_hand(void)
{
  char card[10];
  int i, j;

  hcp = 0;
  for (i = 0; i < 4; i++) {
    dist[i] = 0;
    for (j = 0; j < 4; j++) {
      honor[i][j] = 0;
    }
  }

  for (i = 0; i < 14; i++) {
    dist_count[i] = 0;
  }

  for (i = 0; i < 13; i++) {
    if (fscanf(in, "%s", card) != 1) {
      return 0;
    }

    dist[suit_index[card[0]]]++;
    hcp += hcp_val[card[1]];
    switch (card[1]) {
    case 'A':
      honor[suit_index[card[0]]][0] = 1;
      break;
    case 'K':
      honor[suit_index[card[0]]][1] = 1;
      break;
    case 'Q':
      honor[suit_index[card[0]]][2] = 1;
      break;
    case 'J':
      honor[suit_index[card[0]]][3] = 1;
      break;
    }
  }

  for (i = 0; i < 4; i++) {
    dist_count[dist[i]]++;
  }

  return 1;
}

int rule1(void)
{
  int i;
  if (hcp < 10) return 0;
  for (i = 0; i < 4; i++) {
    if (dist[i] >= 8) {
      fprintf(out, "4 %s\n", suit[i]);
      return 1;
    }
  }
  return 0;
}

int rule2(void)
{
  int i;
  if (hcp >= 10 && hcp <= 13) {
    for (i = 0; i < 4; i++) {
      if (dist[i] >= 7) {
	fprintf(out, "3 %s\n", suit[i]);
	return 1;
      }
    }
  }
  return 0;
}

int rule3(void)
{
  if (hcp >= 8 && hcp <= 9) {
    if (dist[3] >= 7) {
      fprintf(out, "2 %s\n", suit[3]);
      return 1;
    } else if (dist[2] >= 7) {
      fprintf(out, "2 %s\n", suit[2]);
      return 1;
    }    
  }
  return 0;
}

int rule4(void)
{
  if (hcp >= 8 && hcp <= 11) {
    if (dist[3] == 6) {
      fprintf(out, "2 %s\n", suit[3]);
      return 1;
    } else if (dist[2] == 6) {
      fprintf(out, "2 %s\n", suit[2]);
      return 1;
    }
  }
  return 0;
}

int rule5(void)
{
  if (hcp < 11 || hcp > 15) return 0;
  if (dist[3] < 4) return 0;
  if ((dist_count[4] == 3 && dist_count[1] == 1) ||
      (dist_count[5] == 1 && dist_count[4] == 2 && dist_count[0] == 1)) {
    fprintf(out, "2 %s\n", suit[1]);
    return 1;
  }
  return 0;
}

int balanced(void)
{
  return (dist_count[4] == 1 && dist_count[3] == 3) ||
    (dist_count[4] == 2 && dist_count[3] == 1 && dist_count[2] == 1) ||
    (dist_count[5] == 1 && dist_count[3] == 2 && dist_count[2] == 1);
}

int rule6(void) {
  int stopped = 0;
  int i, j;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      if (honor[i][j] && dist[i] >= j+1) {
	stopped++;
	break;
      }
    }
  }

  if (hcp >= 15 && hcp <= 17 && balanced() && stopped >= 3) {
    fprintf(out, "1 No Trump\n");
    return 1;
  }
  return 0;
}

int rule7(void) {
  if (hcp >= 20 && hcp <= 22 && balanced()) {
    fprintf(out, "2 No Trump\n");
    return 1;
  }
  return 0;
}

int rule8(void) {
  if (hcp >= 22) {
    fprintf(out, "2 %s\n", suit[0]);
    return 1;
  }
  return 0;
}

int rule9(void) {
  if (hcp < 13 || hcp > 16) return 0;
  
  if (dist[3] >= 5 && dist[3] >= dist[2]) {
    fprintf(out, "1 %s\n", suit[3]);
  } else if (dist[2] >= 5) {
    fprintf(out, "1 %s\n", suit[2]);
  } else if (dist[1] >= dist[0]) {
    fprintf(out, "1 %s\n", suit[1]);
  } else {
    fprintf(out, "1 %s\n", suit[0]);
  }
  return 1;
}

int rule10(void) {
  int best_suit, best;
  int i;
  
  if (hcp < 17) return 0;

  best_suit = 0;
  best = dist[0];
  for (i = 1; i < 4; i++) {
    if (dist[i] > best) {
      best = dist[i];
      best_suit = i;
    }
  }
  fprintf(out, "1 %s\n", suit[best_suit]);
    
  return 1;
}

int main(void)
{
  int hand_num = 1;

  suit_index['S'] = 3;
  suit_index['H'] = 2;
  suit_index['D'] = 1;
  suit_index['C'] = 0;
  hcp_val['A'] = 4;
  hcp_val['K'] = 3;
  hcp_val['Q'] = 2;
  hcp_val['J'] = 1;

  in = fopen("bidding.in", "r");
  out = fopen("bidding.out", "w");

  while (read_hand()) {
    fprintf(out, "Hand #%d: ", hand_num++);
    if (rule1()) continue;
    if (rule2()) continue;
    if (rule3()) continue;
    if (rule4()) continue;
    if (rule5()) continue;
    if (rule6()) continue;
    if (rule7()) continue;
    if (rule8()) continue;
    if (rule9()) continue;
    if (rule10()) continue;
    fprintf(out, "Pass\n");
  }
  return 0;
}
