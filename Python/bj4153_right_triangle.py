import sys

while True:
    A = list(map(int, sys.stdin.readline().strip().split()))
    A.sort()
    if (A[0] == 0) & (A[1] == 0) & (A[2] == 0):
        break
    else:
        if A[2] ** 2 == A[0] ** 2 + A[1] ** 2:
            print('right')
        else:
            print('wrong')