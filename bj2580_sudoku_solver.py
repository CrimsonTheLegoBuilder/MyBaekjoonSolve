import sys

sudoku = []
blank = []

for a in range(9):
    row = list(map(int, sys.stdin.readline().strip().split()))
    for b in range(9):
        if row[b] == 0:
            blank.append((a, b))
    sudoku.append(row)


def row_check(ans, x):
    for i in range(9):
        if ans == sudoku[x][i]:
            return False
    return True


def col_check(ans, y):
    for i in range(9):
        if ans == sudoku[i][y]:
            return False
    return True


def box_check(ans, x, y):
    X = x // 3 * 3
    Y = y // 3 * 3
    for i in range(3):
        for j in range(3):
            if ans == sudoku[X + i][Y + j]:
                return False
    return True


def sudoku_solver(depth):
    if depth == len(blank):
        for i in range(9):
            print(*sudoku[i])
        exit(0)
    x = blank[depth][0]
    y = blank[depth][1]
    for ans in range(1, 10):
        if row_check(ans, x) and col_check(ans, y) and box_check(ans, x, y):
            sudoku[x][y] = ans
            sudoku_solver(depth + 1)
            sudoku[x][y] = 0


sudoku_solver(0)
