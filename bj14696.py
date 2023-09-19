import sys

for _ in range(int(sys.stdin.readline().strip())):
    A = [0]*5
    B = [0]*5
    N1, *a = map(int, sys.stdin.readline().strip().split())
    for i in range(N1):
        A[a[i]] += 1
    N2, *b = map(int, sys.stdin.readline().strip().split())
    for i in range(N2):
        B[b[i]] += 1
    for i in range(4, -1, -1):
        if A[i] > B[i]:
            print('A')
            break
        elif A[i] < B[i]:
            print('B')
            break
        elif not i:
            print('D')
