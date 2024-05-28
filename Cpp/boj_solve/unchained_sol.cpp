/*
 * ACM ICPC - CERC 2011
 *
 * Sample solution: Unchanged Picture (unchange)
 * Author: Jakub Cerny
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define MAX_LINES 1000
#define EPSILON (0.001)
#define ABS(x)	((x)>0 ? (x) : -(x))
#define HASH_TABLE_SIZE  4567

 /* #define NDEBUG */


 /* every end-point of a segment has coordinates [x,y] */
int points_a[2 * MAX_LINES][2];
int num_points_a;
/* every line contains indexes of its endpoints */
int lines_a[MAX_LINES][2];
int num_lines_a;

/* every end-point of a segment has coordinates [x,y] */
int points_b[2 * MAX_LINES][2];
int num_points_b;
/* every line contains indexes of its endpoints */
int lines_b[MAX_LINES][2];
int num_lines_b;

/* global variables for mapping - rotation, expansion, translation */
float sin_val, cos_val;
int orig_ax, orig_ay, orig_bx, orig_by;
/* global variables for testing, if point lies on a given line */
double seg_a, seg_b, seg_c;

int hash_point_table[HASH_TABLE_SIZE];
int hash_line_table[HASH_TABLE_SIZE];

void init_hash_tables() {
	memset(hash_point_table, 0, sizeof(int) * HASH_TABLE_SIZE);
	memset(hash_line_table, 0, sizeof(int) * HASH_TABLE_SIZE);
}
/* storing values+1 because 0 means empty slot */
void hash_add(int x, int y, int value, int* table) {
	long pos;

	/* hash pair */
	pos = (ABS(x) % HASH_TABLE_SIZE + 997 * (ABS(y) % HASH_TABLE_SIZE)) % HASH_TABLE_SIZE;
	/* search list */
	while (table[pos] != value + 1 && table[pos] != 0) {
		pos++;
		pos %= HASH_TABLE_SIZE;
	}
	if (table[pos] == 0)
		table[pos] = value + 1;
}
/* find pair [x,y] and return stored value or -1 if not found */
int hash_find(int x, int y, int* table, int(*objects)[2]) {
	long pos, index;

	/* hash pair */
	pos = (ABS(x) % HASH_TABLE_SIZE + 997 * (ABS(y) % HASH_TABLE_SIZE)) % HASH_TABLE_SIZE;
	/* simle, but subresutl does not fit into long *
	  pos = (ABS(x) + 997*ABS(y)) % HASH_TABLE_SIZE;
	*/

	/* search list */
	while (table[pos] != 0) {
		index = table[pos] - 1;
		if (objects[index][0] == x && objects[index][1] == y)
			break;
		pos++;
		pos %= HASH_TABLE_SIZE;
	}
	return table[pos] - 1;
}



/* find the point in the array of points
 * returns index of the point
 * if the point do not exists yet, add new point to the array */
int get_point(int x, int y, int* num_points, int(*points)[2]) {
	int i;

	for (i = 0; i < *num_points; i++) {
		if (points[i][0] == x && points[i][1] == y)
			break;
	}
	if (i == *num_points) {
		points[i][0] = x;
		points[i][1] = y;
		(*num_points)++;
	}
	return i;
}


/* returns 1 if sucessfull and 0 in case there is end of input */
int read_logo(int(*points)[2 * MAX_LINES][2], int* num_points, int(*lines)[MAX_LINES][2], int* num_lines) {
	char instruction;
	int x, y;
	int previous_x, previous_y;
	int previous_point, new_point;

	*num_points = 0;
	*num_lines = 0;

	/* starting position of plotter is at [0,0] */
	previous_point = get_point(0, 0, num_points, (*points));
	previous_x = previous_y = 0;
	while (1) {
		scanf(" %c ", &instruction);
		switch (instruction) {
		case 'Q': return 0;

		case 'E': return 1;

		case 'L':
			scanf(" %d %d ", &x, &y);
			new_point = get_point(x + previous_x, y + previous_y, num_points, (*points));
			(*lines)[*num_lines][0] = previous_point;
			(*lines)[*num_lines][1] = new_point;
			(*num_lines)++;
			previous_point = new_point;
			break;
		case 'M':
			scanf(" %d %d ", &x, &y);
			previous_point = get_point(x + previous_x, y + previous_y, num_points, (*points));
			break;
		}
		previous_x = (*points)[previous_point][0];
		previous_y = (*points)[previous_point][1];
	}
}


/* Compute constants for mapping points of logo A to logo B
   providing segment "line_a" which needs to be mapped to segment "line_b". */
void compute_mapping(int line_a, int line_b, int reverse_direction_of_b) {
	int ax2, ay2, bx2, by2;
	int vax, vay, vbx, vby;

	/* compute mapping of points */
	orig_ax = points_a[lines_a[line_a][0]][0];
	orig_ay = points_a[lines_a[line_a][0]][1];
	ax2 = points_a[lines_a[line_a][1]][0];
	ay2 = points_a[lines_a[line_a][1]][1];

	if (reverse_direction_of_b) {
		orig_bx = points_b[lines_b[line_b][1]][0];
		orig_by = points_b[lines_b[line_b][1]][1];
		bx2 = points_b[lines_b[line_b][0]][0];
		by2 = points_b[lines_b[line_b][0]][1];
	}
	else {
		orig_bx = points_b[lines_b[line_b][0]][0];
		orig_by = points_b[lines_b[line_b][0]][1];
		bx2 = points_b[lines_b[line_b][1]][0];
		by2 = points_b[lines_b[line_b][1]][1];
	}

	/* vectors from point 1(origin) to point 2 */
	vax = ax2 - orig_ax;
	vay = ay2 - orig_ay;
	vbx = bx2 - orig_bx;
	vby = by2 - orig_by;

	/* rotations -- when centers of coordinate system moved to [0,0] */
	/* using matrix for rotations:
		 *    [new_x] = [ cos    sin ] * [ x ]
		 *    [new y]   [ -sin   cos ]   [ y ]
		 */
	assert(!(vax == 0 && vay == 0));
	cos_val = (double)(vax * vbx + vay * vby) / (vax * vax + vay * vay);
	if (vax != 0)
		sin_val = (double)(cos_val * vay - vby) / vax;
	else
		sin_val = (double)vbx / vay;
}

void map_point(int x, int y, float* new_x, float* new_y) {
	x = x - orig_ax;
	y = y - orig_ay;
	*new_x = cos_val * x + sin_val * y + orig_bx;
	*new_y = -sin_val * x + cos_val * y + orig_by;
}


/* Verify similarity of logo A to Logo B, when we try to map segment_a to segment b */
/* Returns 1 if similar, 0 otherwise */
int verify_similarity(int line_a, int line_b, int reverse_direction_of_b) {
	int seg;
	int p1, p2, t_p1, t_p2; 			/* endpoints of segments */
	float t_x1, t_x2, t_y1, t_y2; 	/* transformed point coordinates */
	int x1, x2, y1, y2;

	/* compute mapping of points */
	compute_mapping(line_a, line_b, reverse_direction_of_b);

	/* verify all segments */
	for (seg = 0; seg < num_lines_a; seg++) {
		/* get transformed segment end-points */
		p1 = lines_a[seg][0];
		p2 = lines_a[seg][1];
		map_point(points_a[p1][0], points_a[p1][1], &t_x1, &t_y1);
		map_point(points_a[p2][0], points_a[p2][1], &t_x2, &t_y2);
		/* round points to integers */
		x1 = round(t_x1);
		x2 = round(t_x2);
		y1 = round(t_y1);
		y2 = round(t_y2);

		/* find segment end-points in logo B */
		t_p1 = hash_find(x1, y1, hash_point_table, points_b);
		t_p2 = hash_find(x2, y2, hash_point_table, points_b);

		if (t_p1 < 0 || t_p2 < 0) {
			/* logos cannot be similar, because transformed end-point is not in logo B */
			return 0;
		}
		/* find line t_p1 -- t_p2 in logo B */
		if (hash_find(t_p1, t_p2, hash_line_table, lines_b) == -1
			&& hash_find(t_p2, t_p1, hash_line_table, lines_b) == -1)
			/* transformed segments is not in logo B */
			return 0;
	}
	return 1;
}

/* computes constants for quick testing, if point [x,y] lies on line determined by given points */
void set_line(int x1, int y1, int x2, int y2) {
	if (y1 == y2) {
		seg_a = 0;
		seg_b = 1;
	}
	else {
		seg_a = 1;
		seg_b = (float)(x2 - x1) / (y1 - y2);
	}
	seg_c = -seg_a * x1 - seg_b * y1;
}

/* test if point [x,y] lies on line determined by segment given in function set_line() */
int lies_on_line(int x, int y) {
	return ABS(seg_a * x + seg_b * y + seg_c) < EPSILON;
}

int cmp_pts(const void* a, const void* b) {
	int ax, bx, ay, by;
	ax = ((int*)a)[0];
	bx = ((int*)b)[0];
	ay = ((int*)a)[1];
	by = ((int*)b)[1];

	if (ax == bx)	return (ay - by);
	else		return (ax - bx);
}

/* simplify logo - covert overlapping segments into one, two neighbouring segments on a line into one, etc */
void simplify_logo(int(*points)[2], int* num_points, int(*lines)[2], int* num_lines) {
	int seg, cur_seg, last_seg = 0;
	int p1, p2, cur_point, seg_start_pt = 0, seg_end_pt;
	int i, covered;
	char processed_segs[MAX_LINES];
	int linepoints[2 * MAX_LINES][4]; /* strores [x, y, point ind, seg ind] for points on the line */
	int num_linepoints;

	memset(processed_segs, 0, MAX_LINES * sizeof(char));

	/* test all segments determined by segments in logo */
	for (seg = 0; seg < *num_lines; seg++)
	{  	/* simplify segments on the line determined by seg */
		if (!processed_segs[seg]) {
			p1 = lines[seg][0];
			p2 = lines[seg][1];
			set_line(points[p1][0], points[p1][1], points[p2][0], points[p2][1]);

			/* check all segments if they lie on this line */
			num_linepoints = 0;
			for (i = 0; i < *num_lines; i++) {
				p1 = lines[i][0];
				p2 = lines[i][1];
				if (!processed_segs[i]
					&& lies_on_line(points[p1][0], points[p1][1])
					&& lies_on_line(points[p2][0], points[p2][1])) {
					/* seg is on the line, add it to queue of endpoints */
					linepoints[num_linepoints][0] = points[p1][0];
					linepoints[num_linepoints][1] = points[p1][1];
					linepoints[num_linepoints][2] = p1;
					linepoints[num_linepoints][3] = i;
					num_linepoints++;
					linepoints[num_linepoints][0] = points[p2][0];
					linepoints[num_linepoints][1] = points[p2][1];
					linepoints[num_linepoints][2] = p2;
					linepoints[num_linepoints][3] = i;
					num_linepoints++;
				}
			}

			qsort(linepoints, num_linepoints, 4 * sizeof(int), cmp_pts);

			/* moving the sweep line - take sorted end points as events */
			covered = 0;
			seg_end_pt = -1;
			for (i = 0; i < num_linepoints; i++) {
				cur_point = linepoints[i][2];
				cur_seg = linepoints[i][3];

				if (processed_segs[cur_seg] == 0) {
					/* next segment starts covering */
					if (!covered) {
						if (seg_end_pt == cur_point) {
							/* there is an extension, delete last segment */
							while (last_seg >= *num_lines) {
								last_seg = lines[last_seg][0];
							}
							(*num_lines)--;
							lines[last_seg][0] = lines[*num_lines][0];
							lines[last_seg][1] = lines[*num_lines][1];
							processed_segs[last_seg] = processed_segs[*num_lines];
							if (*num_lines != last_seg) lines[*num_lines][0] = last_seg; /* store position, where was the line moved to */
						}
						else {
							/* start new covering segment */
							seg_start_pt = cur_point;
						}
					}
					processed_segs[cur_seg]++;
					covered++;
				}
				else {
					/* end of a cover */
					covered--;
					processed_segs[cur_seg]++;
					if (covered) {
						/* delete segment */
						while (cur_seg >= *num_lines) {
							cur_seg = lines[cur_seg][0];
						}
						(*num_lines)--;
						lines[cur_seg][0] = lines[*num_lines][0];
						lines[cur_seg][1] = lines[*num_lines][1];
						processed_segs[cur_seg] = processed_segs[*num_lines];
						if (*num_lines != cur_seg) lines[*num_lines][0] = cur_seg; /* store position, where was the line moved to */
					}
					else {
						/* this is last point of the cover */

						/* rewrite by covering segment */
						while (cur_seg >= *num_lines) {
							cur_seg = lines[cur_seg][0];
						}
						lines[cur_seg][0] = seg_start_pt;
						lines[cur_seg][1] = cur_point;

						seg_end_pt = cur_point;
						last_seg = cur_seg;
					}
				}
			}
		}
	}
}

int main() {
	int i, j;
	int similar;

	while (read_logo(&points_a, &num_points_a, &lines_a, &num_lines_a) &&
		read_logo(&points_b, &num_points_b, &lines_b, &num_lines_b)) {

		/* simplifycations of each logo */
		simplify_logo(points_a, &num_points_a, lines_a, &num_lines_a);
		simplify_logo(points_b, &num_points_b, lines_b, &num_lines_b);

		/* try to map each segment of A to each segment of B and verify similarity for such a mapping/transformation */
		/* (mapping vectors instead of segments, thus must try both possible directions of segment in B) */
		similar = 0;
		if (num_lines_a == num_lines_b) {
			if (num_lines_a == 0)
				similar = 1;
			else {
				/* prepare hash tables - look up tables to find a point by given coordinates */
				init_hash_tables();
				for (i = 0; i < num_points_b; i++)
					hash_add(points_b[i][0], points_b[i][1], i, hash_point_table);
				for (i = 0; i < num_lines_b; i++)
					hash_add(lines_b[i][0], lines_b[i][1], i, hash_line_table);

				/* try match each segment from A to all segments from B */
				for (i = 0; i < num_lines_a && !similar; i++)
					for (j = 0; j < num_lines_b && !similar; j++) {
						similar += verify_similarity(i, j, 0);
						similar += verify_similarity(i, j, 1);
					}
			}
		}

		if (similar)
			printf("YES\n");
		else
			printf("NO\n");
	}

	return 0;
}