import sys
from math import gcd

n = int(input())
arr = [int(sys.stdin.readline().strip()) for _ in range(n)]

arr_n = [0]*(n-1)
for i in range(len(arr) - 1):
    arr_n[i] = arr[i + 1] - arr[i]

for i in range(len(arr_n) - 1):
    a, b = arr_n[i], arr_n[i + 1]
    arr_n[i + 1] = gcd(a, b)

m = (arr[-1] - arr[0]) // arr_n[-1]
print(m-n+1)
