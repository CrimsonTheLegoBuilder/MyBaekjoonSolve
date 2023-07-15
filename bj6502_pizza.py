import sys
cnt = 1

while True:
    a = list(map(int, sys.stdin.readline().strip().split()))
    if a == [0]:
        break
    elif (a[1] ** 2 + a[2] ** 2) ** 0.5 > a[0] * 2:
        sys.stdout.write(f'Pizza {cnt} does not fit on the table.\n')
    else:
        sys.stdout.write(f'Pizza {cnt} fits on the table.\n')
    cnt += 1
