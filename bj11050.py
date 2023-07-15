import sys

m = int(input())

for _ in range(m):
    k, n = map(int, sys.stdin.readline().strip().split())
    a, b = 1, 1

    for i in range(n - k + 1, n + 1):
        a *= i

    for j in range(1, k + 1):
        b *= j

    print(a // b)
