import sys

n = int(sys.stdin.readline().strip())

score = sorted([int(sys.stdin.readline().strip()) for _ in range(n)])

if n == 0:
    print(0)

else:
    cnt = 0
    total = 0
    a = int(n * 0.15 + 0.5)
    for i in range(a, n - a):
        total += score[i]
        cnt += 1
    print(int(total/cnt + 0.5))
