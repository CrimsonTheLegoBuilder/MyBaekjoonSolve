n = int(input())

for a in range(1, n+1):
    sudoku = []
    flag = 0

    for _ in range(9):
        row = list(map(int, input().split()))
        sudoku.append(row)

    for c in sudoku:
        row_set = set(c)

        if len(row_set) != 9:
            print('#' + str(a) + ' 0')
            flag += 1
            break

    if flag != 1:
        for i in range(9):
            column = set()
            for j in sudoku:
                column.add(j[i])

            if len(column) != 9:
                print('#' + str(a) + ' 0')
                flag += 1
                break

    if flag != 1:
        for b in range(0, 9, 3):
            box1 = set()
            box2 = set()
            box3 = set()
            for k in range(b, b+3):
                box1.add(sudoku[0][k])
                box1.add(sudoku[1][k])
                box1.add(sudoku[2][k])
                box2.add(sudoku[3][k])
                box2.add(sudoku[4][k])
                box2.add(sudoku[5][k])
                box3.add(sudoku[6][k])
                box3.add(sudoku[7][k])
                box3.add(sudoku[8][k])

            if len(box1) != 9 or len(box2) != 9 or len(box3) != 9:
                print('#' + str(a) + ' 0')
                flag += 1
                break

    if flag != 1:
        print('#' + str(a) + ' 1')
