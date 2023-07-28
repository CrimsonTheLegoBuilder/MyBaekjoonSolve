import sys

while True:
    a, b = map(int, sys.stdin.readline().strip().split())
    if not a and not b:
        exit(0)
    if a > b:
        print('YES')
        print('YES')
    else:
        print('NO')
        print('NO')
